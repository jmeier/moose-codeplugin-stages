//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AddStageItemAction.h"
#include "FEProblem.h"
#include "Stage.h"
#include "Stages.h"
#include "StagedBase.h"
#include <iostream>
#include <string>
#include <vector>

registerMooseAction(MOOSEAPPNAME, AddStageItemAction, "add_stage");

InputParameters
AddStageItemAction::validParams()
{
  InputParameters params = MooseObjectAction::validParams();
  params.addClassDescription("Add a Stage object to the simulation.");
  return params;
}

AddStageItemAction::AddStageItemAction(const InputParameters & params) : MooseObjectAction(params)
{
  // std::cout << "AddStageItemAction.AddStageItemAction: _type = " << _type << "; " << "_name = " << _name << "\n";
}

void
AddStageItemAction::act()
{
  // std::cout << "AddStageItemAction.act: _type = " << _type << "; " << "_name = " << _name << "\n";

  // Find the parental stage object
  auto & aw = _app.actionWarehouse();
  std::vector<std::string> actionNameParts = split(aw.getCurrentActionName(), "/");
  if (actionNameParts.size() != 3)
    mooseError("Unsupported user object structure: " + aw.getCurrentActionName());
  std::string stageName = actionNameParts[1];
  if (_problem->hasUserObject(stageName) == false)
    mooseError("Stage user object """ + stageName + """ not found.");
  Stage & stg = _problem->getUserObject<Stage>(stageName);

  // create the user object requested and add to the parental stage
  _moose_object_pars.set<Stage *>("_stage") = &stg;
  _moose_object_pars.set<std::string>("_name") = _name;
  _problem->addUserObject(_type, _name, _moose_object_pars);
  StagedBase & obj = _problem->getUserObject<StagedBase>(_name);
  obj.setup(_problem);
  stg.addItem(obj);

  //  // find the stages-object
  //  if (_problem->hasUserObject("Stages") == false)
  //    mooseError("Stages user object not found.");
  //  Stages & stgs = _problem->getUserObject<Stages>("Stages");
  //
  //  // create the stage user object and also add it to the stages object
  //  _problem->addUserObject(_type, _name, _moose_object_pars);
  //  Stage & stg = _problem->getUserObject<Stage>(_name);
  //  stgs.addStage(stg);
  //
  //  auto & aw = _app.actionWarehouse();
  //  std::vector<std::string> actionNameParts = split(aw.getCurrentActionName(), "/");
  //  // std::cout << "AddStageItemAction.act: _aw.getCurrentActionName = " << aw.getCurrentActionName() << "\n";
  //  // std::cout << "AddStageItemAction.act: _aw.getCurrentActionName (" << actionNameParts.size() <<  " items) = ";
  //  // for (int i = 0; i < actionNameParts.size(); ++i)
  //  // {
  //  //   std::cout << actionNameParts[i] << " / ";
  //  // };
  //  // std::cout << "\n";
  //
  //  // auto & s = aw.syntax();
  //  // auto & a = aw.getCurrentAction();
  //  // std::cout << "AddStageItemAction.act: stage.stage_time = " << stg.getStageTime() << "\n";
}

std::vector<std::string>
AddStageItemAction::split(std::string target, std::string delimiter)
{
  std::vector<std::string> components;
  if (!target.empty())
  {
    size_t start = 0;
    do
    {
      const size_t index = target.find(delimiter, start);
      if (index == std::string::npos)
      {
        break;
      }
      const size_t length = index - start;
      components.push_back(target.substr(start, length));
      start += (length + delimiter.size());
    } while (true);
    components.push_back(target.substr(start));
  }
  return components;
}
