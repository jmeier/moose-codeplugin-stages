//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "StagedTimeSequenceStepper.h"
#include "FEProblem.h"
#include "Stage.h"
#include "Stages.h"
#include "StagedBase.h"

registerMooseObject(MOOSEAPPNAME, StagedTimeSequenceStepper);

InputParameters
StagedTimeSequenceStepper::validParams()
{
  InputParameters params = TimeSequenceStepperBase::validParams();
  params.addClassDescription("Solves the Transient problem at a sequence of time points given by a 'Staging' block tree.");
  return params;
}

StagedTimeSequenceStepper::StagedTimeSequenceStepper(const InputParameters & parameters)
  : TimeSequenceStepperBase(parameters)
{
}

void
StagedTimeSequenceStepper::init()
{
  std::vector<Real> time_sequence;

  // find the stages-object
  Stages & stgs = _fe_problem.getUserObject<Stages>("Stages");

  // iterate all stages
  std::vector<std::reference_wrapper<Stage>> vecStages = stgs.getStages();
  int n = vecStages.size();
  for (int i = 0; i < n; ++i)
  {
    Stage & stg = vecStages[i].get();
    time_sequence.push_back(stg.getStageTime());

    // iterate all items of this stage
    auto items = stg.getItems();
    int m = items.size();
    for (int j = 0; j < m; ++j)
    {
      // we need only item of type StagedBase (they all should be of this type)
      GeneralUserObject & item = items[j].get();

      if (dynamic_cast<StagedBase*>(&item))
      {
        StagedBase & sb_item = dynamic_cast<StagedBase &>(item);

        // ask the sb_item for points in time to add
        std::vector<Real> sb_item_times = sb_item.getTimesForTimeStepper();

        // append to time_sequence
        time_sequence.insert(std::end(time_sequence), std::begin(sb_item_times), std::end(sb_item_times));
      };

    };

  };

  // sort times and de-duplicate
  sort(time_sequence.begin(), time_sequence.end());
  time_sequence.erase(unique(time_sequence.begin(), time_sequence.end() ), time_sequence.end());

  // remove all times smaller or equal to zero
  time_sequence.erase(std::remove_if(
      time_sequence.begin(), time_sequence.end(),
      [](const Real & x) {
          return x <= 0;
      }), time_sequence.end());

  // output
  // int count_items = time_sequence.size();
  // std::cout << "StagedTimeSequenceStepper (" << count_items << " items): ";
  // for (int i=0; i<count_items; ++i)
  // {
  //   if (i>0)
  //     std::cout << "; ";
  //   std::cout << time_sequence[i];
  // }
  // std::cout << "\n" << std::flush;

  // publish the vector of times
  setupSequence(time_sequence);
}
