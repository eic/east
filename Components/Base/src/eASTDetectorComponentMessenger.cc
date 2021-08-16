// ********************************************************************
//
// eASTDetectorComponentMessenger.cc
//   A messenger class that handles geometry configuration.
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#include "eASTDetectorComponentMessenger.hh"

#include "eASTVDetectorComponent.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4Tokenizer.hh"

eASTDetectorComponentMessenger::eASTDetectorComponentMessenger
    (eASTVDetectorComponent* dc, G4String compName)
: pDC(dc)
{
  G4String comName = "/eAST/component/"+compName;
  setupCmd = new G4UIcmdWithAnInteger(comName,this);
  G4String msg = "Activate ";
  msg += compName;
  setupCmd->SetGuidance(msg);
  setupCmd->SetParameterName("verboselevel",true);
  setupCmd->SetDefaultValue(0);
  setupCmd->SetRange("verboselevel>=0");
  setupCmd->AvailableForStates(G4State_PreInit);
  setupCmd->SetToBeBroadcasted(false);
}

void eASTDetectorComponentMessenger::SetUpBaseCommands(G4String comDir)
{
  G4String com = comDir+"locate";
  locCmd = new G4UIcmdWith3VectorAndUnit(com,this);
  locCmd->SetGuidance("Locate the component in the global coordinate.");
  locCmd->SetParameterName("x","y","z",false);
  locCmd->SetDefaultUnit("mm");
  locCmd->AvailableForStates(G4State_PreInit);
  locCmd->SetToBeBroadcasted(false);

  com = comDir+"rotate";
  rotCmd = new G4UIcommand(com,this);
  rotCmd->SetGuidance("Rotate the component in the global coordinate.");
  auto param = new G4UIparameter("axis",'s',false);
  param->SetParameterCandidates("x y z");
  rotCmd->SetParameter(param);
  param = new G4UIparameter("angle",'d',false);
  rotCmd->SetParameter(param);
  param = new G4UIparameter("unit",'s',true);
  param->SetDefaultUnit("deg");
  rotCmd->SetParameter(param);
  rotCmd->AvailableForStates(G4State_PreInit);
  rotCmd->SetToBeBroadcasted(false);
}

eASTDetectorComponentMessenger::~eASTDetectorComponentMessenger()
{
  delete locCmd;
  delete rotCmd;
  delete setupCmd;
}

void eASTDetectorComponentMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  if(cmd==setupCmd)
  { pDC->SetUpBase(setupCmd->GetNewIntValue(val)); }
  else if(cmd==locCmd)
  { pDC->SetLocation(locCmd->GetNew3VectorValue(val)); }
  else if(cmd==rotCmd)
  {
    G4Tokenizer next(val);
    G4String ax = next();
    G4double ang = StoD(next()) * G4UIcommand::ValueOf(next());
    pDC->SetRotation(ax,ang);
  }
}

G4String eASTDetectorComponentMessenger::GetCurrentValue(G4UIcommand* /*cmd*/)
{
  G4String val("");
  return val;
}


