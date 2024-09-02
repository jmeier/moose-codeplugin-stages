//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AddStageAction.h"
#include "FEProblem.h"
#include "Stage.h"
#include "Stages.h"
#include <iostream>
#include <string>
#include <vector>

registerMooseAction(MOOSEAPPNAME, AddStageAction, "add_stage");

InputParameters
AddStageAction::validParams()
{
  InputParameters params = MooseObjectAction::validParams();

  params.addParam<std::string>(
      "type",
      "Stage",
      "A string representing the Moose Object that will be built by this Action");

  params.addClassDescription("Add a Stage object to the simulation.");
  return params;
}

AddStageAction::AddStageAction(const InputParameters & params) : MooseObjectAction(params)
{
}

void
AddStageAction::act()
{
  std::cout << "AddStageAction.act: _type = " << _type << "; " << "_name = " << _name << "\n";

  // find the stages-object
  if (_problem->hasUserObject("Stages") == false)
    mooseError("Stages user object not found.");
  Stages & stgs = _problem->getUserObject<Stages>("Stages");

  // create the stage user object and also add it to the stages object
  _moose_object_pars.set<std::string>("_name") = _name;
  _problem->addUserObject(_type, _name, _moose_object_pars);
  Stage & stg = _problem->getUserObject<Stage>(_name);
  stgs.addStage(stg);
}
