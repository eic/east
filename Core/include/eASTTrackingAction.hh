// ********************************************************************
//
// eASTTrackingAction.hh
//   Header file of eAST Tracking Action class 
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTTrackingAction_h
#define eASTTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class G4Track;
    
class eASTTrackingAction : public G4UserTrackingAction
{
  public:
    eASTTrackingAction();
    virtual ~eASTTrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

};

#endif
