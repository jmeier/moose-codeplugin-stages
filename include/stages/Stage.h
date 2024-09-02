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

/**
 * Class for definition of a model stage
 */
class Stage : public GeneralUserObject
{
public:
  static InputParameters validParams();

  Stage(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void finalize() override;

  std::string getName();

  /**
   * Point in time this stage is associated with.
   */
  Real getStageTime();

  void addItem(GeneralUserObject & obj);

  std::vector<std::reference_wrapper<GeneralUserObject>> getItems();

private:
  const std::string _name;

  /// Point in time this stage is associated with
  const Real _stage_time;
  std::vector<std::reference_wrapper<GeneralUserObject>> _items;
};
