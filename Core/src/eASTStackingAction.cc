// ********************************************************************
//
// eASTStackingAction.cc
//   Header file of eAST Stacking Action class 
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#include "eASTStackingAction.hh"
#include "eASTUserActionDispatcher.hh"
#include "globals.hh"
#include "G4Track.hh"

eASTStackingAction::eASTStackingAction()
{;}

eASTStackingAction::~eASTStackingAction()
{;}

G4ClassificationOfNewTrack eASTStackingAction::ClassifyNewTrack(const G4Track* trk)
{
  G4ClassificationOfNewTrack classification = fUrgent;
  auto stackingActions = eASTUserActionDispatcher::Instance()->GetStackingActions();
  if((stackingActions!=nullptr) && !(stackingActions->empty()))
  {
    for(auto ua : *stackingActions)
    {
      auto cl =  ua.second->ClassifyNewTrack(trk);
      if(cl > classification) classification = cl;
    }
  }
  return classification;
}

void eASTStackingAction::NewStage()
{
  auto stackingActions = eASTUserActionDispatcher::Instance()->GetStackingActions();
  if((stackingActions!=nullptr) && !(stackingActions->empty()))
  {
    for(auto ua : *stackingActions)
    { ua.second->NewStage(); }
  }
}

void eASTStackingAction::PrepareNewEvent()
{
  auto stackingActions = eASTUserActionDispatcher::Instance()->GetStackingActions();
  if((stackingActions!=nullptr) && !(stackingActions->empty()))
  {
    for(auto ua : *stackingActions)
    { ua.second->PrepareNewEvent(); }
  }
}

