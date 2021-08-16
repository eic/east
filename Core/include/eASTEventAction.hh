// ********************************************************************
//
// eASTEventAction.hh
//   Header file of eAST Event Action class 
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTEventAction_h
#define eASTEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
    
class eASTEventAction : public G4UserEventAction
{
  public:
    eASTEventAction();
    virtual ~eASTEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);

};

#endif
