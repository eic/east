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
#include "G4Region.hh"
#include "eASTDetectorComponentMessenger.hh"
#include "eASTDetectorConstruction.hh"
#include "G4UImanager.hh"
#include "G4Tokenizer.hh"
#include <fstream>

eASTVDetectorComponent::eASTVDetectorComponent(G4String compName, G4int vl)
: componentName(compName), verboseLevel(vl)
{
  baseMessenger = new eASTDetectorComponentMessenger(this,compName);
  commandDir = "/eAST/component/" + compName + "/";
}

eASTVDetectorComponent::~eASTVDetectorComponent()
{ delete baseMessenger; }

void eASTVDetectorComponent::SetUpBase(G4int vl)
{
  verboseLevel = vl;
  eASTDetectorConstruction::Instance()->RegisterComponent(componentName,this);
  SetUp();
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
      if(logVolName.isNull() || matName.isNull()) continue;
      G4String cmd = "/eAST/material/create " + matName;
      G4UImanager::GetUIpointer()->ApplyCommand(cmd);
      cmd = "/eAST/material/set " + logVolName + " " + matName;
      G4UImanager::GetUIpointer()->ApplyCommand(cmd);
    }
  }
  matFile.close();
}

