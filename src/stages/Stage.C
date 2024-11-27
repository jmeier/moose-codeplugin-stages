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

registerMooseObject(MOOSEAPPNAME, Stage);

InputParameters
Stage::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  // params.declareControllable("enable"); // allows Control to enable/disable this type of object
  // params.registerBase("Stage");
  params.addPrivateParam<std::string>("_name", "");
  params.addRequiredParam<Real>("t", "Point in time this stage is associated with.");

  params.addParam<std::vector<std::string>>("enable_objects", "A list of object tags to enable.");
  params.addParam<std::vector<std::string>>("disable_objects", "A list of object tags to disable.");
  params.addParamNamesToGroup("enable_objects disable_objects", "Enabling and disabling objects");

  params.addClassDescription("User object that holds a stage.");
  return params;
}

Stage::Stage(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _name(getParam<std::string>("_name")),
    _stage_time(getParam<Real>("t"))
{
  auto objects_enable = parameters.get<std::vector<std::string>>("enable_objects");
  if (objects_enable.size() > 0)
  {
    const std::string suffix = "Enable";
    FEProblemBase & problem = const_cast<FEProblemBase &>(_fe_problem);
    StagedEnableDisable::emit(true, objects_enable, *this, _factory, problem, _app, suffix);
  }

  auto objects_disable = parameters.get<std::vector<std::string>>("disable_objects");
  if (objects_disable.size() > 0)
  {
    const std::string suffix = "Disable";
    FEProblemBase & problem = const_cast<FEProblemBase &>(_fe_problem);
    StagedEnableDisable::emit(false, objects_disable, *this, _factory, problem, _app, suffix);
  }

  // std::cout << "Stage::Stage" << "\n";
}

std::string
Stage::getName()
{
  return _name;
}

Real
Stage::getStageTime()
{
  return _stage_time;
}

void
Stage::addItem(GeneralUserObject & obj)
{
  _items.push_back(obj);
}

std::vector<std::reference_wrapper<GeneralUserObject>>
Stage::getItems()
{
  return _items;
}

void
Stage::initialize()
{
}

void
Stage::execute()
{
}

void
Stage::finalize()
{
}
