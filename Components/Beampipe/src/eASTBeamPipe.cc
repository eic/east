// ********************************************************************
//
// eASTBeamPipe.cc
//   eAST Beam pipe component
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#include "eASTBeamPipe.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Transform3D.hh"
#include "G4LogicalVolume.hh"
#include "G4VSolid.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4GDMLParser.hh"
#include "G4GenericMessenger.hh"
#include "G4VisAttributes.hh"

#include "eASTRegionInformation.hh"

eASTBeamPipe::eASTBeamPipe(G4String compName, G4int vl, const G4bool validate_gdml)
  : eASTVDetectorComponent(compName,vl), m_validate_gdml (validate_gdml)
{;}

eASTBeamPipe::~eASTBeamPipe()
{
  if(messenger!=nullptr)
  {
    delete messenger;
    messenger = nullptr;
  }
}

void eASTBeamPipe::SetUp()
{
  messenger = new G4GenericMessenger(this,commandDir,"Commands for Beampipe component");
  auto envCmd = messenger->DeclareMethod("envelopeGdmlFile",&eASTBeamPipe::SetEnvGDML,
                  "Define the input envelope GDML file");
  envCmd.SetToBeBroadcasted(false);
  envCmd.SetStates(G4State_PreInit);
  auto gdmlCmd = messenger->DeclareMethod("gdmlFile",&eASTBeamPipe::SetGDML,
                  "Define the input GDML file");
  gdmlCmd.SetToBeBroadcasted(false);
  gdmlCmd.SetStates(G4State_PreInit);
  auto matCmd = messenger->DeclareMethod("materialFile",&eASTBeamPipe::SetMatFile,
                  "Define material definition file");
  matCmd.SetToBeBroadcasted(false);
  matCmd.SetStates(G4State_PreInit);
}

void eASTBeamPipe::Construct(G4VPhysicalVolume* worldPhys)
{
  if(gdmlFileName == "*NOTDEFINED*")
  {
    G4Exception("eASTBeamPipe::Construct","eASTBeamPipe000",
                 FatalException,"GDML file is not specified.");
  }

  G4GDMLParser parser;
  G4LogicalVolume* envelopeLog = nullptr;

  if(envGdmlFileName == "*NOTDEFINED*")
  {
    // Local world volume of the input GDML file is used as the envelope
    parser.Read(gdmlFileName, m_validate_gdml);
    auto tempWorld = parser.GetWorldVolume();
    envelopeLog = tempWorld->GetLogicalVolume();
    delete tempWorld;
  }
  else
  {
    // Envelope is read from a separate GDML file
    parser.Read(envGdmlFileName, m_validate_gdml);
    auto tempWorld = parser.GetWorldVolume();
    auto tempWorldLog = tempWorld->GetLogicalVolume();
    auto tempWorldSolid = tempWorldLog->GetSolid();

    if(tempWorldLog->GetNoDaughters()>1)
    {
      G4ExceptionDescription ed;
      ed << "Envelope GDML file <" << envGdmlFileName << "> has more than one volumes.\n"
         << "We cannot use it as an envelope";
      G4Exception("eASTBeamPipe::Construct","eASTBeamPipe001",FatalException,ed);
    }

    auto envPhys = tempWorldLog->GetDaughter(0);
    envelopeLog = envPhys->GetLogicalVolume();
    delete envPhys;
    delete tempWorld;
    delete tempWorldLog;
    delete tempWorldSolid;
    parser.Clear();

    // Now we read the beampipe contents
    parser.Read(gdmlFileName, m_validate_gdml);
    auto tempEnv = parser.GetWorldVolume();
    auto tempEnvLog = tempEnv->GetLogicalVolume();
    auto tempEnvSolid = tempEnvLog->GetSolid();
    G4int ndaughter = G4int(tempEnvLog->GetNoDaughters());
    for(G4int idaughter=0;idaughter<ndaughter;idaughter++)
    {
      auto daughter = tempEnvLog->GetDaughter(idaughter);
      if(daughter->GetName().find("Vacuumvolume")!=std::string::npos)
      {
        // skip vacuum volume
        delete daughter->GetLogicalVolume()->GetSolid();
        delete daughter->GetLogicalVolume();
        delete daughter;
        continue;
      }
      if(daughter->GetName().find("beryllium")!=std::string::npos)
      {
        // paint beryllium pipe red.
        auto visBery = new G4VisAttributes(G4Colour(1.0,0.0,0.0));
        daughter->GetLogicalVolume()->SetVisAttributes(visBery);
      }
      daughter->SetMotherLogical(envelopeLog);
      envelopeLog->AddDaughter(daughter);
    }
    delete tempEnv;
    delete tempEnvLog;
    delete tempEnvSolid;
  }

  envelopeLog->SetName(componentName+"_log");
  auto visAtt = new G4VisAttributes(G4Colour(0.5,0.0,0.0));
  visAtt->SetVisibility(false);
  //visAtt->SetForceWireframe();
  envelopeLog->SetVisAttributes(visAtt);
  envelopeLog->GetSolid()->SetName(componentName+"_solid");

  pRegion = G4RegionStore::GetInstance()->FindOrCreateRegion(componentName+"_reg");
  envelopeLog->SetRegion(pRegion);
  pRegion->AddRootLogicalVolume(envelopeLog);

  auto regInfo = new eASTRegionInformation(componentName+"_regInfo");
  regInfo->SetBeamPipe();
  pRegion->SetUserInformation(regInfo);

  Locate(envelopeLog,worldPhys);

  if(materialToBeSet)
  { ReadMaterialFile(matFileName); }
}


