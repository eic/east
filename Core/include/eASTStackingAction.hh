// ********************************************************************
//
// eASTStackingAction.hh
//   Header file of eAST Stacking Action class 
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTStackingAction_h
#define eASTStackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"

class G4Track;
    
class eASTStackingAction : public G4UserStackingAction
{
  public:
    eASTStackingAction();
    virtual ~eASTStackingAction();

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
    virtual void NewStage();
    virtual void PrepareNewEvent();

};

#endif
