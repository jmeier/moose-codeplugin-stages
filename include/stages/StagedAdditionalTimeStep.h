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
 * This class is used to define an additional timestep.
 */
class StagedAdditionalTimeStep : public StagedBase
{
public:
  static InputParameters validParams();

  StagedAdditionalTimeStep(const InputParameters & parameters);

  Real getTime();

  std::vector<Real> getTimesForTimeStepper() override;

private:
  const Real _time;

};
