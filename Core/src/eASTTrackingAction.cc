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
#include "G4TrackingManager.hh"
#include "G4GenericMessenger.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

eASTTrackingAction::eASTTrackingAction()
{
  messenger = new G4GenericMessenger(this,"/eAST/trajectory/","eAST trajectory commands");
  auto& mkeCmd = messenger->DeclarePropertyWithUnit("minimumKE","MeV",
         lowELimit,"minimum kinetic energy for recording as a trajectory");
  mkeCmd.SetStates(G4State_Idle);
}

eASTTrackingAction::~eASTTrackingAction()
{
  delete messenger;
}

void eASTTrackingAction::PreUserTrackingAction(const G4Track* trk)
{
  auto trackingActions = eASTUserActionDispatcher::Instance()->GetTrackingActions();
  if((trackingActions!=nullptr) && !(trackingActions->empty()))
  {
    for(auto ua : *trackingActions)
    { ua.second->PreUserTrackingAction(trk); }
  }

  defaultStoreTraj = fpTrackingManager->GetStoreTrajectory();

  if(trk->GetParentID() > 0) // secondary track
  {
    auto partDef = trk->GetDefinition();
    if( partDef == G4Gamma::Definition() || partDef == G4Electron::Definition() 
        || partDef == G4Positron::Definition() ) // for e+/- and gamma
    {
      if(trk->GetKineticEnergy() < lowELimit) 
      { fpTrackingManager->SetStoreTrajectory(0); }
    }
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

  // restore trajectory option
  fpTrackingManager->SetStoreTrajectory(defaultStoreTraj);
}

