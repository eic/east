// ********************************************************************
//
//  eAST (eA Simulation Tool)
//
// eAST.cc - Main function of eAST
//
// History:
//   April 28, 2021 - first implementation (M.Asai/SLAC)
//
// ********************************************************************

#include "G4Types.hh"

#include "G4RunManagerFactory.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#include "eAInitialization.hh"

int main(int argc,char** argv)
{
  // Construct Run Manager
  auto runManager = G4RunManagerFactory::CreateRunManager();

  // construct Gorad initializer
  auto eastInitialization = new eAInitialization();

  // Visualization manager construction
  auto visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  if ( argc == 2 ) {
    // execute an argument macro file 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    auto UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // start interactive session
    auto ui = new G4UIExecutive(argc, argv); 
    eastInitialization->SetWindowText(ui);
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  delete eastInitialization;
  delete visManager;
  delete runManager;
}

