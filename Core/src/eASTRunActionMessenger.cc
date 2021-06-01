// ********************************************************************
//
// eASTRunActionMessenger.cc
//   A messenger class that defines histograms and n-tuple in eASTRunAction.
//
// History
//   September 8th, 2020 : first implementation
//
// ********************************************************************

#include "eASTRunActionMessenger.hh"

#include "eASTRunAction.hh"
#include "G4UIcommand.hh"
#include "G4UIparameter.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UnitsTable.hh"

eASTRunActionMessenger::eASTRunActionMessenger(eASTRunAction* dc)
: pRA(dc), currentID(-1)
{
  G4UIparameter* para = nullptr;

  anaDir = new G4UIdirectory("/eAST/analysis/");
  anaDir->SetGuidance("GORAD analysis commands");

  verboseCmd = new G4UIcmdWithAnInteger("/eAST/analysis/verbose",this);
  verboseCmd->SetGuidance("Set verbose level");
  verboseCmd->SetParameterName("level",true);
  verboseCmd->SetDefaultValue(0);
  verboseCmd->SetRange("level>=0");
  verboseCmd->AvailableForStates(G4State_Idle);

  fileCmd = new G4UIcmdWithAString("/eAST/analysis/file",this);
  fileCmd->SetGuidance("Define the output file name.");
  fileCmd->SetParameterName("file",false);
  fileCmd->AvailableForStates(G4State_Idle);

  listCmd = new G4UIcmdWithoutParameter("/eAST/analysis/list",this);
  listCmd->SetGuidance("List defined histograms.");
  listCmd->AvailableForStates(G4State_Idle);

  openCmd = new G4UIcmdWithAnInteger("/eAST/analysis/open",this);
  openCmd->SetGuidance("Open a histogram that has already been created and closed.");
  openCmd->SetGuidance("\"create\" command opens a new histogram so you don't need to open it.");
  openCmd->SetGuidance("This \"open\" command is required only for reopening a closed histogram.");
  openCmd->SetParameterName("id",false);
  openCmd->AvailableForStates(G4State_Idle);

  plotCmd = new G4UIcmdWithAnInteger("/eAST/analysis/plot",this);
  plotCmd->SetGuidance("Create an additional postscript plot for specified histogram/profile.");
  plotCmd->SetGuidance("Regardless of this command, histogram is dumped to the output file.");
  plotCmd->SetGuidance("If id is not specified, currently open histogram/profile is plotted.");
  plotCmd->SetGuidance("If id = -1, all currently defined histograms/profiles are plotted.");
  plotCmd->SetParameterName("id",true,true);
  plotCmd->AvailableForStates(G4State_Idle);

  carryCmd = new G4UIcmdWithABool("/eAST/analysis/carry",this);
  carryCmd->SetGuidance("Carry histograms over more than one runs.");
  carryCmd->SetGuidance("Once this is set, histograms won't be output until /eAST/analysis/flush is explicitly issued.");
  carryCmd->SetGuidance("This command has to be issued before starting the run to be carried over.");
  carryCmd->SetParameterName("carry",true);
  carryCmd->SetDefaultValue(true);
  carryCmd->AvailableForStates(G4State_Idle);

  flushCmd = new G4UIcmdWithoutParameter("/eAST/analysis/flush",this);
  flushCmd->SetGuidance("Make output. This command is necessary if /eAST/analysis/carry is set.");
  flushCmd->AvailableForStates(G4State_Idle);

  resetCmd = new G4UIcmdWithoutParameter("/eAST/analysis/reset",this);
  resetCmd->SetGuidance("Reset histograms without making output.");
  resetCmd->AvailableForStates(G4State_Idle);

  oneDDir = new G4UIdirectory("/eAST/analysis/1D/");
  oneDDir->SetGuidance("1-dimentional histogram");

  create1DCmd = new G4UIcommand("/eAST/analysis/1D/create",this);
  create1DCmd->SetGuidance("Create a 1D histogram and fill it with event-by-event score.");
  create1DCmd->SetGuidance("Scoring mesh (logical volume for real-world volume scoring) and");
  create1DCmd->SetGuidance("primitive scorers must be defined prior to this command.");
  para = new G4UIparameter("meshName",'s',false);
  para->SetGuidance("Scoring mesh name. Logical volume name for real-world volume scoring.");
  create1DCmd->SetParameter(para);
  para = new G4UIparameter("primName",'s',false);
  create1DCmd->SetParameter(para);
  para = new G4UIparameter("idx",'i',true);
  para->SetGuidance("Index (i.e. copy number) of the cell to be scored. \"-1\" (defult) to score all cells.");
  para->SetDefaultValue(-1);
  para->SetParameterRange("idx>=-1");
  create1DCmd->SetParameter(para);
  create1DCmd->AvailableForStates(G4State_Idle);

  create1DPlotPCmd = new G4UIcommand("/eAST/analysis/1D/spectrum",this);
  create1DPlotPCmd->SetGuidance("Create a 1D energy spectrum histogram and fill it with each individual track that gets into the volume.");
  create1DPlotPCmd->SetGuidance("Histogram is created for each physical volume separately.");
  create1DPlotPCmd->SetGuidance("So, this command should not be used for Box or Cylinder mesh type due to memory consumption concern.");
  create1DPlotPCmd->SetGuidance("Currently, this is supported only for some scorers.");
  para = new G4UIparameter("meshName",'s',false);
  para->SetGuidance("Scoring mesh name. Logical volume name for real-world volume scoring.");
  create1DPlotPCmd->SetParameter(para);
  para = new G4UIparameter("primName",'s',false);
  para->SetGuidance("Scoring primitive name.");
  create1DPlotPCmd->SetParameter(para);
  create1DPlotPCmd->AvailableForStates(G4State_Idle);

  config1DCmd = new G4UIcommand("/eAST/analysis/1D/config",this);
  config1DCmd->SetGuidance("Set binning parameters of the current 1D histogram.");
  config1DCmd->SetGuidance("<unit> is applied to <minVal> and <maxVal> as well as filled value.");
  para = new G4UIparameter("nBin",'i',false);
  para->SetParameterRange("nBin>0");
  config1DCmd->SetParameter(para);
  para = new G4UIparameter("minVal",'d',false);
  config1DCmd->SetParameter(para);
  para = new G4UIparameter("maxVal",'d',false);
  config1DCmd->SetParameter(para);
  para = new G4UIparameter("unit",'s',true);
  para->SetDefaultValue("none");
  config1DCmd->SetParameter(para);
  para = new G4UIparameter("scale",'s',true);
  para->SetGuidance("Define the binning scale. (default: linear)");
  para->SetParameterCandidates("linear log");
  para->SetDefaultValue("linear");
  config1DCmd->SetParameter(para);
  para = new G4UIparameter("logVal",'b',true);
  para->SetDefaultValue(false);
  config1DCmd->SetParameter(para);
  config1DCmd->AvailableForStates(G4State_Idle);

  title1DCmd = new G4UIcommand("/eAST/analysis/1D/title",this);
  title1DCmd->SetGuidance("Define histogram title");
  para = new G4UIparameter("title",'s',false);
  title1DCmd->SetParameter(para);
  para = new G4UIparameter("x_axis",'s',false);
  title1DCmd->SetParameter(para);
  para = new G4UIparameter("y_axis",'s',false);
  title1DCmd->SetParameter(para);
  title1DCmd->AvailableForStates(G4State_Idle);

  set1DYaxisLogCmd = new G4UIcmdWithABool("/eAST/analysis/1D/yaxisLog",this);
  set1DYaxisLogCmd->SetGuidance("Set y-axis in log scale.");
  set1DYaxisLogCmd->SetParameterName("flag",true);
  set1DYaxisLogCmd->SetDefaultValue(true);
  set1DYaxisLogCmd->AvailableForStates(G4State_Idle);

  onePDir = new G4UIdirectory("/eAST/analysis/1P/");
  onePDir->SetGuidance("1-dimentional profile plot");

  create1PCmd = new G4UIcommand("/eAST/analysis/1P/create",this);
  create1PCmd->SetGuidance("Create a 1D profile plot and fill it with event-by-event score.");
  create1PCmd->SetGuidance("Scoring mesh (logical volume for real-world volume scoring) and");
  create1PCmd->SetGuidance("primitive scorers must be defined prior to this command.");
  create1PCmd->SetGuidance("Copy number of the scoring cell is used as the x-axis value.");
  para = new G4UIparameter("meshName",'s',false);
  para->SetGuidance("Scoring mesh name. Logical volume name for real-world volume scoring.");
  create1PCmd->SetParameter(para);
  para = new G4UIparameter("primName",'s',false);
  create1PCmd->SetParameter(para);
  para = new G4UIparameter("idx",'i',false);
  para->SetGuidance("Maximum index (i.e. copy number) of the cell to be scored.");
  para->SetParameterRange("idx>0");
  create1PCmd->SetParameter(para);
  create1PCmd->AvailableForStates(G4State_Idle);

  set1PCmd = new G4UIcommand("/eAST/analysis/1P/set",this);
  set1PCmd->SetGuidance("Set binning parameters of the current 1D profile plot.");
  set1PCmd->SetGuidance("<unit> is applied to <minYVal> and <maxYVal> as well as filled value.");
  para = new G4UIparameter("minYVal",'d',false);
  set1PCmd->SetParameter(para);
  para = new G4UIparameter("maxYVal",'d',false);
  set1PCmd->SetParameter(para);
  para = new G4UIparameter("unit",'s',true);
  para->SetDefaultValue("none");
  set1PCmd->SetParameter(para);
  para = new G4UIparameter("func-x",'s',true);
  para->SetGuidance("The function applied to the filled x-value (default: none).");
  para->SetParameterCandidates("log log10 exp none");
  para->SetDefaultValue("none");
  set1PCmd->SetParameter(para);
  para = new G4UIparameter("func-y",'s',true);
  para->SetGuidance("The function applied to the filled y-value (default: none).");
  para->SetParameterCandidates("log log10 exp none");
  para->SetDefaultValue("none");
  set1PCmd->SetParameter(para);
  para = new G4UIparameter("scale",'s',true);
  para->SetGuidance("Define the binning scale. (default: linear)");
  para->SetParameterCandidates("linear log");
  para->SetDefaultValue("linear");
  set1PCmd->SetParameter(para);
  set1PCmd->AvailableForStates(G4State_Idle);

  title1PCmd = new G4UIcommand("/eAST/analysis/1P/title",this);
  title1PCmd->SetGuidance("Define histogram title");
  para = new G4UIparameter("title",'s',false);
  title1PCmd->SetParameter(para);
  para = new G4UIparameter("x_axis",'s',false);
  title1PCmd->SetParameter(para);
  para = new G4UIparameter("y_axis",'s',false);
  title1PCmd->SetParameter(para);
  title1PCmd->AvailableForStates(G4State_Idle);

  ntupleDir = new G4UIdirectory("/eAST/analysis/ntuple/");
  onePDir->SetGuidance("Define an ntuple");

  addColumnCmd = new G4UIcommand("/eAST/analysis/ntuple/addColumn",this);
  addColumnCmd->SetGuidance("Define a column and fill it with event-by-event score.");
  addColumnCmd->SetGuidance("Scoring mesh (logical volume for real-world volume scoring) and");
  addColumnCmd->SetGuidance("primitive scorers must be defined prior to this command.");
  para = new G4UIparameter("meshName",'s',false);
  para->SetGuidance("Scoring mesh name. Logical volume name for real-world volume scoring.");
  addColumnCmd->SetParameter(para);
  para = new G4UIparameter("primName",'s',false);
  addColumnCmd->SetParameter(para);
  para = new G4UIparameter("unit",'s',true);
  para->SetDefaultValue("none");
  addColumnCmd->SetParameter(para);
  para = new G4UIparameter("idx",'i',true);
  para->SetGuidance("Index (i.e. copy number) of the cell to be scored. \"-1\" (defult) to score all cells.");
  para->SetDefaultValue(-1);
  para->SetParameterRange("idx>=-1");
  addColumnCmd->SetParameter(para);
  addColumnCmd->AvailableForStates(G4State_Idle);
}

