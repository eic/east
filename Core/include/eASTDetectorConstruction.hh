// ********************************************************************
//
// eASTDetectorConstruction.hh
//   Header file of the detector construction. 
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTDetectorConstruction_H
#define eASTDetectorConstruction_H 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <map>

class eASTDetectorConstructionMessenger;
class eASTVDetectorComponent;
class G4VPhysicalVolume;

class eASTDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    eASTDetectorConstruction();
    virtual ~eASTDetectorConstruction();
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDAndField();

  public:
    static eASTDetectorConstruction* Instance();

  private:
    static eASTDetectorConstruction* instance;

  private:
    eASTDetectorConstructionMessenger* messenger;
    G4VPhysicalVolume* fWorld = nullptr;

  public:
    void RegisterComponent(G4String,eASTVDetectorComponent*);

  private:
    std::map<G4String,eASTVDetectorComponent*> components;

  public:
    void ListSolids(G4int);
    void ListLogVols(G4int);
    void ListPhysVols(G4int);
    void ListRegions(G4int);
    G4bool CheckOverlap(G4String&,G4int,G4int,G4double);

    void ListAllMaterial();
    G4bool ListMaterial(G4String&);
    void DumpNistMaterials();
    G4bool CreateMaterial(G4String&);
    G4bool GetMaterial(G4String&);
    G4int SetMaterial(G4String&,G4String&);

};

#endif
