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
  params.addRequiredParam<std::string>(
      "time", "Time of the time step. Multiple times may be specified using ';' as a delimiter.");
  params.addClassDescription("User object that holds additional time steps.");
  return params;
}

StagedAdditionalTimeStep::StagedAdditionalTimeStep(const InputParameters & parameters)
  : StagedBase(parameters), _times(parseTimes(getParam<std::string>("time"), ";"))
{
}

std::vector<Real>
StagedAdditionalTimeStep::getTimes()
{
  return _times;
}

std::vector<Real>
StagedAdditionalTimeStep::getTimesForTimeStepper()
{
  return _times;
}
