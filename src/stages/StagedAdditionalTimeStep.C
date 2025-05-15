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

  // split the duration of the stage in equally spaced parts
  params.addParam<int>("count",
                       0,
                       "The Number of additional time steps to be distributed equally over the "
                       "duration of the stage.");

  // split the duration of the stage in equally spaced parts
  // MooseEnum delta_time_align_choice("Start=0 End=1", "Start");
  // params.addParam<Real>("delta_time", "...");
  // params.addParam<MooseEnum>("delta_time_align",
  //                            delta_time_align_choice,
  //                            "Type of the alignment to be used for delta_time.");

  params.addClassDescription("User object that holds additional time steps.");
  return params;
}

StagedAdditionalTimeStep::StagedAdditionalTimeStep(const InputParameters & parameters)
  : StagedBase(parameters),
    _times(parseTimes(getParam<std::string>("time"), ";")),
    _count(getParam<int>("count"))
{
  if (_count < 0)
    paramError("count", "Parameter count must not be negative.");
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
  std::vector<Real> result;

  // copy all explicitly given times
  result.insert(result.end(), _times.begin(), _times.end());

  // copy times given by 'count'
  if (_count > 0)
  {
    auto stage = getStage();
    auto stage_end_time = stage->getStageTime();
    const auto dt = (stage_end_time - stage_start_time) / (_count + 1);
    for (auto i = 0; i < _count; ++i)
    {
      const auto t = stage_start_time + (i + 1) * dt;
      result.push_back(t);
    }
  }

  // return the resulting vector
  return result;
}
