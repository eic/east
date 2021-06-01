// ********************************************************************
//
// eASTDetectorConstructionMessenger.cc
//   A messenger class that handles geometry configuration.
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#include "eASTDetectorConstructionMessenger.hh"

#include "eASTDetectorConstruction.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"

eASTDetectorConstructionMessenger::eASTDetectorConstructionMessenger(eASTDetectorConstruction* dc)
: pDC(dc)
{
  G4UIparameter* para = nullptr;

  geomDir = new G4UIdirectory("/eAST/geometry/");
  geomDir->SetGuidance("eAST geometry commands");

  listSolidCmd = new G4UIcmdWithAnInteger("/eAST/geometry/listSolids",this);
  listSolidCmd->SetGuidance("List all the registered solids");
  listSolidCmd->SetParameterName("level",true);
  listSolidCmd->SetDefaultValue(0);
  listSolidCmd->SetRange("level>=0 && level<=2");
  listSolidCmd->AvailableForStates(G4State_Init,G4State_Idle);
  listSolidCmd->SetToBeBroadcasted(false);

  listLogVolCmd = new G4UIcmdWithAnInteger("/eAST/geometry/listLogicalVolumes",this);
  listLogVolCmd->SetGuidance("List all the registered logical volumes");
  listLogVolCmd->SetParameterName("level",true);
  listLogVolCmd->SetDefaultValue(0);
  listLogVolCmd->SetRange("level>=0 && level<=2");
  listLogVolCmd->AvailableForStates(G4State_Init,G4State_Idle);
  listLogVolCmd->SetToBeBroadcasted(false);

  listPhysVolCmd = new G4UIcmdWithAnInteger("/eAST/geometry/listPhysicalVolumes",this);
  listPhysVolCmd->SetGuidance("List all the registered physical volumes");
  listPhysVolCmd->SetParameterName("level",true);
  listPhysVolCmd->SetDefaultValue(0);
  listPhysVolCmd->SetRange("level>=0 && level<=2");
  listPhysVolCmd->AvailableForStates(G4State_Init,G4State_Idle);
  listPhysVolCmd->SetToBeBroadcasted(false);

  listRegionCmd = new G4UIcmdWithAnInteger("/eAST/geometry/listRegions",this);
  listRegionCmd->SetGuidance("List all the registered regions");
  listRegionCmd->SetParameterName("level",true);
  listRegionCmd->SetDefaultValue(0);
  listRegionCmd->SetRange("level>=0 && level<=2");
  listRegionCmd->AvailableForStates(G4State_Init,G4State_Idle);
  listRegionCmd->SetToBeBroadcasted(false);

  //// This command is fragile for large-scale geometry - maybe temporally disabled
  checkOverlapCmd = new G4UIcommand("/eAST/geometry/checkOverlap",this);
  checkOverlapCmd->SetGuidance("Check volume overlap with existing volumes");
  checkOverlapCmd->SetGuidance(" i.e. with mother volume for protrusion and with other siblings for overlap.");
  checkOverlapCmd->SetGuidance(" - This command is valid only for placement and parameterized volumes. If this command is");
  checkOverlapCmd->SetGuidance("   used for other physical volume type, e.g. replica, command will be simply ignored.");
  checkOverlapCmd->SetGuidance("   If \"**ALL**\" is used as the volume name, all physical volumes are examined (SLOW!!).");
  checkOverlapCmd->SetGuidance(" - nSpots specifies number of spots on the surface of the volume to be examined.");
  checkOverlapCmd->SetGuidance("   The more spots used, the more chances to detect overlaps, but the more time it takes.");
  checkOverlapCmd->SetGuidance(" - maxErr specifies maximum number of errors to be generated (default 1) before quiting.");
  para = new G4UIparameter("physVol",'s',true);
  para->SetDefaultValue("**ALL**");
  checkOverlapCmd->SetParameter(para);
  para = new G4UIparameter("nSpots",'i',true);
  para->SetDefaultValue(1000);
  para->SetGuidance("Number of trial spots on the volume surface");
  checkOverlapCmd->SetParameter(para);
  para = new G4UIparameter("maxErr",'i',true);
  para->SetDefaultValue(1);
  para->SetParameterRange("maxErr > 0");
  para->SetGuidance("Maxinum number of report to be generated");
  checkOverlapCmd->SetParameter(para);
  para = new G4UIparameter("tolerance",'d',true);
  para->SetDefaultValue(0.);
  para->SetParameterRange("tolerance >= 0.");
  para->SetGuidance("Tolerance (default 0.)");
  checkOverlapCmd->SetParameter(para);
  para = new G4UIparameter("unit",'s',true);
  para->SetDefaultUnit("mm");
  checkOverlapCmd->SetParameter(para);
  checkOverlapCmd->AvailableForStates(G4State_Idle);
  checkOverlapCmd->SetToBeBroadcasted(false);
  
  materialDir = new G4UIdirectory("/eAST/material/");
  materialDir->SetGuidance("eAST material commands");

  listMatCmd = new G4UIcmdWithAString("/eAST/material/list",this);
  listMatCmd->SetGuidance("List material property");
  listMatCmd->SetGuidance(" If material name is not specified, this command list all registered materials");
  listMatCmd->SetParameterName("matName",true);
  listMatCmd->SetDefaultValue("**ALL**");
  listMatCmd->AvailableForStates(G4State_Init,G4State_Idle);
  listMatCmd->SetToBeBroadcasted(false);

  dumpMatCmd = new G4UIcmdWithoutParameter("/eAST/material/dumpNistMaterials",this);
  dumpMatCmd->SetGuidance("List all pre-defined material names in G4NistManager.");
  dumpMatCmd->SetGuidance(" Note : a material has to be instantiated with /eAST/material/create before setting it to a logical volume");
  dumpMatCmd->AvailableForStates(G4State_Init,G4State_Idle);
  dumpMatCmd->SetToBeBroadcasted(false);

  createMatCmd = new G4UIcmdWithAString("/eAST/material/create",this);
  createMatCmd->SetGuidance("Instantiate a material defined in G4NistManager");
  createMatCmd->SetGuidance(" If the material has already existed, this command does nothing.");
  createMatCmd->SetParameterName("matName",false);
  createMatCmd->AvailableForStates(G4State_Init,G4State_Idle);
  createMatCmd->SetToBeBroadcasted(false);

  getMatCmd = new G4UIcmdWithAString("/eAST/material/show",this); 
  getMatCmd->SetGuidance("Show the current material of the specified logical volume");
  getMatCmd->SetParameterName("logVol",false);
  getMatCmd->AvailableForStates(G4State_Init,G4State_Idle);
  getMatCmd->SetToBeBroadcasted(false);

  setMatCmd = new G4UIcommand("/eAST/material/set",this);
  setMatCmd->SetGuidance("Set the material to the logical volume. The material has to be instantiated in advance.");
  setMatCmd->SetGuidance("  [usage] /eAST/material/set logicalVolumeName materialName");
  para = new G4UIparameter("logVol",'s',false);
  setMatCmd->SetParameter(para);
  para = new G4UIparameter("matName",'s',false);
  setMatCmd->SetParameter(para);
  setMatCmd->AvailableForStates(G4State_Init,G4State_Idle);
  setMatCmd->SetToBeBroadcasted(false);
}

