//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

// Moose Includes
#include "StagedBase.h"
#include "StagedEnableDisable.h"

/**
 * This class is used to enable controllable entities at a given point in time.
 */
class StagedEnable : public StagedEnableDisable
{
public:
  static InputParameters validParams();

  StagedEnable(const InputParameters & parameters);

};
