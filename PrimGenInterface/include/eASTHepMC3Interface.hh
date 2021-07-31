// ********************************************************************
//
// eASTHepMC3Interface.hh
//   Header file of HepMC3 interface
//
// History
//  June 23rd, 2021 : first empty class definition - Makoto Asai (SLAC)
//
// ********************************************************************

#ifndef eASTHepMC3Interface_h
#define eASTHepMC3Interface_h 1

#include "G4VPrimaryGenerator.hh"
#include "G4PrimaryVertex.hh"
#include "globals.hh"

#include <HepMC3/Reader.h>
#include <set>
#include <map>


class G4Event;
class G4GenericMessenger;

class eASTHepMC3Interface : public G4VPrimaryGenerator
{
private:
  static eASTHepMC3Interface* instance;
  static G4bool instantiated;

public:
  // Creat the singleton instance and return its pointer
  // Maybe nullptr if it has already deleted.
  static eASTHepMC3Interface* GetInstance();

private:
  eASTHepMC3Interface();

public:
  virtual ~eASTHepMC3Interface();
    
public:
  // The method for filling primary vertex and
  // primary particles.
  virtual void GeneratePrimaryVertex(G4Event*);
 
public:
  // Open the input HepMC3 file 
  // return false if the file cannot be opened.
  G4bool OpenFile(G4String);

  // Set/Get vertex position 
  void SetVertexPosition(G4ThreeVector vPos)
  { vPosition = vPos; }
  G4ThreeVector GetVertexPosition() const
  { return vPosition; }

  // SetGet vertex time
  void SetVertexTime(G4double t0)
  { vTime = t0; }
  G4double GetVertexTime() const
  { return vTime; }

  // Verbose level
  void SetVerbose(G4int vl)
  { verboseLevel = vl; }
    
private:
  G4String            fileName;
  G4GenericMessenger* messenger;

  G4ThreeVector   vPosition = G4ThreeVector(0.,0.,0.);
  G4double        vTime = 0.;
  G4int           verboseLevel = 1;

  std::shared_ptr<HepMC3::Reader> HepMC3Reader;
  G4PrimaryParticle* MakeParticle ( const HepMC3::ConstGenParticlePtr hep_p,
				    const bool safetycheck, std::set<int>& used);

  std::map<int,G4PrimaryParticle*> created_daughters;
  
};

#endif
