//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "TimeSequenceStepperBase.h"

/**
 * Solves the PDEs at a sequence of time points given by a tree of Stages-blocks
 * in the input file.
 * Adjusts the time sequence vector according to Transient start_time and end_time.
 */
class StagedTimeSequenceStepper : public TimeSequenceStepperBase
{
public:
  static InputParameters validParams();

  StagedTimeSequenceStepper(const InputParameters & parameters);

  virtual void init() override;
};
