// ********************************************************************
//
// eASTSupportStructure.hh
//   eAST Beam pipe component
//
// History
//   June 5th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTSupportStructure_H
#define eASTSupportStructure_H 1

#include "eASTVDetectorComponent.hh"

class G4GenericMessenger;

class eASTSupportStructure : public eASTVDetectorComponent
{
  public:
    eASTSupportStructure(G4String compName, G4int vl = 0, const G4bool validate_gdml=false);
    virtual ~eASTSupportStructure();

  public:
    virtual void SetUp();
    virtual void Construct(G4VPhysicalVolume*);

  private:
    G4GenericMessenger* messenger = nullptr;
    G4String gdmlFileName = "*NOTDEFINED*";
    G4String matFileName  = "*NOTDEFINED*";
    G4bool materialToBeSet = false;

  protected: 
    const G4bool m_validate_gdml=false;

  public:
    void SetGDML(G4String fn)
    { gdmlFileName = LocateDataFile(fn); }
    void SetMatFile(G4String fn)
    {
      matFileName = LocateDataFile(fn);
      materialToBeSet = true; 
    }
};

#endif

