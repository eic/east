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

  // Enable seach paths for component and b-field data
  auto UImanager = G4UImanager::GetUIpointer();
  G4String searchpath = UImanager->GetMacroSearchPath();
  if (searchpath!="") searchpath += ":";
  searchpath += CMAKE_INSTALL_FULL_DATADIR;
  
  // // Also add data from the source directory to the search path
  // // This way, no "make install" should be necessary
  // if (searchpath!="") searchpath += ":";
  // searchpath += G4String(CMAKE_PROJECT_SOURCE_DIR);
  
  UImanager->SetMacroSearchPath(searchpath);
  UImanager->ParseMacroSearchPath();
  if( UImanager->GetVerboseLevel() >0){
    G4cout << " Search path is " << UImanager->GetMacroSearchPath() << G4endl;
  }
  
  if ( argc == 2 ) {
    // execute an argument macro file 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
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

