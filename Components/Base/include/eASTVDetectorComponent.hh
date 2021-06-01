// ********************************************************************
//
// eASTVDetectorComponent.hh
//   Header file of the abstract base class of eAST detector component
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTVDetectorComponent_H
#define eASTVDetectorComponent_H 1

class G4VPhysicalVolume;
class G4Region;
class eASTDetectorComponentMessenger;

#include "globals.hh"

class eASTVDetectorComponent
{
  public:
    eASTVDetectorComponent(G4String compName, G4int vl = 0);
    virtual ~eASTVDetectorComponent();

  public:
    void SetUpBase(G4int vl = 0);
    virtual void SetUp() = 0;
    virtual void Construct(G4VPhysicalVolume*) = 0;
    virtual void ConstructSD()
    {;}

  public:
    virtual G4VPhysicalVolume* GetEnvelope()
    { return pEnvelopePhys; }
    virtual G4Region* GetRegion()
    { return pRegion; }

  protected:
    void ReadMaterialFile(G4String);

  protected:
    G4String componentName;
    G4String commandDir;
    G4int verboseLevel;
    G4VPhysicalVolume* pEnvelopePhys = nullptr;
    G4Region* pRegion = nullptr;

  private:
    eASTDetectorComponentMessenger* baseMessenger = nullptr;
};

#endif

