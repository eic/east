////////////////////////////////////////////////////////////////////////////////
//
//  eASTPhysicsList.cc
//  Geant4 physics list for Electron Ion Collider detector simulation
//
//  History
//    Jun.21.2018 : original implementation - Dennis H. Wright (SLAC)
//    May.05.2021 : migration to Geant4 version 10.7 - Makoto Asai (SLAC)
//
////////////////////////////////////////////////////////////////////////////////

#include "eASTPhysicsList.hh"

#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4EmParameters.hh"
#include "G4DecayPhysics.hh"
#include "G4NuclideTable.hh"
// #include "G4RadioactiveDecayPhysics.hh"

#include "eASTProtonPhysics.hh"
#include "eASTNeutronPhysics.hh"
#include "eASTPionPhysics.hh"
#include "eASTKaonPhysics.hh"
#include "eASTHyperonPhysics.hh"
#include "eASTAntiBaryonPhysics.hh"
#include "eASTIonPhysics.hh"
#include "eASTGammaLeptoNuclearPhysics.hh"

// particles

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"


eASTPhysicsList::eASTPhysicsList()
 :G4VModularPhysicsList()
{
  G4int verb = 1;
  SetVerboseLevel(verb);
  
  //add new units for radioActive decays
  //
  new G4UnitDefinition( "millielectronVolt", "meV", "Energy", 1.e-3*eV);   
  // 
  const G4double minute = 60*second;
  const G4double hour   = 60*minute;
  const G4double day    = 24*hour;
  const G4double year   = 365*day;
  new G4UnitDefinition("minute", "min", "Time", minute);
  new G4UnitDefinition("hour",   "h",   "Time", hour);
  new G4UnitDefinition("day",    "d",   "Time", day);
  new G4UnitDefinition("year",   "y",   "Time", year);

  // Mandatory for G4NuclideTable
  // Half-life threshold must be set small or many short-lived isomers 
  // will not be assigned life times (default to 0) 
  G4NuclideTable::GetInstance()->SetThresholdOfHalfLife(0.1*picosecond);
  G4NuclideTable::GetInstance()->SetLevelTolerance(1.0*eV);
          
  // EM physics
  RegisterPhysics(new G4EmStandardPhysics());
  G4EmParameters* param = G4EmParameters::Instance();
  param->SetAugerCascade(true);
  param->SetStepFunction(1., 1.*CLHEP::mm);
  param->SetStepFunctionMuHad(1., 1.*CLHEP::mm);

  // Decay
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
//  RegisterPhysics(new G4RadioactiveDecayPhysics());
            
  // Hadronic physics
  RegisterPhysics(new eASTProtonPhysics() );
  RegisterPhysics(new eASTNeutronPhysics() );
  RegisterPhysics(new eASTPionPhysics() );
  RegisterPhysics(new eASTKaonPhysics() );
  RegisterPhysics(new eASTHyperonPhysics() );
  RegisterPhysics(new eASTAntiBaryonPhysics() );
  RegisterPhysics(new eASTIonPhysics() );
  RegisterPhysics(new eASTGammaLeptoNuclearPhysics() );

  // Gamma-Nuclear Physics
//  G4EmExtraPhysics* gnuc = new G4EmExtraPhysics(verb);
//  gnuc->ElectroNuclear(false);
//  gnuc->MuonNuclear(false);
//  RegisterPhysics(gnuc);

  // Optical physics
//  RegisterPhysics(new G4OpticalPhysics() );
}


eASTPhysicsList::~eASTPhysicsList()
{}


void eASTPhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}


void eASTPhysicsList::SetCuts()
{
  SetCutValue(0.7*mm, "proton");
  SetCutValue(0.7*mm, "e-");
  SetCutValue(0.7*mm, "e+");
  SetCutValue(0.7*mm, "gamma");      
}

