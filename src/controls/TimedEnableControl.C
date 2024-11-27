//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TimedEnableControl.h"

registerMooseObject(MOOSEAPPNAME, TimedEnableControl);

InputParameters
TimedEnableControl::validParams()
{
  InputParameters params = ConditionalEnableControl::validParams();
  params.setParameters<bool>("reverse_on_false", false);
  params.suppressParameter<bool>("reverse_on_false");

  params.addRequiredParam<Real>(
      "time", "The time to turn on/off the objects.");

  params.addClassDescription(
      "Control for enabling/disabling objects when a certain time is reached.");


  return params;
}

TimedEnableControl::TimedEnableControl(const InputParameters & parameters)
  : ConditionalEnableControl(parameters),
    _time(getParam<Real>("time")),
    _time_prev(declareRestartableData<Real>("prev_time", _fe_problem.time())),
    _time_curr(_fe_problem.time())
{
}

bool
TimedEnableControl::conditionMet(const unsigned int & /*i*/)
{

  // std::cout << "TimedEnableControl """ << name() << """: _time = " << _time << "; _t = " << _t << "; _time_curr = " << _time_curr << "; _time_prev = " << _time_prev << "\n";

  // Update _time_curr and _time_prev only if we changed time step
  // since the last time conditionMet was called
  if (_t != _time_curr)
  {
    _time_prev = _time_curr;
    _time_curr = _t;
  }

  bool b = false;
  if (_time > _time_prev && _time <= _time_curr)
    b = true;

  // std::cout << "     _time_curr = " << _time_curr << "; _time_prev = " << _time_prev << "; b = " << b << "; _reverse_on_false = " << _reverse_on_false << "\n";

  return b;
}
