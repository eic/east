// ********************************************************************
//
// eASTVDetectorComponent.cc
//   the abstract base class of eAST detector component
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#include "eASTVDetectorComponent.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Region.hh"
#include "eASTDetectorComponentMessenger.hh"
#include "eASTDetectorConstruction.hh"
#include "eASTUserActionDispatcher.hh"
#include "G4UImanager.hh"
#include "G4Tokenizer.hh"
#include "G4Version.hh"
#include <fstream>

eASTVDetectorComponent::eASTVDetectorComponent(G4String compName, G4int vl)
: componentName(compName), verboseLevel(vl)
{
  baseMessenger = new eASTDetectorComponentMessenger(this,compName);
  commandDir = "/eAST/component/" + compName + "/";
  userActionDispatcher = eASTUserActionDispatcher::Instance();
}

eASTVDetectorComponent::~eASTVDetectorComponent()
{ delete baseMessenger; }

void eASTVDetectorComponent::SetUpBase(G4int vl)
{
  verboseLevel = vl;
  eASTDetectorConstruction::Instance()->RegisterComponent(componentName,this);
  SetUp();
  baseMessenger->SetUpBaseCommands(commandDir);
}

void eASTVDetectorComponent::Locate(G4LogicalVolume* compLogVol, 
                G4VPhysicalVolume* worldPhys)
{
  pEnvelopePhys = new G4PVPlacement(G4Transform3D(fRotation,fPosition),compLogVol,
                        componentName+"_phys",worldPhys->GetLogicalVolume(),0,0,0);
}

G4String eASTVDetectorComponent::LocateDataFile( const G4String fn ) const
{
  if ( verboseLevel > 0 )  G4cout << "LocateDataFile: Searching for " << fn
				  << " in " << G4UImanager::GetUIpointer()->GetMacroSearchPath()
				  << G4endl;

  if(fn == "*NOTDEFINED*"){
    // Do nothing, let implementations decide
    if ( verboseLevel > 0 )  G4cout << "LocateDataFile: Returning " << fn << G4endl;
    return fn;
  }
  
  auto UImanager = G4UImanager::GetUIpointer();
  auto ret = UImanager->FindMacroPath(fn);
  if ( verboseLevel > 0 ) G4cout << "LocateDataFile: Returning " << ret << G4endl;
  return ret;
}

void eASTVDetectorComponent::ReadMaterialFile(G4String fileName)
{
  enum { BUFSIZE = 128 };
  char line[BUFSIZE];
  std::ifstream matFile;
  matFile.open(fileName,std::ios::in);
  if(matFile.fail())
  {
    G4ExceptionDescription ed;
    ed << "Material definition file <" << fileName << "> couldn't be opened.";
    G4Exception("eASTBeamPipe::Construct","eASTBeamPipe001",
                JustWarning,ed);
  }
  else
  {
    if(verboseLevel>0)
    { G4cout << "Reading material definition file <" << fileName << ">..." << G4endl;}
    while(!(matFile.eof()))
    {
      matFile.getline(line,BUFSIZE);
      if(line[0]=='#') continue;
      G4Tokenizer next(line);
      G4String logVolName = next();
      G4String matName = next();
#if G4VERSION_NUMBER < 1100
      if(logVolName.isNull() || matName.isNull()) continue;
#else
      if(logVolName.empty() || matName.empty()) continue;
#endif
      G4String cmd = "/eAST/material/create " + matName;
      G4UImanager::GetUIpointer()->ApplyCommand(cmd);
      cmd = "/eAST/material/set " + logVolName + " " + matName;
      G4UImanager::GetUIpointer()->ApplyCommand(cmd);
    }
  }
  matFile.close();
}

void eASTVDetectorComponent::RegisterUserAction(G4UserRunAction* ua)
{ if(CheckRegion()) userActionDispatcher->RegisterUserAction(pRegion,ua); }

void eASTVDetectorComponent::RegisterUserAction(G4UserEventAction* ua)
{ if(CheckRegion()) userActionDispatcher->RegisterUserAction(pRegion,ua); }

void eASTVDetectorComponent::RegisterUserAction(G4UserStackingAction* ua)
{ if(CheckRegion()) userActionDispatcher->RegisterUserAction(pRegion,ua); }

void eASTVDetectorComponent::RegisterUserAction(G4UserTrackingAction* ua)
{ if(CheckRegion()) userActionDispatcher->RegisterUserAction(pRegion,ua); }

void eASTVDetectorComponent::RegisterUserAction(G4UserSteppingAction* ua)
{ if(CheckRegion()) pRegion->SetRegionalSteppingAction(ua); }

G4bool eASTVDetectorComponent::CheckRegion()
{
  if(pRegion==nullptr)
  {
    G4ExceptionDescription ed;
    ed << "Component <" << componentName << "> does not have a valid G4Region pointer.";
    G4Exception("eASTVDetectorComponent::RegisterUserAction()","eASTComp0001",
                FatalException,ed);
    return false;
  }
  return true;
}


