// ********************************************************************
//
// eASTTrackingAction.cc
//   Header file of eAST Tracking Action class 
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#include "eASTTrackingAction.hh"
#include "eASTUserActionDispatcher.hh"
#include "globals.hh"
#include "G4Track.hh"

eASTTrackingAction::eASTTrackingAction()
{;}

eASTTrackingAction::~eASTTrackingAction()
{;}

void eASTTrackingAction::PreUserTrackingAction(const G4Track* trk)
{
  auto trackingActions = eASTUserActionDispatcher::Instance()->GetTrackingActions();
  if((trackingActions!=nullptr) && !(trackingActions->empty()))
  {
    for(auto ua : *trackingActions)
    { ua.second->PreUserTrackingAction(trk); }
  }
}

void eASTTrackingAction::PostUserTrackingAction(const G4Track* trk)
{
  auto trackingActions = eASTUserActionDispatcher::Instance()->GetTrackingActions();
  if((trackingActions!=nullptr) && !(trackingActions->empty()))
  {
    for(auto ua : *trackingActions)
    { ua.second->PostUserTrackingAction(trk); }
  }
}

