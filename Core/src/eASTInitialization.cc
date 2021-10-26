// ********************************************************************
//
// eASTInitialization.cc
//   Defines the initialization procedure of Geant4 and eAST
//
// History
//   April 28, 2021 - first implementation (M.Asai/SLAC)
//
// ********************************************************************

#include "eASTInitialization.hh"

#include "eASTDetectorConstruction.hh"
#include "eASTPhysicsList.hh"
#include "eASTMagneticField.hh"
//#include "FTFP_BERT.hh"
#include "eASTActionInitialization.hh"
#include "G4GenericMessenger.hh"

#include "G4RunManager.hh"
#include "G4ScoringManager.hh"
#include "G4UIdirectory.hh"
#include "G4UnitsTable.hh"
#include <CLHEP/Units/SystemOfUnits.h>

#include "eASTBeamPipe.hh"
#include "eASTSupportStructure.hh"
#include "eASTDetectorComponentGDML.hh"

eASTInitialization::eASTInitialization(G4int verboseLvl)
: verboseLevel(verboseLvl)
{
  // adding units
  new G4UnitDefinition("milligray","mGy","Dose",1.e-3*CLHEP::gray);
  new G4UnitDefinition("microgray","muGy","Dose",1.e-6*CLHEP::gray);
  new G4UnitDefinition("nanogray","nGy","Dose",1.e-9*CLHEP::gray);

  //G4ScoringManager::GetScoringManager()->SetScoreWriter(new eASTScoreWriter());

  messenger = new G4GenericMessenger(this,"/eAST/","eAST commands");
  auto& initCmd = messenger->DeclareMethod("initialize",
         &eASTInitialization::Initialize,"Initialize G4RunManager and eAST");
  initCmd.SetToBeBroadcasted(false);
  initCmd.SetStates(G4State_PreInit);

  auto compDir = new G4UIdirectory("/eAST/component/");
  compDir->SetGuidance("Activate a component of EIC detector");

  detector = new eASTDetectorConstruction();
  physics = new eASTPhysicsList();
  field = new eASTMagneticField();

  detector->SetMagneticField(field);

  actionInitialization = new eASTActionInitialization();

  // EIC Detector Components
  new eASTBeamPipe("beampipe");
  new eASTSupportStructure("DIRC_support");
  new eASTSupportStructure("EM_CAL_support");

  new eASTDetectorComponentGDML("GenericGDML");

}

eASTInitialization::~eASTInitialization()
{
  delete messenger; 
}

void eASTInitialization::Initialize()
{
  auto runManager = G4RunManager::GetRunManager();
  runManager->SetUserInitialization(detector);
  runManager->SetUserInitialization(physics);
  runManager->SetUserInitialization(actionInitialization);
  if(verboseLevel>0) G4cout << "initializing eAST .........." << G4endl;
  runManager->Initialize();
}

#include "G4UIExecutive.hh"
#ifdef G4UI_USE_QT
#include "G4UIQt.hh"
#endif

void eASTInitialization::SetWindowText(G4UIExecutive* ui)
{
  // If the current GUI is not G4UIQt, do nothing and return.
  if(!(ui->IsGUI())) return;

#ifdef G4UI_USE_QT
  G4UIQt* qt = dynamic_cast<G4UIQt*>(ui->GetSession());
  if(!qt) return;

  qt->SetStartPage(std::string("<table width='100%'><tr><td width='50%'></td><td><div ")+
       "style='color: rgb(140, 31, 31); font-size: xx-large; font-family: Garamond, serif; "+
       "padding-bottom: 0px; font-weight: normal'>eAST "+
       "</div></td></td></tr></table>"+
       "<p>&nbsp;</p>"+
       "<div><dl>"+
         "<dd><b>eAST (eA Simulation Tool) is meant to be "+
         "a common tool for detector simulation studies for Electron-Ion Collider experiments "+
         "built on top of Geant4."+
         "</dd></dl></div>"+
       "<p>&nbsp;</p>"+
       "<div style='background:#EEEEEE;'><b>Tooltips :</b><ul>"+
         "<li><b>Start an interactive run :</b><br />"+
         "/control/execute <i>run.mac</i><br />"+
         "/run/beamOn <i>number_of_events</i></li></ul></div>"+
       "<div style='background:#EEEEEE;'><b>Documentation :</b><ul>"+
         "<li><i>"+
         "<b>eAST manual</b> is "+
         "<a href='https://eic.github.io/east/content/manual.html'>"+
         "here"+
         "</a>.</i></li>"+
         "</ul></div>"
       );
#endif
}

