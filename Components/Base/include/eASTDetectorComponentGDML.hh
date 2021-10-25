// ********************************************************************
//
// eASTDetectorComponentGDML.hh
//   eAST Beam pipe component
//
// History
//   October 25, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTDetectorComponentGDML_H
#define eASTDetectorComponentGDML_H 1

#include "eASTVDetectorComponent.hh"

class G4GenericMessenger;

class eASTDetectorComponentGDML : public eASTVDetectorComponent
{
  public:
    eASTDetectorComponentGDML(G4String compName, G4int vl = 0, const G4bool validate_gdml=false);
    virtual ~eASTDetectorComponentGDML();

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

