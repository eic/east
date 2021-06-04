// ********************************************************************
//
// eASTUserActionDispatcher.cc
//   store component-specific user actions
//   Please note that this class does not handle the following actions
//     - G4UserPrimaryGeneratorAction
//     - G4UserSteppingAction
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#include "eASTUserActionDispatcher.hh"
#include "G4UserRunAction.hh"
#include "G4UserEventAction.hh"
#include "G4UserStackingAction.hh"
#include "G4UserTrackingAction.hh"

G4ThreadLocal eASTUserActionDispatcher* eASTUserActionDispatcher::instance = nullptr;
eASTUserActionDispatcher* eASTUserActionDispatcher::Instance()
{
  if(instance==nullptr)
  { instance = new eASTUserActionDispatcher; }
  return instance;
}

eASTUserActionDispatcher::~eASTUserActionDispatcher()
{
  for(auto ua : *fpRunActions)
  { delete ua.second; }
  fpRunActions->clear();
  delete fpRunActions;
  for(auto ua : *fpEventActions)
  { delete ua.second; }
  fpEventActions->clear();
  delete fpEventActions;
  for(auto ua : *fpStackingActions)
  { delete ua.second; }
  fpStackingActions->clear();
  delete fpStackingActions;
  for(auto ua : *fpTrackingActions)
  { delete ua.second; }
  fpTrackingActions->clear();
  delete fpTrackingActions;
  instance = nullptr;
}



