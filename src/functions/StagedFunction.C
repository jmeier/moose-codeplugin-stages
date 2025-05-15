//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "MooseError.h"

// MOOSE includes
#include "InputParameters.h"
#include "StagedFunction.h"
#include "FEProblemBase.h"
#include "Stage.h"
#include "Stages.h"
#include "Transient.h"

registerMooseObject(MOOSEAPPNAME, StagedFunction);

InputParameters
StagedFunction::validParams()
{
  InputParameters params = Function::validParams();

  params.addClassDescription("Function defined by means of blocks of type StagedFunctionValueChange.");

  return params;
}

StagedFunction::StagedFunction(const InputParameters & parameters) : Function(parameters) {}

Real
StagedFunction::value(Real t, const Point & /*p*/) const
{
  if (t == _last_value_t)
  {
    return _last_value;
  };

  Real y = getValueInternal(t, false);

  // std::cout << "############ \"" << name() << "\" new function value: " << y << "\n";

  _last_value_t = t;
  _last_value = y;

  return y;
}

RealGradient
StagedFunction::gradient(Real /*t*/, const Point & /*p*/) const
{
  // this staged function only depends on time
  return 0;
}

Real
StagedFunction::timeDerivative(Real t, const Point & /*p*/) const
{
  if (t == _last_timeDerivative_t)
    return _last_timeDerivative;

  Real y = getValueInternal(t, true);

  _last_timeDerivative_t = t;
  _last_timeDerivative = y;

  return y;
}

RealVectorValue
StagedFunction::vectorValue(Real /*t*/, const Point & /*p*/) const
{
  mooseError("The vectorValue method is not defined in StagedFunction");
}

void
StagedFunction::initialSetup()
{
}

double
StagedFunction::getValueInternal(const Real t, const bool timeDerivative) const
{
  double y = 0.0;
  Real last_time = 0.0;

  // find the stages-object
  FEProblemBase * fe_problem = this->getParam<FEProblemBase *>("_fe_problem_base");
  if (fe_problem->hasUserObject("Stages") == false)
    mooseError("Stages user object not found.");
  Stages & stgs = fe_problem->getUserObject<Stages>("Stages");

  // get the start time of the executioner
  const auto transient_executioner = dynamic_cast<Transient *>(_app.getExecutioner());
  const auto exec_start_time = transient_executioner->getStartTime();

  // iterate all stages
  std::vector<std::reference_wrapper<Stage>> vecStages = stgs.getStages();
  int n = vecStages.size();
  for (int i = 0; i < n; ++i)
  {
    Stage & stg = vecStages[i].get();
    std::string function_name = name();

    Stage & prev_stg = vecStages[std::max(0, i - 1)].get();

    const Real stage_start_time = (i == 0) ? (exec_start_time) : (prev_stg.getStageTime());

    // iterate all items of this stage
    auto items = stg.getItems();
    int m = items.size();
    for (int j = 0; j < m; ++j)
    {
      // we need only item of type StagedFunctionValueChange
      GeneralUserObject & item = items[j].get();

      if (typeid(item) == typeid(StagedFunctionValueChange))
      {
        StagedFunctionValueChange & fvc = dynamic_cast<StagedFunctionValueChange &>(item);

        auto funcIndex = fvc.getIndexOfFunction(function_name);
        if (funcIndex >= 0)
        {
          auto t1 = fvc.getStartTime();
          if (std::isnan(t1))
          {
            if (i == 0) // this is the first stage!
              t1 = stage_start_time;
            else
              t1 = prev_stg.getStageTime();
          }

          auto t2 = fvc.getEndTime();

          if (t1 < last_time)
          {
            mooseError("Time spans for staged function \"" + function_name +
                       "\" are overlapping. Previous time was " + std::to_string(last_time) +
                       " and current time is " + std::to_string(t1) + ".");
          };

          if (t1 <= t)
          {
            if (t2 >= t)
            {
              if (timeDerivative == true)
              {
                return fvc.getTimeDerivative(funcIndex, t, y, stage_start_time);
              }
              else
              {
                return fvc.getValue(funcIndex, t, y, stage_start_time);
              };
            }
            else
            {
              y = fvc.getNewValue(funcIndex);
            };
          };

          last_time = t2;
        };
      };
    };
  };

  if (timeDerivative == true)
  {
    return 0.0;
  }
  else
  {
    return y;
  };
}
