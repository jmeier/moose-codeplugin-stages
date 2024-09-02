//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

// MOOSE includes
#include "Function.h"
#include "StagedFunctionValueChange.h"

/**
 * This class is used to define a function using objects of type StagedFunctionValueChange in the
 * stage tree.
 */
class StagedFunction : public Function
{
public:
  /**
   * Created from MooseSystem via the FunctionFactory.
   * @param parameters The input parameters
   */
  static InputParameters validParams();

  StagedFunction(const InputParameters & parameters);

  using Function::value;
  /**
   * Evaluate the equation at the given location. For 1-D and 2-D equations
   * x and y are optional.
   * @param t The evaluation time
   * @param pt The current point (x,y,z)
   * @return The result of evaluating the function
   */
  virtual Real value(Real t, const Point & pt) const override;

  using Function::gradient;
  /**
   * Evaluate the gradient of the function. This is computed in libMesh
   * through automatic symbolic differentiation.
   */
  virtual RealGradient gradient(Real t, const Point & p) const override;

  /**
   * Evaluate the time derivative of the function. This is computed in libMesh
   * through automatic symbolic differentiation.
   * \param t The time
   * \param p The point in space (x,y,z)
   * \return The time derivative of the function at the specified time and location
   */
  virtual Real timeDerivative(Real t, const Point & p) const override;

  /**
   * Method invalid for ParsedGradFunction
   * @see ParsedVectorFunction
   */
  virtual RealVectorValue vectorValue(Real t, const Point & p) const override;

  /**
   * Creates the parsed function.
   */
  virtual void initialSetup() override;

protected:
  /// The function defined by the user
  std::string _value;

  friend class ParsedFunctionTest;

private:
  mutable double _last_value_t = NAN;
  mutable double _last_value;

  mutable double _last_timeDerivative_t = NAN;
  mutable double _last_timeDerivative;

  double getValueInternal(Real t, bool timeDerivative) const;
};
