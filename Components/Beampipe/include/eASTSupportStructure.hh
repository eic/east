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
    eASTSupportStructure(G4String compName, G4int vl = 0);
    virtual ~eASTSupportStructure();

  public:
    virtual void SetUp();
    virtual void Construct(G4VPhysicalVolume*);

  private:
    G4GenericMessenger* messenger = nullptr;
    G4String gdmlFileName = "*NOTDEFINED*";
    G4String matFileName  = "*NOTDEFINED*";
    G4bool materialToBeSet = false;

  public:
    void SetGDML(G4String fn)
    { gdmlFileName = fn; }
    void SetMatFile(G4String fn)
    {
      matFileName = fn;
      materialToBeSet = true; 
    }
};

#endif

