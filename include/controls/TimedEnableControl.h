//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ConditionalEnableControl.h"

class Times;

/**
 * Control for enabling/disabling objects when near or past times from a Times object
 */
class TimedEnableControl : public ConditionalEnableControl
{
public:
  static InputParameters validParams();

  TimedEnableControl(const InputParameters & parameters);

protected:
  virtual bool conditionMet(const unsigned int & i) override;

private:
  const Real & _time;

  Real _time_prev;
  Real _time_curr;
};
