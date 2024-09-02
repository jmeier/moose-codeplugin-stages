//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "Stages.h"
#include "Stage.h"

registerMooseObject(MOOSEAPPNAME, Stages);

InputParameters
Stages::validParams()
{
  std::cout << "Stages::validParams (start)" << "\n";
  InputParameters params = GeneralUserObject::validParams();
  // params.declareControllable("enable"); // allows Control to enable/disable this type of object
  // params.registerBase("Stages");
  params.addClassDescription("User object that holds all stages.");
  std::cout << "Stages::validParams (done)" << "\n";
  return params;
}

Stages::Stages(const InputParameters & parameters)
  : GeneralUserObject(parameters)
{
  std::cout << "Stages::Stages" << "\n";
}

void
Stages::initialize()
{
  std::cout << "Stages::initialize (start)" << "\n";
}

void
Stages::execute()
{
  std::cout << "Stages::execute (start)" << "\n";
}

void
Stages::finalize()
{
  std::cout << "Stages::finalize (start)" << "\n";
}

void
Stages::addStage(Stage & stg)
{
  std::cout << "Stages::addStage (start)" << "\n";

  // There can only be one stage for a given point in time
  // test, if the time of 'stg' is already defined.
  auto stg_t = stg.getStageTime();
  int n = _stages.size();
  for (int i = 0; i < n; ++i)
  {
    auto i_t = _stages[i].get().getStageTime();
    if (i_t == stg_t)
      mooseError("There are two or more stage definitions for t = " + std::to_string(stg_t) + ".");
  };

  // add the stage to the internal vector of stages
  _stages.push_back(stg);

  // sort the internal vector of stages by time.
  std::sort(_stages.begin(), _stages.end(), [](const std::reference_wrapper<Stage> &a, const std::reference_wrapper<Stage> &b)
    {
      return a.get().getStageTime() < b.get().getStageTime();
    });
}

std::vector<std::reference_wrapper<Stage>>
Stages::getStages()
{
  std::cout << "Stages::getStages (start)" << "\n";
  return _stages;
}


// void
// Stages::registerMooseObjectTasks()
// {
//   /* Stages Block System */
//   registerMooseObjectTask("add_stages",                   UserObject, /*is_required=*/false);
//   registerMooseObjectTask("add_stage",                    UserObject, /*is_required=*/false);
//   registerMooseObjectTask("add_stage_item",               StagedBase, /*is_required=*/false);
// }
//
//
// #include "Syntax.h"
//
// void
// Stages::addDependencySets(Syntax & syntax)
// {
//  // clang-format off
//   syntax.addDependencySets("(add_stages)"
//                            "(add_stage)"
//                            "(add_stage_item)");
// }
//
//
// void
// Stages::registerSyntaxTasks()
// {
//   /* Stages Block System */
//   registerSyntaxTask("AddStagesAction", "Stages", "add_stages");
//   registerSyntaxTask("AddStageAction", "Stages/*", "add_stage");
//   registerSyntaxTask("AddStageItemAction", "Stages/*/*", "add_stage_item");
// }



