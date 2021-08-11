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
#include "G4SystemOfUnits.hh"

class G4Track;
class G4GenericMessenger;
    
class eASTTrackingAction : public G4UserTrackingAction
{
  public:
    eASTTrackingAction();
    virtual ~eASTTrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:
    G4GenericMessenger* messenger;
    G4double lowELimit = 100. * CLHEP::MeV;
    G4int defaultStoreTraj = 0;

};

#endif
