// ********************************************************************
//
// eASTRunAction.cc
//   eAST Run Action class that takes care of defining and handling
//   histograms and n-tuple.
//   Filling histograms is taken care by eASTRun class.
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#include "eASTRunAction.hh"
#include "eASTRunActionMessenger.hh"
#include "eASTAnalysis.hh"
#include "eASTUserActionDispatcher.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

eASTRunAction::eASTRunAction()
{ 
  messenger = new eASTRunActionMessenger(this);
  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  analysisManager->SetVerboseLevel(verbose);
  //analysisManager->SetNtupleMerging(true);
}

eASTRunAction::~eASTRunAction()
{
  delete G4AnalysisManager::Instance();  
  delete messenger;
}

void eASTRunAction::BeginOfRunAction(const G4Run* run)
{ 
  // Open an output file
  //
  OpenFile();

  // Define nTuple column if needed
  //
  DefineNTColumn();

  // Invoke component-specific run actions
  //
  auto runActions = eASTUserActionDispatcher::Instance()->GetRunActions();
  if((runActions!=nullptr) && !(runActions->empty()))
  {
    for(auto ua : *runActions)
    { ua.second->BeginOfRunAction(run); }
  }
}

void eASTRunAction::OpenFile()
{
  if(!fileOpen)
  {
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile(fileName);
    if(verbose>0) G4cout << "eASTRunAction::BeginOfRunAction ### <" << fileName << "> is opened." << G4endl;
    fileOpen = true;
  }
}

void eASTRunAction::EndOfRunAction(const G4Run* run)
{
  if(!ifCarry) Flush();

  // Invoke component-specific run actions
  //
  auto runActions = eASTUserActionDispatcher::Instance()->GetRunActions();
  if((runActions!=nullptr) && !(runActions->empty()))
  {
    for(auto ua : *runActions)
    { ua.second->EndOfRunAction(run); }
  }
}

void eASTRunAction::Flush()
{
  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
  if(verbose>0) G4cout << "eASTRunAction::Flush ### <" << fileName << "> is closed." << G4endl;

  fileOpen = false;

  if(IsMaster()) MergeNtuple();
}

void eASTRunAction::SetVerbose(G4int val)
{
  verbose = val;
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(verbose);
}

void eASTRunAction::ListHistograms()
{
  G4cout << "################## registered histograms/plots" << G4endl;
  G4cout << "id\thistID\thistType\tdetName-X\tpsName-X\tcollID-X\tcopyNo-X\tdetName-Y\tpsName-Y\tcollID-Y\tcopyNo-Y" << G4endl;
  for(auto itr : IDMap)
  {
    G4cout << itr.first << "\t" << itr.second->histID << "\t";
    if(itr.second->histType==1) // 1D histogram
    { G4cout << "1-D hist\t" << itr.second->meshName << "\t" << itr.second->primName << "\t" << itr.second->collID << "\t" << itr.second->idx; }
    else if(itr.second->histType==2) // 1D profile
    { G4cout << "1-D prof\t" << itr.second->meshName << "\t" << itr.second->primName << "\t" << itr.second->collID; }
    G4cout << G4endl;
  }
}

G4bool eASTRunAction::Open(G4int id)
{
  auto hItr = IDMap.find(id);
  return (hItr!=IDMap.end());
}

#include "G4SDManager.hh"
using namespace G4Analysis;

G4bool eASTRunAction::SetAllPlotting(G4bool val)
{
  G4bool valid = true;
  for(auto hItr : IDMap)
  { 
    valid = SetPlotting(hItr.first,val);
    if(!valid) break;
  }
  return valid;
}

G4bool eASTRunAction::SetPlotting(G4int id,G4bool val)
{
  auto hItr = IDMap.find(id);
  if(hItr==IDMap.end()) return false;
  auto ht = hItr->second;
  auto hTyp = ht->histType;
  auto analysisManager = G4AnalysisManager::Instance();
  if(hTyp==1) // 1D histogram
  { analysisManager->SetH1Plotting(ht->histID,val); }
  else if(hTyp==2) // 1D profile
  { analysisManager->SetP1Plotting(ht->histID,val); }
  else
  { return false; }
  return true;
}

// ------------- 1D histogram

