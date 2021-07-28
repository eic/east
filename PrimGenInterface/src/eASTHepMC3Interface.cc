// ********************************************************************
//
// eASTHepMC3Interface.cc
//   HepMC3 interface
//
// History
//  June 23rd, 2021 : first empty class definition - Makoto Asai (SLAC)
//
// ********************************************************************

#include "eASTHepMC3Interface.hh"

#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"

#ifdef eAST_USE_HepMC3
#include <HepMC3/GenEvent.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/GenParticle.h>
#endif

#include "G4AutoLock.hh"
namespace { G4Mutex myHepMC3Mutex = G4MUTEX_INITIALIZER; }

eASTHepMC3Interface* eASTHepMC3Interface::instance = nullptr;
G4bool eASTHepMC3Interface::instantiated = false;
eASTHepMC3Interface* eASTHepMC3Interface::GetInstance()
{
  G4AutoLock mlock(&myHepMC3Mutex);
  if(instance==nullptr && !instantiated)
  { instance = new eASTHepMC3Interface(); }
  return instance;
}

eASTHepMC3Interface::eASTHepMC3Interface()
{
  messenger = new G4GenericMessenger(this,"/eAST/generator/HepMC3/",
                  "HepMC3 interface commands");

  auto& fileCmd = messenger->DeclareMethod("openFile",
          &eASTHepMC3Interface::OpenFile, "Open the HepMC3 input file");
  fileCmd.SetParameterName("fileName",false);
  fileCmd.SetToBeBroadcasted(false);

  auto& vtxCmd = messenger->DeclareMethodWithUnit("vertex","mm",
          &eASTHepMC3Interface::SetVertexPosition,
          "Set vertex position");
  vtxCmd.SetParameterName("position",false);
  vtxCmd.SetToBeBroadcasted(false);

  auto& timeCmd = messenger->DeclareMethodWithUnit("time0","s",
          &eASTHepMC3Interface::SetVertexTime,
          "Set t0 (start time at the primary vertex)");
  timeCmd.SetParameterName("t0",false);
  timeCmd.SetToBeBroadcasted(false);

  auto& verCmd = messenger->DeclareMethod("verbose",
         &eASTHepMC3Interface::SetVerbose,"Verbose level");
  verCmd.SetParameterName("verboseLevel",true);
  verCmd.SetDefaultValue("0");
  verCmd.SetToBeBroadcasted(false);

  instantiated = true;
}

eASTHepMC3Interface::~eASTHepMC3Interface()
{
  G4AutoLock mlock(&myHepMC3Mutex);
  delete messenger; 
  instance = nullptr;
}

G4bool eASTHepMC3Interface::OpenFile(G4String fName)
{
  fileName = fName;

  // Make a new reader for every file.
  // We could use a central one, but we want to differentiate between
  // HepMC2 and HepMC3 anyway, and the smart pointer does our cleanup
  HepMC3Reader =  make_shared<ReaderAscii>(fileName);

  
  // TODO: Need to determine version, then we can use
  // HepMC3Reader = std::make_shared<HepMC3::ReaderAsciiHepMC2>(fileName);



  
  // return false if file cannot be opened
  return true;
}

void eASTHepMC3Interface::GeneratePrimaryVertex(G4Event* g4event)
{
  G4AutoLock mlock(&myHepMC3Mutex);

/************************************************************************
  // loop over vertecies
  for(...)
  {
    auto* g4vtx = new G4PrimaryVertex(vPosition.x(),vPosition.y(),vPosition.z(),vTime);
  
    // loop over particles
    for(...)
    {
      auto* g4prim = new G4PrimaryParticle(pdgcode, p.x(), p.y(), p.z());
      if(...)
      // pre-assigned decay
      { g4motherprim->SetDaughter(g4prim); }
      else
      // primary particle
      { g4vtx->SetPrimary(g4prim); }
    }

    g4event->AddPrimaryVertex(g4vtx);
  }
************************************************************************/
}

