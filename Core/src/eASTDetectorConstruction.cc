// ********************************************************************
//
// eASTDetectorConstruction.cc
//   eAST detector construction class.
//   This class also takes care of several utility methods on geometry
//
// History
//   September 8th, 2020 : first implementation
//
// ********************************************************************

#include "eASTDetectorConstruction.hh"

#include "eASTDetectorConstructionMessenger.hh"
#include "eASTVDetectorComponent.hh"

#include "G4RunManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "eASTRegionInformation.hh"
#include <algorithm>

eASTDetectorConstruction* eASTDetectorConstruction::instance = nullptr;
eASTDetectorConstruction* eASTDetectorConstruction::Instance()
{
  if(instance==nullptr)
  { new eASTDetectorConstruction; }
  return instance; 
}

eASTDetectorConstruction::eASTDetectorConstruction()
{
  if(instance!=nullptr)
  {
    G4Exception("eASTDetectorConstruction::eASTDetectorConstruction()",
                "eAST01230", FatalException,
                "eASTDetectorConstruction is constructed twice!!");
  }
  instance = this;
  messenger = new eASTDetectorConstructionMessenger(this);
}

eASTDetectorConstruction::~eASTDetectorConstruction()
{
  for(auto comp : components)
  { delete comp.second; }
  components.clear();
  delete messenger;
  instance = nullptr;
}

void eASTDetectorConstruction::RegisterComponent
       (G4String compName, eASTVDetectorComponent* comp)
{
  components[compName] = comp;
}

G4VPhysicalVolume* eASTDetectorConstruction::Construct()
{
  if(fWorld==nullptr)
  {
    if(components.empty())
    {
      G4Exception("eASTDetectorConstruction::Construct()",
                  "eAST01231", FatalException,
                  "No detector component is registered. Nothing to construct!!");
    }

    auto worldBox = new G4Box("worldBox",50.*CLHEP::m,50.*CLHEP::m,150.*CLHEP::m);
    auto air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    auto worldLog = new G4LogicalVolume(worldBox,air,"worldLog");
    auto visAtt = new G4VisAttributes(G4Colour(0.5,0.5,0.5));
    //visAtt->SetVisibility(false);
    visAtt->SetForceWireframe();
    worldLog->SetVisAttributes(visAtt);
    fWorld = new G4PVPlacement(0,G4ThreeVector(),worldLog,"worldPhys",0,0,0);

    auto worldDefRegion = G4RegionStore::GetInstance()
                                ->GetRegion("DefaultRegionForTheWorld");
    auto worldDefRegionInfo = new eASTRegionInformation("world_RegInfo");
    worldDefRegionInfo->SetExperimentalHall();
    worldDefRegion->SetUserInformation(worldDefRegionInfo);

    for(auto comp : components)
    {
      G4cout << "##### Constructing " << comp.first << "........." << G4endl;
      comp.second->Construct(fWorld);
    }

    // now construct component-specific user run action for master thread
    // this should be done after all components are constructed
    for(auto comp : components)
    {
      comp.second->ConstructActionForMaster();
    }
  }
  return fWorld;
}

#include "G4AutoLock.hh"
namespace
{
  G4Mutex constructSDAndFieldMutex = G4MUTEX_INITIALIZER;
}

void eASTDetectorConstruction::ConstructSDAndField()
{ 
  G4AutoLock l(&constructSDAndFieldMutex);
  for(auto comp : components)
  {
    comp.second->ConstructSD();
    comp.second->ConstructActions();
  }
}

#include "G4UnitsTable.hh"
#include "G4VSolid.hh"
#include "G4SolidStore.hh"
void eASTDetectorConstruction::ListSolids(G4int lvl)
{
  G4cout << "*********** List of registered solids *************" << G4endl;
  auto store = G4SolidStore::GetInstance();
  auto itr = store->begin();
  for(;itr!=store->end();itr++)
  {
    switch(lvl)
    {
      case 0:
        G4cout << (*itr)->GetName() << G4endl;
        break;
      case 1:
        G4cout << (*itr)->GetName()
               << "\t volume = " << G4BestUnit((*itr)->GetCubicVolume(),"Volume")
               << "\t surface = " << G4BestUnit((*itr)->GetSurfaceArea(),"Surface")
               << G4endl;
        break;
      default:
        (*itr)->DumpInfo();
        break;
    }
  }
}

