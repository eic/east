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
#include "G4UIcmdWithAnInteger.hh"

eASTDetectorComponentMessenger::eASTDetectorComponentMessenger
    (eASTVDetectorComponent* dc, G4String compName)
: pDC(dc)
{
  G4String comName = "/eAST/component/";
  comName += compName;

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

eASTDetectorComponentMessenger::~eASTDetectorComponentMessenger()
{
  delete setupCmd;
}

void eASTDetectorComponentMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  if(cmd==setupCmd)
  { pDC->SetUpBase(setupCmd->GetNewIntValue(val)); }
}

G4String eASTDetectorComponentMessenger::GetCurrentValue(G4UIcommand* /*cmd*/)
{
  G4String val("");
  return val;
}


