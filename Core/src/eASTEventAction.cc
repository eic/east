// ********************************************************************
//
// eASTEventAction.cc
//   Header file of eAST Event Action class 
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#include "eASTEventAction.hh"
#include "eASTUserActionDispatcher.hh"
#include "globals.hh"
#include "G4Event.hh"

eASTEventAction::eASTEventAction()
{;}

eASTEventAction::~eASTEventAction()
{;}

void eASTEventAction::BeginOfEventAction(const G4Event* evt)
{
  auto eventActions = eASTUserActionDispatcher::Instance()->GetEventActions();
  if((eventActions!=nullptr) && !(eventActions->empty()))
  {
    for(auto ua : *eventActions)
    { ua.second->BeginOfEventAction(evt); }
  }
}

void eASTEventAction::EndOfEventAction(const G4Event* evt)
{
  auto eventActions = eASTUserActionDispatcher::Instance()->GetEventActions();
  if((eventActions!=nullptr) && !(eventActions->empty()))
  {
    for(auto ua : *eventActions)
    { ua.second->EndOfEventAction(evt); }
  }
}

