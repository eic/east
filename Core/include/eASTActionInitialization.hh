// ********************************************************************
//
// eASTActionInitialization.hh  
//   Action initialization class
//
// History
//   May 8th, 2021 : first implementation - Makoto Asai (SLAC)
//
// ********************************************************************

#ifndef eASTActionInitialization_h
#define eASTActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

#include "eASTAnalysis.hh"
#include "G4TScoreHistFiller.hh"
#include "G4Types.hh"

class G4GenericMessenger;

class eASTActionInitialization : public G4VUserActionInitialization
{
  public:
    eASTActionInitialization();
    virtual ~eASTActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    G4TScoreHistFiller<G4AnalysisManager>* filler;
    G4GenericMessenger* generatorMsg;
    G4bool useParticleGun = false;
    G4bool useParticleSource = true;
    G4bool useHepMC3Interface = false;
};

#endif

    
