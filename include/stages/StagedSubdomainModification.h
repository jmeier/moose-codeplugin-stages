//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

// Moose Includes
#include "StagedBase.h"

/**
 * This class is used to define the transition of all elements of a given subdomain to another at a
 * given point in time.
 */
class StagedSubdomainModification : public StagedBase
{
public:
  static InputParameters validParams();

  StagedSubdomainModification(const InputParameters & parameters);

  std::vector<SubdomainName> SubdomainsFrom();
  std::vector<SubdomainName> SubdomainsTo();

private:
  const std::vector<SubdomainName> _subdomains_from;
  const std::vector<SubdomainName> _subdomains_to;
};