G4int eASTRunAction::Create1D(G4String& mName,G4String& pName,G4int cn)
{
  G4String collName = mName;
  collName += "/";
  collName += pName;
  auto cID = G4SDManager::GetSDMpointer()->GetCollectionID(collName);
  if(cID<0) return cID;

  G4int id = (cID+id_offset)*id_factor+cn+1;
  auto histoTypeItr = IDMap.find(id);
  if(histoTypeItr!=IDMap.end()) return false;
  if(verbose) G4cout << "eASTRunAction::Create1D for <" << collName
                     << ", copyNo=" << cn << "> is registered for hitCollectionID "
                     << cID << G4endl;
  
  auto histTyp = new eASTHistoType;
  histTyp->collID = cID;
  histTyp->histType = 1; // 1D histogram
  histTyp->meshName = mName;
  histTyp->primName = pName;
  histTyp->idx = cn;
  IDMap[id] = histTyp;
  return id;
}

#include "G4SDManager.hh"
#include "G4ScoringManager.hh"
#include "G4VScoringMesh.hh"
#include "G4VPrimitivePlotter.hh"
G4int eASTRunAction::Create1DForPlotter(G4String& mName,G4String& pName,G4bool /*wgt*/)
{
  using MeshShape = G4VScoringMesh::MeshShape;

  G4String collName = mName;
  collName += "/";
  collName += pName;
  auto cID = G4SDManager::GetSDMpointer()->GetCollectionID(collName);
  if(cID<0) return cID;

  auto sm = G4ScoringManager::GetScoringManagerIfExist();
  assert(sm!=nullptr);
  auto mesh = sm->FindMesh(mName);
  if(mesh==nullptr) 
  { return -2; }
  auto shape = mesh->GetShape();
  if(shape!=MeshShape::realWorldLogVol && shape!=MeshShape::probe)
  { return -3; }
  G4int nBin[3];
  mesh->GetNumberOfSegments(nBin);

  auto prim = mesh->GetPrimitiveScorer(pName);
  if(prim==nullptr)
  { return -3; }
  auto pp = dynamic_cast<G4VPrimitivePlotter*>(prim);
  if(pp==nullptr)
  { return -4; }
  
  G4int id0 = (cID+id_offset)*id_factor+1;
  for(G4int cn=0; cn<nBin[0]; cn++)
  {
    G4int id = id0+cn;
    auto histoTypeItr = IDMap.find(id);
    if(histoTypeItr!=IDMap.end())
    { return -5; }
    if(verbose) G4cout << "eASTRunAction::Create1D for <" << collName
                     << ", copyNo=" << cn << "> is registered for hitCollectionID "
                     << cID << G4endl;
  
    auto histTyp = new eASTHistoType;
    histTyp->collID = cID;
    histTyp->histType = 1; // 1D histogram
    histTyp->histDup = nBin[0];
    histTyp->meshName = mName;
    histTyp->primName = pName;
    histTyp->idx = cn;
    histTyp->pplotter = pp;
    IDMap[id] = histTyp;
  }
  return id0;
}  

#include "G4UIcommand.hh"
G4bool eASTRunAction::Set1D(G4int id0,G4int nBin,G4double valMin,G4double valMax,G4String& unit,
                          G4String& schem, G4bool logVal)
{
  OpenFile();

  auto hIt = IDMap.find(id0);
  if(hIt==IDMap.end()) return false;

  auto analysisManager = G4AnalysisManager::Instance();
  auto dup = (hIt->second)->histDup;
  for(G4int ii=0;ii<dup;ii++)
  {
    G4int id = id0 + ii;
    auto hItr = IDMap.find(id);
    auto ht = hItr->second;
    G4String mNam = ht->primName;
    G4String nam = ht->meshName + "_" + ht->primName;
    if(ht->idx>-1)
    { 
      mNam += "_";
      mNam += G4UIcommand::ConvertToString(ht->idx);
      nam += "_";
      nam += G4UIcommand::ConvertToString(ht->idx);
    }
    G4int hid = -1;
    if(schem=="linear")
    { hid = analysisManager->CreateH1(mNam,nam,nBin,valMin,valMax,unit,"none","linear"); }
    else
    {
      if(logVal)
      { hid = analysisManager->CreateH1(mNam,nam,nBin,valMin,valMax,unit,"log10","linear"); }
      else
      {
        hid = analysisManager->CreateH1(mNam,nam,nBin,valMin,valMax,unit,"none","log");
        analysisManager->SetH1XAxisIsLog(hid,true);
      }
    }

    if(verbose) G4cout << "eASTRunAction::Set1D for " << mNam << " / " << nam
                       << " has the histogram ID " << hid << G4endl;
    ht->histID = hid;
    auto pp = ht->pplotter;
    if(pp!=nullptr) pp->Plot(ht->idx,hid);
  }
  return true;
}

