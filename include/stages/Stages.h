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
#include "GeneralUserObject.h"
#include "Stage.h"

/**
 * Base class for deriving Stages
 */
class Stages : public GeneralUserObject
{
public:
  static InputParameters validParams();

  virtual void initialize() override;
  virtual void execute() override;
  virtual void finalize() override;

  Stages(const InputParameters & parameters);

  /**
   * Adds a stage to the vector of stages.
   */
  void addStage(Stage & stg);

  /**
   * Returns a sorted vector of stages. The stages are sorted ascending by time.
   */
  std::vector<std::reference_wrapper<Stage>> getStages();

private:
  std::vector<std::reference_wrapper<Stage>> _stages;

};
