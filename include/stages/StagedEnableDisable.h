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

/**
 * This base class is used to enable or disable controllable entities at a given point in time.
 */
class StagedEnableDisable : public StagedBase
{
public:
  static InputParameters validParams(const bool doEnable);

  StagedEnableDisable(const InputParameters & parameters);

  static void emit(const bool doEnable,
                   const std::vector<std::string> objects,
                   Stage & stage,
                   Factory & factory,
                   FEProblemBase & problem,
                   MooseApp & app,
                   const std::string suffix = "");

protected:
  const FEProblemBase & _fe_problem;
  const SubProblem & _subproblem;
  const SystemBase & _sys;

  // the parenting stage
  Stage & _stage;

  // vector of objects to enable/disable
  const std::vector<std::string> _objects;

  void emit(const bool doEnable);
};
