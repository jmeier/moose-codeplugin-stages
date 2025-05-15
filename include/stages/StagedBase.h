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
#include "GeneralUserObject.h"
#include "Stage.h"

/**
 * Base class for deriving staged objects
 */
class StagedBase : public GeneralUserObject
{
public:
  static InputParameters validParams();

  StagedBase(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void finalize() override;

  virtual void setup(std::shared_ptr<FEProblemBase> p);

  std::string getName();
  Stage * getStage();

  virtual std::vector<Real> getTimesForTimeStepper(const Real stage_start_time);

protected:
  Real parseTime(std::string s, bool allow_empty = false);
  std::vector<Real> parseTimes(std::string s, std::string delimiter);

  // String split
  std::vector<std::string> split(std::string s, std::string delimiter);
  std::vector<std::string> split(const std::string &s, char delim);

private:
  const std::string _name;

  // Pointer to the owning stage
  Stage * const _stage;

  bool isWhitespace(std::string s);
};