G4bool eASTRunAction::Set1DTitle(G4int id,G4String& title,G4String& x_axis,G4String&y_axis)
{
  auto hItr = IDMap.find(id);
  if(hItr==IDMap.end()) return false;

  auto analysisManager = G4AnalysisManager::Instance();
  auto hid = hItr->second->histID;
  analysisManager->SetH1Title(hid,title);
  analysisManager->SetH1XAxisTitle(hid,x_axis);
  analysisManager->SetH1YAxisTitle(hid,y_axis);
  return true;
}

G4bool eASTRunAction::Set1DYAxisLog(G4int id0,G4bool val)
{
  auto hIt = IDMap.find(id0);
  if(hIt==IDMap.end()) return false;
  auto analysisManager = G4AnalysisManager::Instance();
  auto dup = (hIt->second)->histDup;
  for(G4int ii=0;ii<dup;ii++)
  {
    G4int id = id0 + ii;
    auto hItr = IDMap.find(id);
    analysisManager->SetH1YAxisIsLog(hItr->second->histID,val);
  }
  return true;
}
  
// ------------- 1D profile

G4int eASTRunAction::Create1P(G4String& mName,G4String& pName,G4int cn)
{
  G4String collName = mName;
  collName += "/";
  collName += pName;
  auto cID = G4SDManager::GetSDMpointer()->GetCollectionID(collName);
  if(cID<0) return cID;

  G4int id = (cID+2*id_offset)*id_factor;
  auto histoTypeItr = IDMap.find(id);
  if(histoTypeItr!=IDMap.end()) return false;
  if(verbose) G4cout << "eASTRunAction::Create1P for <" << collName
                     << "> is registered for hitCollectionID "
                     << cID << G4endl;

  auto histTyp = new eASTHistoType;
  histTyp->collID = cID;
  histTyp->histType = 2; // 1D profile
  histTyp->meshName = mName;
  histTyp->primName = pName;
  histTyp->idx = cn;
  IDMap[id] = histTyp;
  return id;
}

G4bool eASTRunAction::Set1P(G4int id,G4double valYMin,G4double valYMax,G4String& unit,
        G4String& funcX,G4String& funcY,G4String& schem)
{
  OpenFile();

  if(verbose) G4cout << "eASTRunAction::Set1P for id = " << id << G4endl;
  auto hItr = IDMap.find(id);
  if(hItr==IDMap.end()) return false;

  auto ht = hItr->second;
  if(verbose) G4cout << "eASTRunAction::Set1P for " << ht->meshName << " / " << ht->primName << G4endl;
  auto analysisManager = G4AnalysisManager::Instance();
  auto nBin = ht->idx;
  G4double valMin = -0.5;
  G4double valMax = G4double(nBin) - 0.5;
  G4String nam = ht->meshName + "_" + ht->primName;
  auto hid = analysisManager->CreateP1(nam,ht->primName,nBin,
              valMin,valMax,valYMin,valYMax,"none",unit,funcX,funcY,schem);

  if(verbose) G4cout << "eASTRunAction::Set1P for " << ht->meshName << " / " << ht->primName
                     << " has the histogram ID " << hid << G4endl;
  ht->histID = hid;
  return true;
}

G4bool eASTRunAction::Set1PTitle(G4int id,G4String& title,G4String& x_axis,G4String&y_axis)
{
  auto hItr = IDMap.find(id);
  if(hItr==IDMap.end()) return false;

  auto analysisManager = G4AnalysisManager::Instance();
  auto hid = hItr->second->histID;
  analysisManager->SetP1Title(hid,title);
  analysisManager->SetP1XAxisTitle(hid,x_axis);
  analysisManager->SetP1YAxisTitle(hid,y_axis);
  return true;
}

// ------------- Ntuple

