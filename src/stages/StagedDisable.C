//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "Stage.h"
#include "StagedEnableDisable.h"
#include "StagedDisable.h"
#include "Times.h"
#include "InputTimes.h"
#include "TimesEnableControl.h"
#include <math.h>

registerMooseObject(MOOSEAPPNAME, StagedDisable);

InputParameters
StagedDisable::validParams()
{
  return StagedEnableDisable::validParams(/* enable */ false);
}

StagedDisable::StagedDisable(const InputParameters & parameters)
  : StagedEnableDisable(parameters)
{
  emit(/* doEnable */ false);
}
