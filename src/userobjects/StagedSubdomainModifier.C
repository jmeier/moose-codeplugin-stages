//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "StagedSubdomainModifier.h"
#include "TimedElementSubdomainModifier.h"
#include "DelimitedFileReader.h"
#include "MooseMesh.h"
#include "Stages.h"
#include "Stage.h"
#include "StagedSubdomainModification.h"

registerMooseObject(MOOSEAPPNAME, StagedSubdomainModifier);

InputParameters
StagedSubdomainModifier::validParams()
{
  InputParameters params = TimedElementSubdomainModifier::validParams();

  params.addClassDescription(
      "Modify element subdomain ID of entire subdomains as defined by the [Stages] action system.");

  return params;
}

StagedSubdomainModifier::StagedSubdomainModifier(const InputParameters & parameters)
  : TimedElementSubdomainModifier(parameters)
{
  _init_from_stages = true;
}

void
StagedSubdomainModifier::buildFromStages()
{
  if (_init_from_stages == false)
    return;

  std::vector<Real> raw_times;
  std::vector<SubdomainID> raw_from;
  std::vector<SubdomainID> raw_to;

  // find the stages-object
  FEProblemBase * fe_problem = this->getParam<FEProblemBase *>("_fe_problem_base");
  if (fe_problem->hasUserObject("Stages") == false)
    mooseError("Stages user object not found.");
  Stages & stgs = fe_problem->getUserObject<Stages>("Stages");

  // iterate all stages
  std::vector<std::reference_wrapper<Stage>> vecStages = stgs.getStages();
  int n = vecStages.size();
  for (int i = 0; i < n; ++i)
  {
    Stage & stg = vecStages[i].get();

    // iterate all items of this stage
    auto items = stg.getItems();
    int m = items.size();
    for (int j = 0; j < m; ++j)
    {
      // we need only item of type StagedSubdomainModification
      GeneralUserObject & item = items[j].get();

      if (typeid(item) == typeid(StagedSubdomainModification))
      {
        StagedSubdomainModification & smd = dynamic_cast<StagedSubdomainModification &>(item);

        // collect all data of this subdomain modification
        Real sdTime = stg.getStageTime();
        std::vector<SubdomainName> sdFromNames = smd.SubdomainsFrom();
        std::vector<SubdomainName> sdToNames = smd.SubdomainsTo();

        // make SubdomainIDs from SubdomainNames
        std::vector<SubdomainID> sdFromIDs = _mesh.getSubdomainIDs(sdFromNames);
        std::vector<SubdomainID> sdToIDs = _mesh.getSubdomainIDs(sdToNames);

        // store the data
        int n_ids = sdFromIDs.size();
        for (int k = 0; k < n_ids; ++k)
        {
          raw_times.push_back(sdTime);
          raw_from.push_back(sdFromIDs[k]);
          raw_to.push_back(sdToIDs[k]);
        }
      };
    };
  };

  _times = raw_times;
  _blocks_from = raw_from;
  _blocks_to = raw_to;
  _init_from_stages = false;
}

SubdomainID
StagedSubdomainModifier::computeSubdomainID()
{
  // do we have to build from stages
  if (_init_from_stages)
    buildFromStages();

  // get the subdomain-id of the current element
  SubdomainID resulting_subdomain_id = _current_elem->subdomain_id();

  // check for all the subdomain changes that can have been requested between the previous and the
  // current time
  for (const auto & time_pair : _times_and_indices)
  {
    // time of the data point
    const auto t = time_pair.time;

    // original data point index
    const auto j = time_pair.index;

    // do we have to apply?
    if (t > _t_old && t <= _t && resulting_subdomain_id == _blocks_from[j])
    {
      // we have to change the subdomain-id using the original index (stored in 'j')
      resulting_subdomain_id = _blocks_to[j];
    }
  }

  return resulting_subdomain_id;
}