G4int eASTRunAction::NtupleColumn(G4String& mName,G4String& pName,G4String& unit,G4int cn)
{
  G4String collName = mName;
  collName += "/";
  collName += pName;
  auto cID = G4SDManager::GetSDMpointer()->GetCollectionID(collName);
  if(cID<0) return cID;

  G4int id = NTMap.size();
  if(verbose) G4cout << "eASTRunAction::NtupleColumn : <" << collName
                     << ", copyNo=" << cn << "> is registered for nTuple column "
                     << id << G4endl;

  auto histTyp = new eASTHistoType;
  histTyp->collID = cID;
  histTyp->meshName = mName;
  histTyp->primName = pName;
  histTyp->meshName2 = unit;
  if(unit!="none")
  { histTyp->fuct = 1./(G4UnitDefinition::GetValueOf(unit)); }
  histTyp->idx = cn;
  NTMap[id] = histTyp;
  return id;
}

#include "G4UIcommand.hh"

void eASTRunAction::DefineNTColumn()
{
  if(NTMap.size()==0) return;

  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->CreateNtuple("eASTNtuple","Scores for each event");
  
  for(auto itr : NTMap)
  {
    G4String colNam = itr.second->meshName;
    colNam += "_";
    colNam += itr.second->primName;
    if(itr.second->idx != -1)
    { colNam += "_"; colNam += G4UIcommand::ConvertToString(itr.second->idx); }
    if(itr.second->meshName2 != "none")
    { colNam += "["; colNam += itr.second->meshName2; colNam += "]"; }
    analysisManager->CreateNtupleDColumn(colNam);
  }

  analysisManager->FinishNtuple();
}

#include <fstream>
#include "G4Threading.hh"
#include "G4UImanager.hh"

void eASTRunAction::MergeNtuple()
{
  if(NTMap.size()==0) return;
  if(!(G4Threading::IsMultithreadedApplication())) return;

  auto analysisManager = G4AnalysisManager::Instance();

  // This MergeNtuple() method is valid only for CSV file format
  if(analysisManager->GetType()!="Csv") return;

  std::fstream target;
  G4String targetFN = fileName + "_nt_eASTNtuple_total.csv";
  target.open(targetFN,std::ios::out);

  enum { BUFSIZE = 4096 };
  char* line = new char[BUFSIZE];

  G4String titleFN = fileName + "_nt_eASTNtuple.csv";
  std::ifstream title;
  title.open(titleFN,std::ios::in);
  while(1)
  {
    title.getline(line,BUFSIZE);
    if(title.eof()) break;
    G4cout << line << G4endl;
    target << line << G4endl;
  }
  title.close();

  auto nWorker = G4Threading::GetNumberOfRunningWorkerThreads();
  G4String sourceFNBase = fileName + "_nt_eASTNtuple_t";
  for(G4int i = 0; i < nWorker; i++)
  {
    G4String sourceFN = sourceFNBase;
    sourceFN += G4UIcommand::ConvertToString(i);
    sourceFN += ".csv";
    std::ifstream source;
    source.open(sourceFN,std::ios::in);
    if(!source)
    {
      G4ExceptionDescription ed; ed << "Source file <" << sourceFN << "> is not found.";
      G4Exception("eASTRunAction::MergeNtuple()","eASTim12345",FatalException,ed);
    }
    while(1)
    {
      source.getline(line,BUFSIZE);
      if(line[0]=='#') continue;
      if(source.eof()) break;
      target << line << G4endl;
    }
    source.close();
    G4String scmd = "rm -f ";
    scmd += sourceFN;
    auto rc = system(scmd);
    if(rc<0)
    {
      G4ExceptionDescription ed; 
      ed << "File <" << sourceFN << "> could not be deleted, thought it is merged.";
      G4Exception("eASTRunAction::MergeNtuple()","eASTim12345",JustWarning,ed);
    }
  }

  target.close();

  G4String cmd = "mv ";
  cmd += targetFN;
  cmd += " ";
  cmd += titleFN;
  auto rcd = system(cmd);
  if(rcd<0)
  {
    G4ExceptionDescription ed; 
    ed << "File <" << targetFN << "> could not be renamed.";
    G4Exception("eASTRunAction::MergeNtuple()","eASTim12345",JustWarning,ed);
  }
}


