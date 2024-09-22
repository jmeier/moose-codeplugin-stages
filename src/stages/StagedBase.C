//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "StagedBase.h"
#include "Stage.h"

registerMooseObject(MOOSEAPPNAME, StagedBase);

InputParameters
StagedBase::validParams()
{
  InputParameters params = GeneralUserObject::validParams();
  // params.declareControllable("enable"); // allows Control to enable/disable this type of object
  params.registerBase("StagedBase");
  params.addPrivateParam<std::string>("_name", "");
  params.addPrivateParam<Stage *>("_stage", nullptr);
  return params;
}

StagedBase::StagedBase(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _name(getParam<std::string>("_name")),
  _stage(getParam<Stage *>("_stage"))
{
  // std::cout << "StagedBase::StagedBase: _type = " << typeid(this).name() << "; name = " << _name  << "; parent = " << _stage->getName() << "\n";
}

std::string
StagedBase::getName()
{
  return _name;
}

Stage *
StagedBase::getStage()
{
  return _stage;
}

void
StagedBase::initialize()
{
}

void
StagedBase::execute()
{
}

void
StagedBase::finalize()
{
}

void
StagedBase::setup(std::shared_ptr<FEProblemBase>)
{
}

std::vector<Real>
StagedBase::getTimesForTimeStepper()
{
  return {};
}
