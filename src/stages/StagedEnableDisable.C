//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "Stage.h"
#include "StagedEnableDisable.h"
#include "Times.h"
#include "InputTimes.h"
#include "TimedEnableControl.h"
#include <math.h>
#include "Moose.h"

StagedEnableDisable::StagedEnableDisable(const InputParameters & parameters)
  : StagedBase(parameters),
    _fe_problem(*getCheckedPointerParam<FEProblemBase *>(
        "_fe_problem_base", "This might happen if you don't have a mesh")),
    _subproblem(*getCheckedPointerParam<SubProblem *>("_subproblem")),
    _sys(*getCheckedPointerParam<SystemBase *>("_sys")),
    _stage(*getCheckedPointerParam<Stage *>("_stage")),
    _objects(getParam<std::vector<std::string>>("objects"))
{
}

InputParameters
StagedEnableDisable::validParams(const bool doEnable)
{
  InputParameters params = StagedBase::validParams();
  // params.registerBase("StagedEnableDisable");

  std::string s1 = (doEnable) ? "enable" : "disable";
  params.addRequiredParam<std::vector<std::string>>("objects",
                                                    "A list of object tags to " + s1 + ".");

  std::string s2 = (doEnable) ? "enables" : "disables";
  params.addClassDescription("User object that " + s2 + " an set of objects at a given time.");

  return params;
}

void
StagedEnableDisable::emit(const bool doEnable)
{
  const std::string suffix = name();
  FEProblemBase & problem = const_cast<FEProblemBase &>(_fe_problem);
  emit(doEnable, _objects, _stage, _factory, problem, _app, suffix);
}

void
StagedEnableDisable::emit(const bool doEnable,
                          const std::vector<std::string> objects,
                          Stage & stage,
                          Factory & factory,
                          FEProblemBase & problem,
                          MooseApp & app,
                          const std::string suffix)
{
  // // get a vector of times just having one item: the time of the stage
  // std::vector<Real> t = {0, _stage.getStageTime()};
  //
  // // params for the InputTimes
  // InputParameters paramsInputTimes = InputTimes::validParams();
  // paramsInputTimes.addPrivateParam("_moose_app", &_app);
  // paramsInputTimes.addPrivateParam("_subproblem", &_subproblem);
  // paramsInputTimes.addPrivateParam("_sys", &_sys);
  // paramsInputTimes.setParameters("times", t);
  //
  // // add InputTimes reporter
  // auto nameTimes = "InputTimes" + suffix;
  // _fe_problem.addReporter("InputTimes", nameTimes, paramsInputTimes);
  //
  // // params for the TimesEnableControl
  // InputParameters paramsTimesEnableControl = TimesEnableControl::validParams();
  // paramsTimesEnableControl.addPrivateParam("_moose_app", &_app);
  // paramsTimesEnableControl.setParameters<UserObjectName>("times", nameTimes);
  // paramsTimesEnableControl.setParameters<bool>("act_on_time_stepping_across_a_time_point", true);
  // paramsTimesEnableControl.set<ExecFlagEnum>("execute_on", true) = {EXEC_INITIAL,
  // EXEC_TIMESTEP_BEGIN}; paramsTimesEnableControl.set<Real>("time_window", 0.0);
  // paramsTimesEnableControl.set<bool>("reverse_on_false", false);
  // if (doEnable)
  //   paramsTimesEnableControl.setParameters("enable_objects", _objects);
  // else
  //   paramsTimesEnableControl.setParameters("disable_objects", _objects);
  //
  // // add TimesEnableControl
  // std::shared_ptr<Control> control = _factory.create<Control>("TimesEnableControl", nameControl,
  // paramsTimesEnableControl); _fe_problem.getControlWarehouse().addObject(control);

  // we need a unique suffix no avoid name collisions
  const std::string stageName = stage.getName();
  const std::string nameTimedEnableControl =
      "TimedEnableControl_" + stageName + ((suffix.empty()) ? "" : ("_" + suffix));

  // params for the TimedEnableControl
  InputParameters paramsTimedEnableControl = TimedEnableControl::validParams();
  paramsTimedEnableControl.addPrivateParam("_moose_app", &app);
  // paramsTimedEnableControl.addPrivateParam("_subproblem", &_subproblem);
  // paramsTimedEnableControl.addPrivateParam("_sys", &_sys);
  paramsTimedEnableControl.setParameters<Real>("time", stage.getStageTime());
  paramsTimedEnableControl.set<ExecFlagEnum>("execute_on", true) = {EXEC_INITIAL,
                                                                    EXEC_TIMESTEP_BEGIN};
  if (doEnable)
    paramsTimedEnableControl.setParameters("enable_objects", objects);
  else
    paramsTimedEnableControl.setParameters("disable_objects", objects);

  // add TimedEnableControl
  std::shared_ptr<Control> ctrl = factory.create<Control>(
      "TimedEnableControl", nameTimedEnableControl, paramsTimedEnableControl);
  problem.getControlWarehouse().addObject(ctrl);
}
