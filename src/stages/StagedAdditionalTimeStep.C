//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "StagedAdditionalTimeStep.h"
#include <math.h>

registerMooseObject(MOOSEAPPNAME, StagedAdditionalTimeStep);

InputParameters
StagedAdditionalTimeStep::validParams()
{
  InputParameters params = StagedBase::validParams();
  // params.declareControllable("enable"); // allows Control to enable/disable this type of object
  // params.registerBase("StagedAdditionalTimeStep");

  // allow the user to specify explicitly points in time
  params.addParam<std::string>("time",
                               "",
                               "Time of the additional time step. Multiple times may be "
                               "specified using ';' as a delimiter.");

  // split the duration of the stage in equally spaced parts given by count
  params.addParam<int>("count",
                       0,
                       "The Number of additional time steps to be distributed equally over the "
                       "duration of the stage.");

  // split the duration of the stage in equally spaced parts given by length
  MooseEnum delta_time_align_choice("Start=0 End=1", "Start");
  params.addParam<Real>(
      "delta_time",
      0,
      "Length of additional timesteps. If the duration of the stage is not a multiple of this "
      "length, the parameter 'delta_time_align' defines the alignment to use.");
  params.addParam<MooseEnum>("delta_time_align",
                             delta_time_align_choice,
                             "Type of the alignment to be used for delta_time.");

  params.addClassDescription("User object that holds additional time steps.");
  return params;
}

StagedAdditionalTimeStep::StagedAdditionalTimeStep(const InputParameters & parameters)
  : StagedBase(parameters),
    _times(parseTimes(getParam<std::string>("time"), ";")),
    _count(getParam<int>("count")),
    _delta_time(getParam<Real>("delta_time")),
    _delta_time_align(getParam<MooseEnum>("delta_time_align").getEnum<DeltaTimeAlignType>())
{
  if (_count < 0)
    paramError("count", "Parameter count must not be negative.");

  if (_delta_time < 0)
    paramError("delta_time", "Parameter delta_time must not be negative.");
}

std::vector<Real>
StagedAdditionalTimeStep::getAdditionalTimes()
{
  return _times;
}

int
StagedAdditionalTimeStep::getAdditionalCount()
{
  return _count;
}

std::vector<Real>
StagedAdditionalTimeStep::getTimesForTimeStepper(const Real stage_start_time)
{
  auto stage = getStage();
  auto stage_end_time = stage->getStageTime();

  std::vector<Real> result;

  // copy all explicitly given times
  result.insert(result.end(), _times.begin(), _times.end());

  // copy times given by 'count'
  if (_count > 0)
  {
    const auto dt = (stage_end_time - stage_start_time) / (_count + 1);
    for (auto i = 0; i < _count; i++)
    {
      const auto t = stage_start_time + (i + 1) * dt;
      result.push_back(t);
    }
  }

  // copy the times given by delta_time
  if (_delta_time > 0 && (stage_end_time - stage_start_time) > _delta_time)
  {
    const uint n = (stage_end_time - stage_start_time) / _delta_time;
    for (uint i = 0; i < n; i++)
    {
      if (_delta_time_align == DeltaTimeAlignType::START)
      {
        const auto t = stage_start_time + (i + 1) * _delta_time;
        result.push_back(t);
      }
      if (_delta_time_align == DeltaTimeAlignType::END)
      {
        const auto t = stage_end_time - (i + 1) * _delta_time;
        result.push_back(t);
      }
    }
  }

  // return the resulting vector
  return result;
}
