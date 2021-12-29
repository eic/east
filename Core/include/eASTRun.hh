// ********************************************************************
//
// eASTRun.hh
//   Header file of Run class
//
// History
//   September 8th, 2020 : first implementation
//
// ********************************************************************

#ifndef eASTRun_h
#define eASTRun_h 1

#include "G4Run.hh"
#include "G4Event.hh"

#include "G4THitsMap.hh"
#include <vector>

class eASTRunAction;

class eASTRun : public G4Run {

public:
  eASTRun(eASTRunAction*);
  virtual ~eASTRun();

public:
  virtual void RecordEvent(const G4Event*);
  virtual void Merge(const G4Run*);

private:
  eASTRunAction* pRA;
};

#endif
