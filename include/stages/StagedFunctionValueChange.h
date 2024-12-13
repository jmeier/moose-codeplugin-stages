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
 * This class is used to define a value change in a StagedFunction at a given point in time.
 */
class StagedFunctionValueChange : public StagedBase
{
public:
  enum class StepFunctionType
  {
    LINEAR = 0,
    SMOOTH = 1,
    PERLIN = 2
  };

  static InputParameters validParams();

  StagedFunctionValueChange(const InputParameters & parameters);

  virtual void setup(std::shared_ptr<FEProblemBase> p) override;

  std::vector<std::string> getFunctionNames();
  int getIndexOfFunction(std::string function_name);

  StepFunctionType getStepFunctionType();
  std::vector<double> getNewValues();
  double getNewValue(const int funcIndex);
  Real getStartTime();
  Real getEndTime();

  double getValue(const int funcIndex, const Real t, const double old_value);
  double getTimeDerivative(const int funcIndex, const Real t, const double old_value);

  std::vector<Real> getTimesForTimeStepper() override;

private:
  const std::vector<std::string> _function_names;
  const StepFunctionType _step_function_type;
  const std::vector<double> _new_values;
  const Real _start_time;
  const Real _end_time;

  const bool _register_start_time;
  const bool _register_end_time;

};
