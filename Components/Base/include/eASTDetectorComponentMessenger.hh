// ********************************************************************
//
// eASTDetectorComponentMessenger.hh
//   Headter file of a messenger class that handles geometry configuration.
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTDetectorComponentMessenger_H
#define eASTDetectorComponentMessenger_H 1

#include "G4UImessenger.hh"
#include "globals.hh"

class eASTVDetectorComponent;
class G4UIcmdWithAnInteger;
class G4UIcmdWith3VectorAndUnit;
class G4UIcommand;

class eASTDetectorComponentMessenger: public G4UImessenger
{
  public:
    eASTDetectorComponentMessenger(eASTVDetectorComponent*,G4String);
    virtual ~eASTDetectorComponentMessenger();
    virtual void SetNewValue(G4UIcommand*,G4String);
    virtual G4String GetCurrentValue(G4UIcommand*);

  public:
    void SetUpBaseCommands(G4String);

  private:
    eASTVDetectorComponent* pDC;
    G4UIcmdWithAnInteger*	setupCmd;
    G4UIcmdWith3VectorAndUnit*  locCmd;
    G4UIcommand*                rotCmd;
};

#endif

