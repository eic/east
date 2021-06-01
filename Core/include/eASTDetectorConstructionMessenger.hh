// ********************************************************************
//
// eASTDetectorConstructionMessenger.hh
//   Headter file of a messenger class that handles geometry configuration.
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTDetectorConstructionMessenger_H
#define eASTDetectorConstructionMessenger_H 1

#include "G4UImessenger.hh"
#include "globals.hh"

class eASTDetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;

class eASTDetectorConstructionMessenger: public G4UImessenger
{
  public:
    eASTDetectorConstructionMessenger(eASTDetectorConstruction*);
    virtual ~eASTDetectorConstructionMessenger();
    virtual void SetNewValue(G4UIcommand*,G4String);
    virtual G4String GetCurrentValue(G4UIcommand*);

  private:
    eASTDetectorConstruction* pDC;

    G4UIdirectory* 		geomDir;
    G4UIcmdWithAnInteger*	listSolidCmd;
    G4UIcmdWithAnInteger*	listLogVolCmd;
    G4UIcmdWithAnInteger*	listPhysVolCmd;
    G4UIcmdWithAnInteger*	listRegionCmd;
    G4UIcommand*                checkOverlapCmd;

    G4UIdirectory*              materialDir;
    G4UIcmdWithAString*         listMatCmd;
    G4UIcmdWithoutParameter*    dumpMatCmd;
    G4UIcmdWithAString*         createMatCmd;
    G4UIcmdWithAString*         getMatCmd;
    G4UIcommand*                setMatCmd;

};

#endif

