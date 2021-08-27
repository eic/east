// ********************************************************************
//
// eASTBeamPipe.hh
//   eAST Beam pipe component
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTBeamPipe_H
#define eASTBeamPipe_H 1

#include "eASTVDetectorComponent.hh"

class G4GenericMessenger;

class eASTBeamPipe : public eASTVDetectorComponent
{
  public:
    eASTBeamPipe(G4String compName, G4int vl = 0, const G4bool validate_gdml=false);
    virtual ~eASTBeamPipe();

  public:
    virtual void SetUp();
    virtual void Construct(G4VPhysicalVolume*);

  private:
    G4GenericMessenger* messenger = nullptr;
    G4String envGdmlFileName = "*NOTDEFINED*";
    G4String gdmlFileName = "*NOTDEFINED*";
    G4String matFileName  = "*NOTDEFINED*";
    G4bool materialToBeSet = false;

  protected: 
    const G4bool m_validate_gdml=false;

  public:
    void SetEnvGDML(G4String fn)
    { envGdmlFileName = fn; }
    void SetGDML(G4String fn)
    { gdmlFileName = fn; }
    void SetMatFile(G4String fn)
    {
      matFileName = fn;
      materialToBeSet = true; 
    }
};

#endif