#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4VSensitiveDetector.hh"
void eASTDetectorConstruction::ListLogVols(G4int lvl)
{
  G4cout << "*********** List of registered logical volumes *************" << G4endl;
  auto store = G4LogicalVolumeStore::GetInstance();
  auto itr = store->begin();
  for(;itr!=store->end();itr++)
  {
    G4cout << (*itr)->GetName() << "\t Solid = " << (*itr)->GetSolid()->GetName();
    if((*itr)->GetMaterial())
    { G4cout << "\t Material = " << (*itr)->GetMaterial()->GetName() << G4endl; }
    else
    { G4cout << "\t Material : not defined " << G4endl; }
    if(lvl<1) continue;
    G4cout << "\t region = ";
    if((*itr)->GetRegion())
    { G4cout << (*itr)->GetRegion()->GetName(); }
    else
    { G4cout << "not defined"; }
    G4cout << "\t sensitive detector = ";
    if((*itr)->GetSensitiveDetector())
    { G4cout << (*itr)->GetSensitiveDetector()->GetName(); }
    else
    { G4cout << "not defined"; }
    G4cout << G4endl;
    G4cout << "\t daughters = " << (*itr)->GetNoDaughters();
    if((*itr)->GetNoDaughters()>0)
    {
      switch((*itr)->CharacteriseDaughters())
      {
        case kNormal:
          G4cout << " (placement)"; break;
        case kReplica:
          G4cout << " (replica : " << (*itr)->GetDaughter(0)->GetMultiplicity() << ")"; break;
        case kParameterised:
          G4cout << " (parameterized : " << (*itr)->GetDaughter(0)->GetMultiplicity() << ")"; break;
        default:
          ;
      }
    }
    G4cout << G4endl;
    if(lvl<2) continue;
    if((*itr)->GetMaterial())
    { G4cout << "\t weight = " << G4BestUnit((*itr)->GetMass(),"Mass") << G4endl; }
    else
    { G4cout << "\t weight : not available" << G4endl; }
  }
}

#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"
void eASTDetectorConstruction::ListPhysVols(G4int lvl)
{
  G4cout << "*********** List of registered physical volumes *************" << G4endl;
  auto store = G4PhysicalVolumeStore::GetInstance();
  auto itr = store->begin();
  for(;itr!=store->end();itr++)
  {
    switch(lvl)
    {
      case 0:
        G4cout << (*itr)->GetName() << G4endl;
        break;
      case 1:
        G4cout << (*itr)->GetName()
               << "\t logical volume = " << (*itr)->GetLogicalVolume()->GetName()
               << "\t mother logical = ";
        if((*itr)->GetMotherLogical())
        { G4cout << (*itr)->GetMotherLogical()->GetName(); }
        else
        { G4cout << "not defined"; }
        G4cout << G4endl;
        break;
      default:
        G4cout << (*itr)->GetName()
               << "\t logical volume = " << (*itr)->GetLogicalVolume()->GetName()
               << "\t mother logical = ";
        if((*itr)->GetMotherLogical())
        { G4cout << (*itr)->GetMotherLogical()->GetName(); }
        else
        { G4cout << "not defined"; }
        G4cout << "\t type = ";
        switch((*itr)->VolumeType())
        {
          case kNormal:
            G4cout << "placement"; break;
          case kReplica:
            G4cout << "replica"; break;
          case kParameterised:
            G4cout << "parameterized"; break;
          default:
            ;
        }
        G4cout << G4endl;
    }
  }
}

G4bool eASTDetectorConstruction::CheckOverlap(G4String& physVolName, G4int nSpots,
                 G4int maxErr, G4double tol)
{
  G4cout << "*********** Checking overlap for <" << physVolName << "> *************" << G4endl;
  G4bool checkAll = (physVolName=="**ALL**");
  auto store = G4PhysicalVolumeStore::GetInstance();
  auto itr = store->begin();
  G4VPhysicalVolume* physVol = nullptr;
  for(;itr!=store->end();itr++)
  {
    if(checkAll || (*itr)->GetName()==physVolName)
    {
      physVol = (*itr);
      physVol->CheckOverlaps(nSpots,tol,true,maxErr);
      if(!checkAll) break;
    }
  }
  return (physVol!=nullptr);
}

