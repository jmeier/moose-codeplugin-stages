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
  enum class DeltaTimeAlignType
  {
    START = 0,
    END = 1
  };

  static InputParameters validParams();

  StagedAdditionalTimeStep(const InputParameters & parameters);

  std::vector<Real> getAdditionalTimes();
  int getAdditionalCount();

  std::vector<Real> getTimesForTimeStepper(const Real stage_start_time) override;

private:
  const std::vector<Real> _times;

  const int _count;

  const Real _delta_time;
  const DeltaTimeAlignType _delta_time_align;
};
