// ********************************************************************
//
// eASTDetectorComponentGDML.cc
//   eAST Generic Detctor component from gdml file
//
// History
//   October 25, 2021 : first implementation
//
// ********************************************************************

#include "eASTDetectorComponentGDML.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4GDMLParser.hh"
#include "G4GenericMessenger.hh"
#include "G4VisAttributes.hh"

#include "eASTRegionInformation.hh"

eASTDetectorComponentGDML::eASTDetectorComponentGDML(G4String compName, G4int vl, const G4bool validate_gdml)
  : eASTVDetectorComponent(compName,vl),  m_validate_gdml (validate_gdml)
{;}

eASTDetectorComponentGDML::~eASTDetectorComponentGDML()
{
  if(messenger!=nullptr)
  {
    delete messenger;
    messenger = nullptr;
  }
}

void eASTDetectorComponentGDML::SetUp()
{
  messenger = new G4GenericMessenger(this,commandDir,"Commands for the component");
  auto gdmlCmd = messenger->DeclareMethod("gdmlFile",&eASTDetectorComponentGDML::SetGDML,
                  "Define the input GDML file");
  gdmlCmd.SetToBeBroadcasted(false);
  gdmlCmd.SetStates(G4State_PreInit);
  auto matCmd = messenger->DeclareMethod("materialFile",&eASTDetectorComponentGDML::SetMatFile,
                  "Define material definition file");
  matCmd.SetToBeBroadcasted(false);
  matCmd.SetStates(G4State_PreInit);
}

void eASTDetectorComponentGDML::Construct(G4VPhysicalVolume* worldPhys)
{
  if(gdmlFileName == "*NOTDEFINED*")
  {
    G4Exception("eASTDetectorComponentGDML::Construct","eASTDetectorComponentGDML000",
                 FatalException,"GDML file is not specified.");
  }

  G4GDMLParser parser;
  // Support structures will be placed directly to the world volume
  // There is no envelope volume to be defined
  // A region is created for this component and all volumes that consist
  // of this component will share the same region.

  pRegion = G4RegionStore::GetInstance()->FindOrCreateRegion(componentName+"_reg");
  auto regInfo = new eASTRegionInformation(componentName+"_regInfo");
  regInfo->SetDetectorComponentGDML();
  pRegion->SetUserInformation(regInfo);

  // Now we read the GDML contents
  parser.Read(gdmlFileName, m_validate_gdml);
  auto tempEnv = parser.GetWorldVolume();
  auto tempEnvLog = tempEnv->GetLogicalVolume();
  auto tempEnvSolid = tempEnvLog->GetSolid();
  G4int ndaughter = G4int(tempEnvLog->GetNoDaughters());
  auto visAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  for(G4int idaughter=0;idaughter<ndaughter;idaughter++)
  {
    auto daughter = tempEnvLog->GetDaughter(idaughter);
    auto new_rmat = GetRotation();
    if(new_rmat!=G4RotationMatrix())
    {
      auto prmat = const_cast<G4RotationMatrix*>(daughter->GetFrameRotation());
      if(prmat==nullptr) prmat = new G4RotationMatrix();
      *prmat *= new_rmat.inverse();
      daughter->SetRotation(prmat);
    }
    auto tran = daughter->GetObjectTranslation();
    tran += GetLocation();
    daughter->SetTranslation(tran);
    daughter->GetLogicalVolume()->SetVisAttributes(visAtt);
    worldPhys->GetLogicalVolume()->AddDaughter(daughter);
    daughter->SetMotherLogical(worldPhys->GetLogicalVolume());
    daughter->GetLogicalVolume()->SetRegion(pRegion);
    pRegion->AddRootLogicalVolume(daughter->GetLogicalVolume());
  }
  delete tempEnv;
  delete tempEnvLog;
  delete tempEnvSolid;

  if(materialToBeSet)
  { ReadMaterialFile(matFileName); }
}


