// ********************************************************************
//
// eASTPrimaryGeneratorAction.hh
//   Header file of primary generator action class
//
// History
//  May 8th, 2021 : first implementation - Makoto Asai (SLAC)
//  June 23rd, 2021 : Add eASTHepMC3Interface - Makoto Asai (SLAC)
//  December 28th, 2021 : Add event start time - Makoto Asai (JLab)
//
// ********************************************************************

#ifndef eASTPrimaryGeneratorAction_h
#define eASTPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;

#ifdef eAST_USE_HepMC3
class eASTHepMC3Interface;
#endif // eAST_USE_HepMC3

class eASTPrimGenActionMessenger;

class eASTPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    eASTPrimaryGeneratorAction(G4bool,G4bool,G4bool);
    virtual ~eASTPrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event*);
 
  private:
    G4ParticleGun* fParticleGun = nullptr;
    G4GeneralParticleSource* fParticleSource = nullptr;
#ifdef eAST_USE_HepMC3
    eASTHepMC3Interface* fHepMC3Interface = nullptr;
#endif // eAST_USE_HepMC3

    eASTPrimGenActionMessenger* messenger = nullptr;

  public:
    // Event start time is set to (deltaT * eventID + T0)
    void SetT(G4double dt, G4double t0)
    {
      deltaT = dt;
      T0 = t0;
    }
    void GetT(G4double& dt, G4double& t0)
    {
      dt = deltaT;
      t0 = T0;
    }

  private:
    G4double deltaT = 0.0;
    G4double T0 = 0.0;
};

#endif
