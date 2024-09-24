//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "StagedSubdomainModification.h"

registerMooseObject(MOOSEAPPNAME, StagedSubdomainModification);

InputParameters
StagedSubdomainModification::validParams()
{
  InputParameters params = StagedBase::validParams();
  // params.declareControllable("enable"); // allows Control to enable/disable this type of object
  // params.registerBase("StagedSubdomainModification");
  params.addParam<std::vector<SubdomainName>>("from", "Vector of subdomains to move all elements from.");
  params.addParam<std::vector<SubdomainName>>("to", "Vector of subdomains to move all elements to.");
  params.addClassDescription("User object that bla bla bla.");
  return params;
}

StagedSubdomainModification::StagedSubdomainModification(const InputParameters & parameters)
  : StagedBase(parameters),
    _subdomains_from(getParam<std::vector<SubdomainName>>("from")),
    _subdomains_to(getParam<std::vector<SubdomainName>>("to"))
{
  // std::cout << "StagedSubdomainModification::StagedSubdomainModification" << "\n";

  auto const n_subdomains_from = _subdomains_from.size();
  auto const n_subdomains_to = _subdomains_to.size();
  if (n_subdomains_from != n_subdomains_to)
    mooseError("Parameters ""from"" and ""to"" must have the same number of items." );
}

// void
// StagedSubdomainModification::setup(std::shared_ptr<FEProblemBase> p)
// {
//   // ToDo: make sure there is one user object of type StagedSubdomainModifier with from_stages=true
//   // p->hasOserObject(...)
// }

std::vector<SubdomainName>
StagedSubdomainModification::SubdomainsFrom()
{
  return _subdomains_from;
}

std::vector<SubdomainName>
StagedSubdomainModification::SubdomainsTo()
{
  return _subdomains_to;
}