#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4RunManagerKernel.hh"
void eASTDetectorConstruction::ListRegions(G4int lvl)
{
  if(lvl==2)
  {
    G4RunManagerKernel::GetRunManagerKernel()->DumpRegion();
    return;
  }
  G4cout << "*********** List of registered regions *************" << G4endl;
  auto store = G4RegionStore::GetInstance();
  auto itr = store->begin();
  for(;itr!=store->end();itr++)
  {
    G4cout << (*itr)->GetName();
    if((*itr)->GetWorldPhysical())
    {
      G4cout << "\t in the world volume <" << (*itr)->GetWorldPhysical()->GetName() << "> ";
      if((*itr)->IsInMassGeometry()) G4cout << "-- mass world";
      if((*itr)->IsInParallelGeometry()) G4cout << "-- parallel world";
    }
    else
    { G4cout << " -- is not associated to any world."; }
    G4cout << G4endl;
    if(lvl==0) continue;
    G4cout << "\t\t Root logical volume(s) : ";
    size_t nRootLV = (*itr)->GetNumberOfRootVolumes();
    std::vector<G4LogicalVolume*>::iterator lvItr = (*itr)->GetRootLogicalVolumeIterator();
    for(size_t j=0;j<nRootLV;j++)
    { G4cout << (*lvItr)->GetName() << " "; lvItr++; }
    G4cout << G4endl;
    G4cout << "\t\t Pointers : G4VUserRegionInformation[" << (*itr)->GetUserInformation()
           << "], G4UserLimits[" << (*itr)->GetUserLimits()
           << "], G4FastSimulationManager[" << (*itr)->GetFastSimulationManager()
           << "], G4UserSteppingAction[" << (*itr)->GetRegionalSteppingAction() << "]" << G4endl;
  }
}

#include "G4MaterialTable.hh"
void eASTDetectorConstruction::ListAllMaterial()
{
  auto materialTable = G4Material::GetMaterialTable();
  auto matItr = materialTable->begin();
  G4cout << "*********** List of instantiated materials **************" << G4endl;
  G4int i = 0;
  for(;matItr!=materialTable->end();matItr++)
  {
    G4cout << (*matItr)->GetName() << "\t";
    if(++i%5==0) G4cout << G4endl;
  }
  G4cout << G4endl;
}

G4bool eASTDetectorConstruction::ListMaterial(G4String& matName)
{
  auto materialTable = G4Material::GetMaterialTable();
  auto matItr = materialTable->begin();
  for(;matItr!=materialTable->end();matItr++)
  {
    if((*matItr)->GetName()==matName) 
    {
      G4cout << *matItr << G4endl;
      return true;
    }
  }
  return false;
}

#include "G4NistManager.hh"
void eASTDetectorConstruction::DumpNistMaterials()
{
  auto nameVec = G4NistManager::Instance()->GetNistMaterialNames();
  auto itr = nameVec.begin();
  G4int i = 0;
  for(;itr!=nameVec.end();itr++)
  {
    G4cout << std::setw(26) << *itr;
    if(++i%3==0) G4cout << G4endl;
  }
  G4cout << G4endl;
}

G4bool eASTDetectorConstruction::CreateMaterial(G4String& matName)
{
  auto mat = G4NistManager::Instance()->FindOrBuildMaterial(matName);
  return (mat!=nullptr);
}

G4bool eASTDetectorConstruction::GetMaterial(G4String& logVol)
{
  auto store = G4LogicalVolumeStore::GetInstance();
  std::vector<G4LogicalVolume*>::iterator itr = store->begin();
  for(;itr!=store->end();itr++)
  {
    if((*itr)->GetName()==logVol)
    {
      G4cout << "Logical volume <" << (*itr)->GetName() << "> is made of <"
           << (*itr)->GetMaterial()->GetName() << ">" << G4endl;
      return true;
    }
  }
  return false;
}

G4int eASTDetectorConstruction::SetMaterial(G4String& logVolName,G4String& matName)
{
  G4LogicalVolume* logVol = nullptr;
  G4Material* mat = nullptr;

  auto store = G4LogicalVolumeStore::GetInstance();
  auto itr = store->begin();
  for(;itr!=store->end();itr++)
  {
    if(((*itr)->GetName()).find(logVolName)!=std::string::npos)
    {
      logVol = *itr;
      break;
    }
  }

  auto materialTable = G4Material::GetMaterialTable();
  auto matItr = materialTable->begin();
  for(;matItr!=materialTable->end();matItr++)
  {
    if((*matItr)->GetName()==matName) 
    {
      mat = *matItr;
      break;
    }
  }

  G4int retVal = 0;
  if(!logVol && !mat) 
  { retVal = 3; }
  else if(!logVol)
  { retVal = 1; }
  else if(!mat)
  { retVal = 2; }
  else
  { logVol->SetMaterial(mat); }

  return retVal;
}


