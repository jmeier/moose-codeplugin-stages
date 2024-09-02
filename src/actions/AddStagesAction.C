//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AddStagesAction.h"
#include "FEProblem.h"

registerMooseAction(MOOSEAPPNAME, AddStagesAction, "add_stages");

InputParameters
AddStagesAction::validParams()
{
  InputParameters params = MooseObjectAction::validParams();

  // Here we are setting the default type to "Stages".
  params.addParam<std::string>(
      "type", "Stages", "A string representing the Moose Object that will be built by this Action");

  params.addClassDescription("Add a Stages object to the simulation.");
  return params;
}

AddStagesAction::AddStagesAction(const InputParameters & params) : MooseObjectAction(params)
{
  std::cout << "AddStagesAction.AddStagesAction: _type = " << _type << "; " << "_name = " << _name
            << "\n";
}

void
AddStagesAction::act()
{
  std::cout << "AddStagesAction.act: _type = " << _type << "; " << "_name = " << _name << "\n"
            << std::flush;

  if (_problem->hasUserObject("Stages") == false)
  {
    std::cout << "AddStagesAction.act: no user object of name "
                 "Stages"
                 "\n"
              << std::flush;
    _problem->addUserObject(/*type=*/"Stages", /*name=*/"Stages", _moose_object_pars);
  };
}
