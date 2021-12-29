// ********************************************************************
//
// eASTPrimGenActionMessenger.hh
//   Headter file of a messenger class that sets primary vertex parameters
//
// History
//   Dec 28th, 2021 : first implementation - M. Asai (JLab)
//
// ********************************************************************

#ifndef eASTPrimGenActionMessenger_H
#define eASTPrimGenActionMessenger_H 1

#include "G4UImessenger.hh"
#include "globals.hh"

class eASTPrimaryGeneratorAction;
class G4UIcommand;

class eASTPrimGenActionMessenger: public G4UImessenger
{
  public:
    eASTPrimGenActionMessenger(eASTPrimaryGeneratorAction*);
    virtual ~eASTPrimGenActionMessenger();
    virtual void SetNewValue(G4UIcommand*,G4String);
    virtual G4String GetCurrentValue(G4UIcommand*);

  private:
    eASTPrimaryGeneratorAction* pPG;

    G4UIcommand*                setTimeCmd;

};

#endif

