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

eASTPrimaryGeneratorAction::eASTPrimaryGeneratorAction(
             G4bool useParticleGun, G4bool useParticleSource)
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
}

eASTPrimaryGeneratorAction::~eASTPrimaryGeneratorAction()
{
  if(fParticleGun) delete fParticleGun;
  if(fParticleSource) delete fParticleSource;
}

void eASTPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  if(fParticleGun) fParticleGun->GeneratePrimaryVertex(event);
  if(fParticleSource) fParticleSource->GeneratePrimaryVertex(event);
}

