// ********************************************************************
//
// eASTPrimaryGeneratorAction.cc
//   Gorad primary generator action class
//
// History
//   September 8th, 2020 : first implementation
//
// ********************************************************************

#include "eASTPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#ifdef eAST_USE_HepMC3
#include "eASTHepMC3Interface.hh"
#endif //  eAST_USE_HepMC3

eASTPrimaryGeneratorAction::eASTPrimaryGeneratorAction(
             G4bool useParticleGun, G4bool useParticleSource,
             G4bool useHepMC3Interface)
: G4VUserPrimaryGeneratorAction()
{
  if(useParticleGun)
  {
    fParticleGun  = new G4ParticleGun(1);
  
    auto particleTable = G4ParticleTable::GetParticleTable();
    auto fPion = particleTable->FindParticle("pi+");
    fParticleGun->SetParticleDefinition(fPion);
  
    // default particle kinematics
    fParticleGun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
    fParticleGun->SetParticleEnergy(1.*GeV);
  }

  if(useParticleSource)
  { fParticleSource = new G4GeneralParticleSource(); }

#ifdef eAST_USE_HepMC3
  if(useHepMC3Interface)
  { fHepMC3Interface = eASTHepMC3Interface::GetInstance(); }
#endif // eAST_USE_HepMC3
}

eASTPrimaryGeneratorAction::~eASTPrimaryGeneratorAction()
{
  if(fParticleGun!=nullptr) delete fParticleGun;
  if(fParticleSource!=nullptr) delete fParticleSource;

#ifdef eAST_USE_HepMC3
  if(fHepMC3Interface!=nullptr && eASTHepMC3Interface::GetInstance()!=nullptr)
    { delete fHepMC3Interface; }
#endif // eAST_USE_HepMC3
}

void eASTPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  if(fParticleGun) fParticleGun->GeneratePrimaryVertex(event);
  if(fParticleSource) fParticleSource->GeneratePrimaryVertex(event);
#ifdef eAST_USE_HepMC3
  if(fHepMC3Interface) fHepMC3Interface->GeneratePrimaryVertex(event);
#endif // eAST_USE_HepMC3
}

