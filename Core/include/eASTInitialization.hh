// ********************************************************************
//
// eASTInitialization.hh
//   Defines the initialization procedure of Geant4 and eAST
//
// History
//   April 28, 2021 - first implementation (M.Asai/SLAC)
//
// ********************************************************************

#ifndef eASRInitialization_h
#define eASTInitialization_h 1

#include "G4Types.hh"

class eASTDetectorConstruction;
class G4VModularPhysicsList;
class eASTMagneticField;
class eASTActionInitialization;
class G4GenericMessenger;
class G4UIExecutive;

class eASTInitialization
{
  public:
    eASTInitialization(G4int verboseLvl=0);
    ~eASTInitialization();
    void Initialize();
    void SetWindowText(G4UIExecutive*);

  private:
    eASTDetectorConstruction* detector;
    G4VModularPhysicsList* physics;
    eASTMagneticField* field;
    eASTActionInitialization* actionInitialization;

    G4GenericMessenger* messenger;

    G4int verboseLevel;
};

#endif
