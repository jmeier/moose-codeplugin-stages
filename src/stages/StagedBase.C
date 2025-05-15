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
  // std::cout << "StagedBase::StagedBase: _type = " << typeid(this).name() << "; name = " << _name
  // << "; parent = " << _stage->getName() << "\n";
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
StagedBase::getTimesForTimeStepper(const Real stage_start_time)
{
  return {};
}

Real
StagedBase::parseTime(std::string s, bool allow_empty)
{
  if (allow_empty && isWhitespace(s))
    return std::nan("");

  // regex for parsing strings like "t-1.6E+12.23", "t + 0.1", or just "t"
  std::regex rx("^\\s*t\\s*(?:([+-])\\s*([+-]?[0-9]*[.]?[0-9]*\\s*[Ee]?\\s*[+-]?\\s*[0-9.]+))?\\s*$");
  std::smatch m;
  if (std::regex_match(s, m, rx))
  {
    // we've got a regex match - relative point in time given

    std::string g1 = m[1].str(); // plus or minus
    std::string g2 = m[2].str(); // offset as floating point number

    auto stage = getStage();
    auto stage_t = stage->getStageTime();

    if (g1 == "" && g2 == "")
    {
      // just "t" - we return stage_t
      return stage_t;
    }
    else
    {
      // we've got an offset; lets parse and apply it to stage_t
      Real res;
      std::stringstream sval(g2);
      sval >> res;

      if (g1 == "+")
      {
        return stage_t + res;
      }
      else
      {
        return stage_t - res;
      };
    };
  }
  else
  {
    // absolute point in time given
    Real res;
    std::stringstream sval(s);
    sval >> res;

    return res;
  }
}

std::vector<Real>
StagedBase::parseTimes(std::string s, std::string delimiter)
{
  if (isWhitespace(s))
    return {};

  // split the input string
  std::vector<std::string> items = split(s, delimiter);

  // debug-output...
  std::cout << "Splitting \"" << s << "\" with delimiter \"" << delimiter << "\" gives " << items.size() << " items: ";
  for (int kk = 0; kk < items.size(); ++kk)
  {
    if (kk > 0)
      std::cout << ";";
    std::cout << """" << items[kk] << """";
  }
  std::cout << "\n" << std::flush;


  // convert all items
  std::vector<Real> times;
  times.reserve(items.size());    // avoid unnecessary reallocations
  std::transform(items.begin(), items.end(),
                 std::back_inserter(times),
                 [this](std::string i) {return parseTime(i);} );

  return times;
}

bool
StagedBase::isWhitespace(std::string s)
{
  const auto n = s.length();
  for (long unsigned int index = 0; index < n; index++)
  {
    if (!std::isspace(s[index]))
      return false;
  }
  return true;
}

// split a string for string delimiter
std::vector<std::string>
StagedBase::split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

// split a string for char delimiter
std::vector<std::string>
StagedBase::split (const std::string &s, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline(ss, item, delim))
    {
        result.push_back(item);
    }

    return result;
}
