// ********************************************************************
//
// eASTActionInitialization.cc
//   Action initialization class
//
// History
//   May 8th, 2021 : first implementation - Makoto Asai (SLAC)
//   June 23rd, 2021 : Add HepMC3INterface - Makoto Asai (SLAC)
//
// ********************************************************************

#include "eASTActionInitialization.hh"
#include "eASTRunAction.hh"
#include "eASTPrimaryGeneratorAction.hh"
#include "G4GenericMessenger.hh"

#ifdef eAST_USE_HepMC3
#include "eASTHepMC3Interface.hh"
#endif // eAST_USE_HepMC3

eASTActionInitialization::eASTActionInitialization()
{
  filler = new G4TScoreHistFiller<G4AnalysisManager>;

  generatorMsg = new G4GenericMessenger(this,"/eAST/generator/",
                            "primary particle generator selection");

  auto& useParticleGunCmd = generatorMsg->DeclareProperty("useParticleGun",
                useParticleGun, "use Particle Gun");
  useParticleGunCmd.SetStates(G4State_PreInit);
  useParticleGunCmd.SetToBeBroadcasted(false);
  auto& useParticleSourceCmd = generatorMsg->DeclareProperty("useParticleSource",
                useParticleSource, "use General Particle Source");
  useParticleSourceCmd.SetStates(G4State_PreInit);
  useParticleSourceCmd.SetToBeBroadcasted(false);

#ifdef eAST_USE_HepMC3
  auto& useHepMC3InterfaceCmd = generatorMsg->DeclareProperty("useHepMC3",
                useHepMC3Interface, "use HepMC3 interface");
  useHepMC3InterfaceCmd.SetStates(G4State_PreInit);
  useHepMC3InterfaceCmd.SetToBeBroadcasted(false);
#endif // eAST_USE_HepMC3
}

eASTActionInitialization::~eASTActionInitialization()
{
  delete filler;
  delete generatorMsg;
}

void eASTActionInitialization::BuildForMaster() const
{
  SetUserAction(new eASTRunAction);
#ifdef eAST_USE_HepMC3
 if(useHepMC3Interface) 
 {
   auto* HepMC3 = eASTHepMC3Interface::GetInstance();
   G4cout << "eASTHepMC3Interface is instantiated ########### " << HepMC3 << G4endl;
 }
#endif // eAST_USE_HepMC3
}

void eASTActionInitialization::Build() const
{
  SetUserAction(new eASTRunAction);
  if(!useParticleGun && !useParticleSource && !useHepMC3Interface)
  {
    G4ExceptionDescription ed;
    ed << "No way to generate primary particles!!!\n"
       << "Use command(s) in /eAST/generator/ to define at least one primary generator.";
    G4Exception("eASTActionInitialization::Build()","eAST0001",FatalException,ed);
  }
  SetUserAction(new eASTPrimaryGeneratorAction(useParticleGun,useParticleSource,
                                               useHepMC3Interface));
}  

