//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "StagedFunctionValueChange.h"
#include <regex>
#include <math.h>

registerMooseObject(MOOSEAPPNAME, StagedFunctionValueChange);

InputParameters
StagedFunctionValueChange::validParams()
{
  InputParameters params = StagedBase::validParams();
  // params.declareControllable("enable"); // allows Control to enable/disable this type of object
  // params.registerBase("StagedFunctionValueChange");
  params.addRequiredParam<std::vector<std::string>>("function_names",
                                       "Names of the staged state variable.");
  params.addRequiredParam<std::vector<double>>("new_values",
                                  "Values of the variable at end time.");
  params.addParam<std::string>("start_time", "t",
                               "Start time for the transition to the new value.");
  params.addParam<std::string>("end_time", "t",
                               "End time at which the new value must be fully reached.");
  MooseEnum step_function_type_choice("Linear=0 Smooth=1 Perlin=2", "Perlin");
  params.addParam<MooseEnum>("step_function_type", step_function_type_choice,
                             "Type of the step function used for the transition.");
  params.addClassDescription("User object that holds a single state change for a state variable.");
  return params;
}

StagedFunctionValueChange::StagedFunctionValueChange(const InputParameters & parameters)
  : StagedBase(parameters),
    _function_names(getParam<std::vector<std::string>>("function_names")),
    _step_function_type(getParam<MooseEnum>("step_function_type").getEnum<StepFunctionType>()),
    _new_values(getParam<std::vector<double>>("new_values")),
    _start_time(parseTime(getParam<std::string>("start_time"))),
    _end_time(parseTime(getParam<std::string>("end_time")))
{
  // consistency check: start time must not be larger than end time
  if (_start_time > _end_time)
    mooseError("Start time is larger than end time in stage \"" + getStage()->getName() + "\"." );
}

void
StagedFunctionValueChange::setup(std::shared_ptr<FEProblemBase> p)
{
  for (const auto & function_name : _function_names)
  {
    if (p->hasFunction(function_name) == false)
    {
      mooseError("Function \"" + function_name + "\" not found. Please add a function of this name and of type 'StagedFunction'.");
      // TODO: add function if missing (the code below comes too late...
      auto type = "StagedFunction";
      auto original_params = _factory.getValidParams(type);
      p->addFunction(type, function_name, original_params);
    };
    // StagedFunction & f = p->getFunction<Function>(_function_name);
  }
}

std::vector<std::string>
StagedFunctionValueChange::getFunctionNames()
{
  return _function_names;
}

int
StagedFunctionValueChange::getIndexOfFunction(std::string function_name)
{
  auto it = std::find(_function_names.begin(), _function_names.end(), function_name);
  if (it == _function_names.end())
  {
    // name not in vector
    return -1;
  } else
  {
    auto index = std::distance(_function_names.begin(), it);
    return index;
  }
}


StagedFunctionValueChange::StepFunctionType
StagedFunctionValueChange::getStepFunctionType()
{
  return _step_function_type;
}

std::vector<double>
StagedFunctionValueChange::getNewValues()
{
  return _new_values;
}

double
StagedFunctionValueChange::getNewValue(const int funcIndex)
{
  return _new_values[funcIndex];
}

Real
StagedFunctionValueChange::getStartTime()
{
  return _start_time;
}

Real
StagedFunctionValueChange::getEndTime()
{
  return _end_time;
}

std::vector<Real>
StagedFunctionValueChange::getTimesForTimeStepper()
{
  return {_start_time, _end_time};
}

double
StagedFunctionValueChange::getValue(const int funcIndex, const Real t, const double old_value)
{
  if (t < _start_time)
  {

    // we are before _start_time - just return the old value
    return old_value;

  } else if (t >= _end_time) {

    // we are at or after _end_time - just return the new value
    return _new_values[funcIndex];

  } else {

    // we are in between _start_time and _end_time
    // let's calculate the dimensionless factor 'f' for time [0..1]
    const double f = (t - _start_time) / (_end_time - _start_time);

    if (_step_function_type == StepFunctionType::LINEAR)
      return old_value + (_new_values[funcIndex] - old_value) * f;

    if (_step_function_type == StepFunctionType::SMOOTH)
    {
      const double p = f * f * (3.0 - 2.0 * f);
      return old_value + (_new_values[funcIndex] - old_value) * p;
    };

    if (_step_function_type == StepFunctionType::PERLIN)
    {
      const double p = f * f * f * (f * (6.0 * f - 15.0) + 10.0);
      return old_value + (_new_values[funcIndex] - old_value) * p;
    };

    mooseError("Unsupported value for \"step_function_type\".");
  }
}

double
StagedFunctionValueChange::getTimeDerivative(const int funcIndex, const Real t, const double old_value)
{
  if (t < _start_time)
  {

    // we are before _start_time - time derivative is zero
    return 0;

  } else if (t >= _end_time) {

    // we are at or after _end_time - time derivative is zero
    return 0;

  } else {

    // please note: this code path is never active if _start_time
    // equals _end_time, so no need for checking

    if (_step_function_type == StepFunctionType::LINEAR)
      return (_new_values[funcIndex] - old_value) / (_end_time - _start_time);

    if (_step_function_type == StepFunctionType::SMOOTH)
    {
      Real const x = (t -_start_time) / (_end_time - _start_time);
      return (old_value - _new_values[funcIndex]) * 6 * (-1 + x) * x;
    };

    if (_step_function_type == StepFunctionType::PERLIN)
    {
      Real const x = (t -_start_time) / (_end_time - _start_time);
      return (old_value - _new_values[funcIndex]) * -30 * (-1 + x) * (-1 + x) * x * x;
    };

    mooseError("Unsupported value for \"step_function_type\".");
  }
}

Real
StagedFunctionValueChange::parseTime(std::string s)
{
  // regex for parsing strings like "t-1.6E+12.23", "t + 0.1", or just "t"
  std::regex rx("^\\s*t\\s*(?:([+-])\\s*([+-]?[0-9]*[.]?[0-9]*\\s*[Ee]?\\s*[+-]?\\s*[0-9.]+))?\\s*$");
  std::smatch m;
  if (std::regex_match(s, m, rx))
  {
    // we've got a regex match - relative point in time given

    std::string g1 = m[1].str();  // plus or minus
    std::string g2 = m[2].str();  // offset as floating point number

    auto stage = getStage();
    auto stage_t = stage->getStageTime();

    if (g1 == "" && g2 == "")
    {
      // just "t" - we return stage_t
      return stage_t;
    } else {
      // we've got an offset; lets parse and apply it to stage_t
      Real res;
      std::stringstream sval(g2);
      sval >> res;

      if (g1 == "+")
      {
        return stage_t + res;
      } else {
        return stage_t - res;
      };
    };
  } else {
    // absolute point in time given
    Real res;
    std::stringstream sval(s);
    sval >> res;

    return res;
  }
}
