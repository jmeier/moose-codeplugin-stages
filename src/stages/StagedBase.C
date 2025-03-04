//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "StagedBase.h"
#include <regex>
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

Real
StagedBase::parseTime(std::string s)
{
  // regex for parsing strings like "t-1.6E+12.23", "t + 0.1", or just "t"
  std::regex rx("^\\s*t\\s*(?:([+-])\\s*([+-]?[0-9]*[.]?[0-9]*\\s*[Ee]?\\s*[+-]?\\s*[0-9.]+))?\\s*$");
  std::smatch m;
  if (std::regex_match(s, m, rx))
  {
    // we've got a regex match - relative point in time given

    std::string g1 = m[1].str();  // plus or minus
    std::string g2 = m[2].str();  // offset as floating point number

    auto stage = getStage();
    auto stage_t = stage->getStageTime();

    if (g1 == "" && g2 == "")
    {
      // just "t" - we return stage_t
      return stage_t;
    } else {
      // we've got an offset; lets parse and apply it to stage_t
      Real res;
      std::stringstream sval(g2);
      sval >> res;

      if (g1 == "+")
      {
        return stage_t + res;
      } else {
        return stage_t - res;
      };
    };
  } else {
    // absolute point in time given
    Real res;
    std::stringstream sval(s);
    sval >> res;

    return res;
  }
}
