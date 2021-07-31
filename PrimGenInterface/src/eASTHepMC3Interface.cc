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

// The newer ReaderFactory is header-only and can be used for older versions
// This file is copied verbatim from https://gitlab.cern.ch/hepmc/HepMC3
// Copyright (C) 2014-2020 The HepMC collaboration, licensed under GPL3
#include <HepMC3/Version.h>
#if HEPMC3_VERSION_CODE < 3002004
#include <HepMC_3_2_4_ReaderFactory.h>
#else
#include <HepMC3/ReaderFactory.h>
#endif

#include <HepMC3/GenEvent.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/GenParticle.h>


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
  if(verboseLevel>0){
    G4cout << "eASTHepMC3Interface - opening " << fileName << G4endl;
  }

  // Make a new reader for every file.
  // Note: This supports a variety of formats (versions 1, 2, 3, HepEct, Root)
  //       and sources (files, streams, urls, ...)
  HepMC3Reader = HepMC3::deduce_reader(fileName); 

  if ( !HepMC3Reader || HepMC3Reader->failed() ) {
    G4Exception("eASTHepMC3Interface::GeneratePrimaryVertex","Event0201",
		FatalException, "eASTHepMC3Interface:: cannot open input.");
  }    
  // if ( !HepMC3Reader || HepMC3Reader->failed() ) return false;
  
  if(verboseLevel>0){
    G4cout << "eASTHepMC3Interface - " << fileName << " is open." << G4endl;
  }
     
  return true;
}

void eASTHepMC3Interface::GeneratePrimaryVertex(G4Event* g4event)
{
  G4AutoLock mlock(&myHepMC3Mutex);

  G4cerr << 1 << G4endl;

  //read the event
  HepMC3::GenEvent hepevt(HepMC3::Units::GEV,HepMC3::Units::MM);

  if (HepMC3Reader->failed()){
    // Todo: Don't know if there's a correct exceptionCode
    G4Exception("eASTHepMC3Interface::GeneratePrimaryVertex","Event0201",
		FatalException, "eASTHepMC3Interface:: cannot open input.");
  }
  G4cerr << 2 << G4endl;
  if ( !HepMC3Reader->read_event(hepevt) ) return; // false for eof
  G4cerr << 3 << G4endl;

  // The root vertex is the default primary vertex
  // There can be multiple, unconnected graphs in the event.
  // In all cases I've seen, that is done for technical reasons,
  // the record still only has one event, i.e. they all share the same primary vertex
  auto pos = hepevt.event_pos();
  auto* g4vtx  = new G4PrimaryVertex(pos.x()*mm, pos.y()*mm, pos.z()*mm, 0);

  G4cerr << 4 << G4endl;
  // loop over particles
  // allowed statuses: 1 - final, 2 - decayed hadron/lepton
  // decay daughters will be enrolled by their mothers.
  // topological ordering in HepMC guarantees that mothers are
  // seen first, so only need to keep track of already created ones
  // to either ignore or reuse
  // using the std::map created_daughters;

  // better safe than sorry, detect faulty double-counting
  bool safetycheck=true;
  std::set<int> used;
  
  for(auto hep_p : hepevt.particles()) {

    auto status = hep_p->status();
    if ( status == 1 || status == 2  ) continue;
    
    // already created?
    auto id = hep_p->id();
    auto finditer=created_daughters.find( id );
    G4PrimaryParticle* g4prim;
    if ( finditer != created_daughters.end() ){
      g4prim = finditer->second;
    } else {
      // doesn't exist already -> need to create
      // that also means it belongs to the primary vertex
      g4prim = MakeParticle ( hep_p, safetycheck, used );
      g4vtx->SetPrimary(g4prim);
    }

    if ( hep_p->status() == 1 ){ // final
      // Nothing else to do, but should look for sanity
      if ( hep_p->children().size() !=0 ){
	// This should never happen
	G4Exception("eASTHepMC3Interface::GeneratePrimaryVertex","HepmcStatus",
		    FatalException, "eASTHepMC3Interface:: status 1 but children.");
      }
      continue;
    }
    
    
    if ( hep_p->status() == 2 ){ // decayed, has daugthers
      if ( hep_p->children().size() ==0 ){
	// This should never happen
	G4Exception("eASTHepMC3Interface::GeneratePrimaryVertex","HepmcStatus",
		    FatalException, "eASTHepMC3Interface:: status 2 but no children.");
      }
      // Now register daughters
      for ( auto hep_d : hep_p->children()) {
	auto id_d = hep_d->id();
	auto finditer_d=created_daughters.find( id_d );
	if ( finditer_d != created_daughters.end() ){
	  // This should never happen
	  G4Exception("eASTHepMC3Interface::GeneratePrimaryVertex","Daughter",
		      FatalException, "eASTHepMC3Interface:: found daughter out of order.");
	}
	if ( hep_p->status() == 2 || hep_p->status() == 1 ){
	  auto g4daughter = MakeParticle ( hep_d, safetycheck, used );
	  g4prim->SetDaughter(g4daughter);
	  // and register
	  created_daughters[id_d] = g4daughter;      
	}
      }
    } // end of status == 1
    
    
    // if(...)
    // // pre-assigned decay
    // { g4motherprim->SetDaughter(g4prim); }
    // else
    // // primary particle
    // { g4vtx->SetPrimary(g4prim); }
    // }
    
  }//particle loop
  
  g4event->AddPrimaryVertex(g4vtx);
}

G4PrimaryParticle* eASTHepMC3Interface::MakeParticle ( const HepMC3::ConstGenParticlePtr hep_p,
						       const bool safetycheck, std::set<int>& used){
  // Shouldn't see a particle more than once
  if ( safetycheck ){
    auto id = hep_p->id();
    if ( used.count ( id ) ){ // contains() is c++20...
      G4Exception("eASTHepMC3Interface::MakeParticle","Particle",
		  FatalException, "eASTHepMC3Interface:: Double-counting particles.");
    } else {
      used.insert( id );
    }
  }
  auto p = hep_p->momentum();
  auto* g4prim = new G4PrimaryParticle(hep_p->pid(), p.x(), p.y(), p.z());
  g4prim->SetPolarization(0, 0, 0);
  
  if ( verboseLevel > 1) { 
    G4cout << " Made primary with pid = " << hep_p->pid()
	   << ", status = " << hep_p->status()
	   << " and 4-momentum = " << p.px() << " " << p.py() << " , " << p.pz() << " , " << p.e()
	   << G4endl;
  }
  
  return g4prim; 
}

