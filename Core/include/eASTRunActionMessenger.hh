// ********************************************************************
//
// eASTRunActionMessenger.hh
//   Header file of a messenger class that handles histograms and
//   n-tuple in eASTRunAction.
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTRunActionMessenger_H
#define eASTRunActionMessenger_H 1

#include "G4UImessenger.hh"
#include "globals.hh"

class eASTRunAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithoutParameter;

class eASTRunActionMessenger: public G4UImessenger
{
  public:
    eASTRunActionMessenger(eASTRunAction*);
    virtual ~eASTRunActionMessenger();
    virtual void SetNewValue(G4UIcommand*,G4String);
    virtual G4String GetCurrentValue(G4UIcommand*);

  private:
    void Define1D();
    void Define1P();

  private:
    eASTRunAction* pRA;

    G4UIdirectory* 		anaDir;
    G4UIcmdWithAString*         fileCmd;
    G4UIcmdWithAnInteger*	verboseCmd;
    G4UIcmdWithoutParameter*    listCmd;
    G4UIcmdWithAnInteger*	openCmd;
    G4UIcmdWithAnInteger*       plotCmd;
    G4UIcmdWithABool*           carryCmd;
    G4UIcmdWithoutParameter*    flushCmd;
    G4UIcmdWithoutParameter*    resetCmd;

    G4UIdirectory*              oneDDir;
    G4UIcommand*                create1DCmd;
    G4UIcommand*                create1DPlotPCmd;
    G4UIcommand*                config1DCmd;
    G4UIcommand*                title1DCmd;
    G4UIcmdWithABool*           set1DYaxisLogCmd;

    G4UIdirectory*              onePDir;
    G4UIcommand*                create1PCmd;
    G4UIcommand*                set1PCmd;
    G4UIcommand*                title1PCmd;

    G4UIdirectory*              ntupleDir;
    G4UIcommand*                addColumnCmd;

  private:
    G4int currentID;

  private:
    inline G4bool CheckID(G4UIcommand* cmd)
    {
      if(currentID<0)
      {
        G4ExceptionDescription ed;
        ed << "There is no currently opened histogram. Create or open one.";
        cmd->CommandFailed(ed);
        return false;
      }
      return true;
    }
    inline G4bool CheckOpenID(G4UIcommand* /*cmd*/)
    {
      if(currentID>=0)
      {
        G4cout << "Previously opened histogram <" << currentID << "> is closed." << G4endl;
        currentID = -1;
      }
      return true;
    }
};

#endif

