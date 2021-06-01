// ********************************************************************
//
// eRInitialization.hh
//   Defines the initialization procedure of Geant4 and eAST
//
// History
//   April 28, 2021 - first implementation (M.Asai/SLAC)
//
// ********************************************************************

#ifndef eAInitialization_h
#define eAInitialization_h 1

#include "G4Types.hh"

class eASTDetectorConstruction;
class G4VModularPhysicsList;
class eASTActionInitialization;
class G4GenericMessenger;
class G4UIExecutive;

class eAInitialization
{
  public:
    eAInitialization(G4int verboseLvl=0);
    ~eAInitialization();
    void Initialize();
    void SetWindowText(G4UIExecutive*);

  private:
    eASTDetectorConstruction* detector;
    G4VModularPhysicsList* physics;
    eASTActionInitialization* actionInitialization;

    G4GenericMessenger* messenger;

    G4int verboseLevel;
};

#endif
