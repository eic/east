// ********************************************************************
//
// eASTRunAction.hh
//   Header file of eAST Run Action class that takes care of
//   handling histograms and n-tuple.
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTRunAction_h
#define eASTRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class eASTRunActionMessenger;
class eASTRunAction;
#include "eASTRun.hh"

#include <map>
class G4VPrimitivePlotter;

class eASTHistoType
{
  friend class eASTRunAction;
  friend class eASTRun;
  private:
    eASTHistoType()
    {;}
  private:
    G4int histID = -1;
    G4int histType = -1;
    G4int histDup = 1;

    G4int collID = -1;
    G4String meshName = "dummy";
    G4String primName = "dummy";
    G4int idx = -1;

    G4int collID2 = -1;
    G4String meshName2 = "dummy";
    G4String primName2 = "dummy";
    G4int idx2 = -1;

    G4int biasf = 0;
    G4double fuct = 1.;
    G4VPrimitivePlotter* pplotter = nullptr;
};
    
class eASTRunAction : public G4UserRunAction
{
  friend class eASTRun;
  public:
    eASTRunAction();
    virtual ~eASTRunAction();

    virtual G4Run* GenerateRun()
    { return new eASTRun(this); }
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:
    eASTRunActionMessenger* messenger;

  public:
    void SetVerbose(G4int);
    void ListHistograms();
    G4bool Open(G4int);
    G4bool SetAllPlotting(G4bool val=true);
    G4bool SetPlotting(G4int,G4bool val=true);
    void Flush();
    void Reset();

    G4int Create1D(G4String&,G4String&,G4int);
    G4int Create1DForPlotter(G4String&,G4String&,G4bool);
    G4bool Set1D(G4int,G4int,G4double,G4double,G4String&,G4String&,G4bool);
    G4bool Set1DTitle(G4int,G4String&,G4String&,G4String&);
    G4bool Set1DYAxisLog(G4int,G4bool);

    G4int Create1P(G4String&,G4String&,G4int);
    G4bool Set1P(G4int,G4double,G4double,G4String&,G4String&,G4String&,G4String&);
    G4bool Set1PTitle(G4int,G4String&,G4String&,G4String&);

    G4int NtupleColumn(G4String&,G4String&,G4String&,G4int);

  private:
    void OpenFile();
    void DefineNTColumn();
    void MergeNtuple();

  public:
    inline void SetFileName(G4String& fn)
    { fileName = fn; }
    inline const G4String& GetFileName() const
    { return fileName; }
    inline G4int GetVerbose() const
    { return verbose; }
    inline void SetCarry(G4bool val = true)
    { ifCarry = val; }
    inline G4bool GetCarry() const
    { return ifCarry; }

  private:
    G4String fileName = "eASTOut";
    G4bool fileOpen = false;
    G4int verbose = 0;
    G4bool ifCarry = false;
    G4int id_offset = 100;
    G4int id_factor = 100;

  private:
    std::map<G4int,eASTHistoType*> IDMap;
    std::map<G4int,eASTHistoType*> NTMap;
};

#endif
