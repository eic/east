// ********************************************************************
//
// eASTPrimGenActionMessenger.cc
//   messenger class that sets primary vertex parameters
//
// History
//   Dec 28th, 2021 : first implementation - M. Asai (JLab)
//
// ********************************************************************

#include "eASTPrimGenActionMessenger.hh"

#include "eASTPrimaryGeneratorAction.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UnitsTable.hh"

eASTPrimGenActionMessenger::eASTPrimGenActionMessenger(eASTPrimaryGeneratorAction* pg)
: pPG(pg)
{
  G4UIparameter* para = nullptr;

  // note: /eAST/generator/ directory has already been created 

  setTimeCmd = new G4UIcommand("/eAST/generator/setTime",this);
  setTimeCmd->SetGuidance("Set the start time of each event");
  setTimeCmd->SetGuidance(" t = deltaT * eventID + T0");
  para = new G4UIparameter("deltaT",'d',false);
  setTimeCmd->SetParameter(para);
  para = new G4UIparameter("T0",'d',false);
  setTimeCmd->SetParameter(para);
  para = new G4UIparameter("unit",'s',true);
  para->SetDefaultUnit("microsecond");
  setTimeCmd->SetParameter(para);
  setTimeCmd->AvailableForStates(G4State_Init,G4State_Idle);
}

eASTPrimGenActionMessenger::~eASTPrimGenActionMessenger()
{
  delete setTimeCmd;
}

#include "G4Tokenizer.hh"

void eASTPrimGenActionMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  if(cmd==setTimeCmd)
  {
    G4Tokenizer next(val);
    G4double deltaT = StoD(next());
    G4double T0 = StoD(next());
    G4double uVal = G4UIcommand::ValueOf(next());
    pPG->SetT(deltaT*uVal,T0*uVal);
  }
}

G4String eASTPrimGenActionMessenger::GetCurrentValue(G4UIcommand* cmd)
{
  G4String val("");
  if(cmd==setTimeCmd)
  {
    G4double deltaT = 0.;
    G4double T0 = 0.;
    pPG->GetT(deltaT,T0);
    std::ostringstream os;
    os << "deltaT = " << G4BestUnit(deltaT,"Time") << "  T0 = " << G4BestUnit(T0,"Time");
    val += os.str();
  }
  return val;
}


