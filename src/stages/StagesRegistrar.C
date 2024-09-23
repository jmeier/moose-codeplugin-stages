#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"
#include "StagesRegistrar.h"

void
StagesRegistrar::registerAll(Syntax & syntax)
{
  /* register custom execute flags, action syntax, etc. here */

  /* Stages Block System: Moose Object Tasks */
  registerMooseObjectTask("add_stages",     UserObject, /*is_required=*/false);
  registerMooseObjectTask("add_stage",      UserObject, /*is_required=*/false);
  registerMooseObjectTask("add_stage_item", StagedBase, /*is_required=*/false);

  /* Stages Block System: Dependencies */
  syntax.addDependencySets("(add_stages)"
                           "(add_stage)"
                           "(add_stage_item)");

  /* Stages Block System: Syntax */
  registerSyntaxTask("AddStagesAction",    "Stages",     "add_stages");
  registerSyntaxTask("AddStageAction",     "Stages/*",   "add_stage");
  registerSyntaxTask("AddStageItemAction", "Stages/*/*", "add_stage_item");

}