eASTDetectorConstructionMessenger::~eASTDetectorConstructionMessenger()
{
  delete listSolidCmd;
  delete listLogVolCmd;
  delete listPhysVolCmd;
  delete listRegionCmd;
  delete checkOverlapCmd;
  delete geomDir;

  delete listMatCmd;
  delete dumpMatCmd;
  delete createMatCmd;
  delete getMatCmd;
  delete setMatCmd;
  delete materialDir;
}

#include "G4Tokenizer.hh"

void eASTDetectorConstructionMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  if(cmd==listSolidCmd)
  { pDC->ListSolids(listSolidCmd->GetNewIntValue(val)); }
  else if(cmd==listLogVolCmd)
  { pDC->ListLogVols(listLogVolCmd->GetNewIntValue(val)); }
  else if(cmd==listPhysVolCmd)
  { pDC->ListPhysVols(listPhysVolCmd->GetNewIntValue(val)); }
  else if(cmd==listRegionCmd)
  { pDC->ListRegions(listRegionCmd->GetNewIntValue(val)); }
  else if(cmd==checkOverlapCmd)
  {
    G4Tokenizer next(val);
    G4String physVolName = next();
    G4int nSpots = StoI(next());
    G4int maxErr = StoI(next());
    G4String tolStr = next();
    G4double tol = StoD(tolStr);
    if(tol>0.)
    {
      tolStr += " ";
      tolStr += next();
      tol = checkOverlapCmd->ConvertToDimensionedDouble(tolStr);
    }
    auto valid = pDC->CheckOverlap(physVolName,nSpots,maxErr,tol);
    if(!valid)
    {
      G4ExceptionDescription ed;
      ed << "Physical volume <" << physVolName << "> is not defined. Command ignored.";
      cmd->CommandFailed(ed);
    }
  }

  else if(cmd==listMatCmd)
  { 
    if(val=="**ALL**") 
    { pDC->ListAllMaterial(); }
    else
    {
      auto valid = pDC->ListMaterial(val);
      if(!valid)
      {
        G4ExceptionDescription ed;
        ed << "<" << val << "> is not defined. If necessary, create it with /eAST/material/create command.";
        cmd->CommandFailed(ed);
      }
    }
  }
  else if(cmd==dumpMatCmd)
  { pDC->DumpNistMaterials(); }
  else if(cmd==createMatCmd)
  { 
    auto valid = pDC->CreateMaterial(val); 
    if(!valid)
    {
      G4ExceptionDescription ed;
      ed << "The material name <" << val << "> is not defined in G4NistManager.";
      cmd->CommandFailed(ed);
    }
  }
  else if(cmd==getMatCmd)
  {
    auto valid = pDC->GetMaterial(val);
    if(!valid)
    {
      G4ExceptionDescription ed;
      ed << "<" << val << "> is not a name of registered logical volume.\n"
         << "Check existing logical volumes with /eAST/geometry/listLogicalVolumes command.";
      cmd->CommandFailed(ed);
    }
  }
  else if(cmd==setMatCmd)
  {
    G4Tokenizer next(val);
    G4String logVolName = next();
    G4String matName = next();
    auto valid = pDC->SetMaterial(logVolName,matName);
    if(valid!=0)
    {
      G4ExceptionDescription ed;
      if(valid==1 || valid==3)
      {
        ed << "<" << logVolName << "> is not a name of registered logical volume.\n"
           << "Check existing logical volumes with /eAST/geometry/listLogicalVolumes command.\n";
      }
      if(valid==2 || valid==3)
      {
        ed << "<" << matName << "> is not defined. If necessary, create it with /eAST/material/create command.";
      }
      cmd->CommandFailed(ed);
    }
  }

}

G4String eASTDetectorConstructionMessenger::GetCurrentValue(G4UIcommand* /*cmd*/)
{
  G4String val("");
  return val;
}