eASTRunActionMessenger::~eASTRunActionMessenger()
{ 
  delete addColumnCmd;
  delete ntupleDir;

  delete create1PCmd;
  delete set1PCmd;
  delete title1PCmd;
  delete onePDir;

  delete create1DCmd;
  delete create1DPlotPCmd;
  delete config1DCmd;
  delete title1DCmd;
  delete set1DYaxisLogCmd;
  delete oneDDir;

  delete fileCmd;
  delete verboseCmd;
  delete listCmd;
  delete openCmd;
  delete plotCmd;
  delete carryCmd;
  delete flushCmd;
  delete resetCmd;
  delete anaDir;
}

#include "G4Tokenizer.hh"

void eASTRunActionMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  if(cmd==fileCmd)
  { pRA->SetFileName(val); }
  else if(cmd==verboseCmd)
  { pRA->SetVerbose(verboseCmd->GetNewIntValue(val)); }
  else if(cmd==listCmd)
  { pRA->ListHistograms(); }
  else if(cmd==openCmd)
  { 
    auto id = openCmd->GetNewIntValue(val);
    if(currentID!=id)
    {
      if(!CheckOpenID(cmd)) return;
      auto valid = pRA->Open(id);
      if(!valid)
      {
        G4ExceptionDescription ed;
        ed << "<" << id << "> is not a valid histogram ID.";
        cmd->CommandFailed(ed);
      }
      else
      { currentID = id; }
    }
  }
  else if(cmd==plotCmd)
  { 
    auto id = plotCmd->GetNewIntValue(val);
    G4bool valid = true;
    if(id==-1)
    { valid = pRA->SetAllPlotting(true); }
    else
    { valid = pRA->SetPlotting(id,true); }
    if(!valid)
    {
      G4ExceptionDescription ed;
      ed << "Histogram/profile id <" << id << "> is not valid.";
      cmd->CommandFailed(ed);
    }
  }
  else if(cmd==carryCmd)
  { pRA->SetCarry(carryCmd->GetNewBoolValue(val)); }
  else if(cmd==flushCmd)
  { pRA->Flush(); }
  else if(cmd==resetCmd)
  { /*pRA->ResetHistograms();*/
    G4cout << "Not yet implemented. Sorry." << G4endl;
  }

  // 1D histogram commands

  else if(cmd==create1DCmd)
  {
    if(!CheckOpenID(cmd)) return;
    G4Tokenizer next(val);
    G4String meshName = next();
    G4String primName = next();
    G4int idx = StoI(next());
    auto id = pRA->Create1D(meshName,primName,idx);
    if(id<0)
    {
      G4ExceptionDescription ed;
      ed << "1D histogram <" << val << "> cannot be created.";
      cmd->CommandFailed(ed);
    }
    else
    { currentID = id; }
  }
  else if(cmd==create1DPlotPCmd)
  {
    if(!CheckOpenID(cmd)) return;
    G4Tokenizer next(val);
    G4String meshName = next();
    G4String primName = next();
    G4bool wgt = true;
    auto id = pRA->Create1DForPlotter(meshName,primName,wgt);
    if(id<0)
    {
      G4ExceptionDescription ed;
      ed << "1D histogram <" << val << "> cannot be created.";
      cmd->CommandFailed(ed);
    }
    else
    { currentID = id; }
  }
  else if(cmd==config1DCmd)
  {
    if(!CheckID(cmd)) return;
    G4Tokenizer next(val);
    G4int nBin = StoI(next());
    G4double minVal = StoD(next());
    G4double maxVal = StoD(next());
    G4String unit = next();
    G4String schem = next();
    G4bool logVal = StoB(next());
    if(unit!="none" && !(G4UnitDefinition::IsUnitDefined(unit)))
    { 
      G4ExceptionDescription ed;
      ed << "Unknown unit <" << unit << ">. Command failed.";
      cmd->CommandFailed(ed);
    }
    else
    { pRA->Set1D(currentID,nBin,minVal,maxVal,unit,schem,logVal); }
  }
  else if(cmd==title1DCmd)
  {
    if(!CheckID(cmd)) return;
    G4Tokenizer next(val);
    G4String title = next();
    G4String x_axis = next();
    G4String y_axis = next();
    pRA->Set1DTitle(currentID,title,x_axis,y_axis);
  }
  else if(cmd==set1DYaxisLogCmd)
  {
    if(!CheckID(cmd)) return;
    auto succ = pRA->Set1DYAxisLog(currentID,set1DYaxisLogCmd->GetNewBoolValue(val));
    if(!succ)
    {
      G4ExceptionDescription ed;
      ed << "This command is not available for this histogram.";
      cmd->CommandFailed(ed);
    }
  }

  // 1D profile commands

  else if(cmd==create1PCmd)
  {
    if(!CheckOpenID(cmd)) return;
    G4Tokenizer next(val);
    G4String meshName = next();
    G4String primName = next();
    G4int cn = StoI(next());
    auto id = pRA->Create1P(meshName,primName,cn);
    if(id<0)
    {
      G4ExceptionDescription ed;
      ed << "1D histogram <" << val << "> cannot be created.";
      cmd->CommandFailed(ed);
    }
    else
    { currentID = id; }
  }
  else if(cmd==set1PCmd)
  {
    if(!CheckID(cmd)) return;
    G4Tokenizer next(val);
    G4double minYVal = StoD(next());
    G4double maxYVal = StoD(next());
    G4String unit = next();
    G4String funcX = next();
    G4String funcY = next();
    G4String schem = next();
    if(unit!="none" && !(G4UnitDefinition::IsUnitDefined(unit)))
    { 
      G4ExceptionDescription ed;
      ed << "Unknown unit <" << unit << ">. Command failed.";
      cmd->CommandFailed(ed);
    }
    else
    { pRA->Set1P(currentID,minYVal,maxYVal,unit,funcX,funcY,schem); }
  }
  else if(cmd==title1PCmd)
  {
    if(!CheckID(cmd)) return;
    G4Tokenizer next(val);
    G4String title = next();
    G4String x_axis = next();
    G4String y_axis = next();
    pRA->Set1PTitle(currentID,title,x_axis,y_axis);
  }

  // ntuple commands

  else if(cmd==addColumnCmd)
  {
    G4Tokenizer next(val);
    G4String meshName = next();
    G4String primName = next();
    G4String unit = next();
    G4int idx = StoI(next());
    if(unit!="none" && !(G4UnitDefinition::IsUnitDefined(unit)))
    { 
      G4ExceptionDescription ed;
      ed << "Unknown unit <" << unit << ">. Command failed.";
      cmd->CommandFailed(ed);
    }
    else
    {
      auto id = pRA->NtupleColumn(meshName,primName,unit,idx);
      if(id<0)
      {
        G4ExceptionDescription ed;
        ed << "Ntuple column <" << val << "> cannot be created.";
        cmd->CommandFailed(ed);
      }
    }
  }
}

G4String eASTRunActionMessenger::GetCurrentValue(G4UIcommand* cmd)
{
  G4String val("");
  if(cmd==openCmd)
  { val = openCmd->ConvertToString(currentID); }
  if(cmd==fileCmd)
  { val = pRA->GetFileName(); }
  else if(cmd==verboseCmd)
  { val = verboseCmd->ConvertToString(pRA->GetVerbose()); }
  else if(cmd==plotCmd)
  { val = plotCmd->ConvertToString(currentID); }
  else if(cmd==carryCmd)
  { val = carryCmd->ConvertToString(pRA->GetCarry()); }

  return val;
}


