#ifndef EXTRACT_ECCE_GDML_C
#define EXTRACT_ECCE_GDML_C

#include <GlobalVariables.C>

#include <G4Setup_EICDetector.C>
#include <G4_Global.C>
#include <G4_Input.C>
#include <G4_Production.C>
#include <G4_User.C>

#include <TROOT.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include <RooUnblindPrecision.h>

R__LOAD_LIBRARY(libfun4all.so)

// Get all subsystem options with
// grep 'if ( subsys==' Extract_ECCE_gdml.C | cut -d'"' -f 2
// To create them all, run
// foreach s ( all pipe magfarfwd magfarbwd gems tracking tofs becal hcalin hcalout dirc femc drcalo lfhcal eemc ehcal )
//    root -l -b -q Extract_ECCE_gdml.C\(\"$s\"\)
// end

// Important: Fix generated gdml with
// sed -i .bak 's/constant/matrix coldim="1"/g ; s/value/values/g' *gdml

int Extract_ECCE_gdml( string subsys="all", const string outbase="")
{

  // Make the subsystem case-insensitive
  for (auto & c: subsys) c = tolower(c);
  
  // output filename
  string outname = "";
  if ( subsys=="all" ) outname = "ecce.gdml";

  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  //===============
  // Input options
  //===============

  // switching IPs by comment/uncommenting the following lines
  // used for both beamline setting and for the event generator crossing boost
  Enable::IP6 = true;
  // Enable::IP8 = true;


  //======================
  // What to run
  //======================
  // Global options (enabled for all subsystems - if implemented)
  //  Enable::ABSORBER = true;
  //  Enable::OVERLAPCHECK = true;
  //  Enable::VERBOSITY = 1;
  
  if ( subsys=="pipe" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    // whether to simulate the Be section of the beam pipe
    Enable::PIPE = true;
    // If need to disable EIC beam pipe extension beyond the Be-section:
    G4PIPE::use_forward_pipes = true;
    //EIC hadron far forward magnets and detectors. IP6 and IP8 are incompatible (pick either or);
  }

  if ( subsys=="magfarfwd" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::HFARFWD_MAGNETS = true;
    Enable::HFARFWD_VIRTUAL_DETECTORS = true;
  }

  if ( subsys=="magfarbwd" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::HFARBWD_MAGNETS = true;
    Enable::HFARBWD_VIRTUAL_DETECTORS = true;
  }
  
  if ( subsys=="gems" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    // gems
    Enable::EGEM = true;
    Enable::FGEM = true;
    // Enable::BGEM = true; // not yet defined in this model
  }
   
  if ( subsys=="tracking" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::RWELL = true;
    // barrel tracker
    Enable::TrackingService = true;
    // Enable::TrackingService_VERBOSITY = INT_MAX - 10;
    Enable::BARREL = true;
    // fst
    Enable::FST = true;
  }

  if ( subsys=="tofs" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    // TOFs
    Enable::FTTL = true;
    Enable::ETTL = true;
    Enable::CTTL = true;
    G4TTL::SETTING::optionCEMC = false;
    G4TTL::SETTING::optionGeo = 1;
  }

  Enable::TRACKING = true;
  Enable::TRACKING_EVAL = Enable::TRACKING && true;
  G4TRACKING::DISPLACED_VERTEX = true;  // this option exclude vertex in the track fitting and use RAVE to reconstruct primary and 2ndary vertexes
                                        // projections to calorimeters
  G4TRACKING::PROJECTION_EEMC = true;
  G4TRACKING::PROJECTION_BECAL = true;
  G4TRACKING::PROJECTION_EHCAL = true;
  G4TRACKING::PROJECTION_CEMC = true;
  G4TRACKING::PROJECTION_HCALOUT = true;
  G4TRACKING::PROJECTION_FEMC = true;
  G4TRACKING::PROJECTION_FHCAL = true;
  G4TRACKING::PROJECTION_LFHCAL = true;

  if ( subsys=="becal" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::BECAL = true;
    Enable::BECAL_CELL = Enable::BECAL && true;
    Enable::BECAL_TOWER = Enable::BECAL_CELL && true;
    Enable::BECAL_CLUSTER = Enable::BECAL_TOWER && true;
    Enable::BECAL_EVAL = Enable::BECAL_CLUSTER && true;
  }

  if ( subsys=="hcalin" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::HCALIN = true;
    //  Enable::HCALIN_ABSORBER = true;
    Enable::HCALIN_CELL = Enable::HCALIN && true;
    Enable::HCALIN_TOWER = Enable::HCALIN_CELL && true;
    Enable::HCALIN_CLUSTER = Enable::HCALIN_TOWER && true;
    Enable::HCALIN_EVAL = Enable::HCALIN_CLUSTER && true;
  }

  Enable::MAGNET = true;

  if ( subsys=="hcalout" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::HCALOUT = true;
    //  Enable::HCALOUT_ABSORBER = true;
    Enable::HCALOUT_CELL = Enable::HCALOUT && true;
    Enable::HCALOUT_TOWER = Enable::HCALOUT_CELL && true;
    Enable::HCALOUT_CLUSTER = Enable::HCALOUT_TOWER && true;
    Enable::HCALOUT_EVAL = Enable::HCALOUT_CLUSTER && true;
  }
    
  if ( subsys=="dirc" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    // EICDetector geometry - barrel
    Enable::DIRC = true;
    Enable::DIRC_RECO = Enable::DIRC && true;
    // Enable::DIRC_VERBOSITY = 2;
    
    // EICDetector geometry - 'hadron' direction
    Enable::RICH = true;
    Enable::RICH_RECO = Enable::DIRC && true;
    // Enable::RICH_VERBOSITY = 2;
    
    // EICDetector geometry - 'electron' direction
    Enable::mRICH = true;
    Enable::mRICH_RECO = Enable::DIRC && true;
    // Enable::mRICH_VERBOSITY = 2;
  }

  if ( subsys=="femc" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::FEMC = true;
    //  Enable::FEMC_ABSORBER = true;
    Enable::FEMC_TOWER = Enable::FEMC && true;
    Enable::FEMC_CLUSTER = Enable::FEMC_TOWER && true;
    Enable::FEMC_EVAL = Enable::FEMC_CLUSTER && true;
  }    

  // if ( subsys=="drcalo" || subsys=="all" ) {
  if ( subsys=="drcalo" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::DRCALO = true;
    Enable::DRCALO_CELL = Enable::DRCALO && true;
    Enable::DRCALO_TOWER = Enable::DRCALO_CELL && true;
    Enable::DRCALO_CLUSTER = Enable::DRCALO_TOWER && true;
    Enable::DRCALO_EVAL = Enable::DRCALO_CLUSTER && false;
    G4TTL::SETTING::optionDR = 1;
  }

  if ( subsys=="lfhcal" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::LFHCAL = true;
    Enable::LFHCAL_ABSORBER = false;
    Enable::LFHCAL_CELL = Enable::LFHCAL && true;
    Enable::LFHCAL_TOWER = Enable::LFHCAL_CELL && true;
    Enable::LFHCAL_CLUSTER = Enable::LFHCAL_TOWER && true;
    Enable::LFHCAL_EVAL = Enable::LFHCAL_CLUSTER && true;
  }

  if ( subsys=="eemc" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    // EICDetector geometry - 'electron' direction
    Enable::EEMCH = true;
    Enable::EEMCH_TOWER = Enable::EEMCH && true;
    Enable::EEMCH_CLUSTER = Enable::EEMCH_TOWER && true;
    Enable::EEMCH_EVAL = Enable::EEMCH_CLUSTER && true;
    G4TTL::SETTING::optionEEMCH = Enable::EEMCH && true;
  }

  if ( subsys=="ehcal" || subsys=="all" ) {
    if (outname=="") outname = "ecce_" + subsys + ".gdml";
    Enable::EHCAL = true;
    Enable::EHCAL_CELL = Enable::EHCAL && true;
    Enable::EHCAL_TOWER = Enable::EHCAL_CELL && true;
    Enable::EHCAL_CLUSTER = Enable::EHCAL_TOWER && true;
    Enable::EHCAL_EVAL = Enable::EHCAL_CLUSTER && true;
  }
    
  Enable::FFR_EVAL = Enable::HFARFWD_MAGNETS && Enable::HFARFWD_VIRTUAL_DETECTORS && true;

  Enable::PLUGDOOR = false;


  // new settings using Enable namespace in GlobalVariables.C
  Enable::BLACKHOLE = true;
  //Enable::BLACKHOLE_SAVEHITS = false; // turn off saving of bh hits
  //BlackHoleGeometry::visible = true;

  // ZDC
  // Enable::ZDC = true;
  // Enable::ZDC_DISABLE_BLACKHOLE = true;

  // B0
  // Enable::B0_DISABLE_HITPLANE = true;
  // Enable::B0_FULLHITPLANE = true;

  //---------------
  // World Settings
  //---------------
  //  G4WORLD::PhysicsList = "FTFP_BERT"; //FTFP_BERT_HP best for calo
  //  G4WORLD::WorldMaterial = "G4_AIR"; // set to G4_GALACTIC for material scans

  //---------------
  // Magnet Settings
  //---------------

  //  const string magfield = "1.5"; // alternatively to specify a constant magnetic field, give a float number, which will be translated to solenoidal field in T, if string use as fieldmap name (including path)
  //  G4MAGNET::magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root");  // default map from the calibration database
  G4MAGNET::magfield_rescale = -1.4 / 1.5;  // make consistent with expected Babar field strength of 1.4T

  // At this point we better have a valid target subsystem
  if ( outname =="" ){
    cout << "Please specify a valid sub-system. Got:" << subsys << endl;
    return -1;
  }
  
  // Initialize the selected subsystems
  G4Init();
  G4Setup();

  //------------------
  // Detector Division
  //------------------
  if (Enable::CEMC_CELL) CEMC_Cells();

  if (Enable::HCALIN_CELL) HCALInner_Cells();

  if (Enable::HCALOUT_CELL) HCALOuter_Cells();

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------

  if (Enable::CEMC_TOWER) CEMC_Towers();
  if (Enable::CEMC_CLUSTER) CEMC_Clusters();

  //-----------------------------
  // HCAL towering and clustering
  //-----------------------------

  if (Enable::HCALIN_TOWER) HCALInner_Towers();
  if (Enable::HCALIN_CLUSTER) HCALInner_Clusters();

  if (Enable::HCALOUT_TOWER) HCALOuter_Towers();
  if (Enable::HCALOUT_CLUSTER) HCALOuter_Clusters();

  //-----------------------------
  // e, h direction Calorimeter  towering and clustering
  //-----------------------------

  if (Enable::FEMC_TOWER) FEMC_Towers();
  if (Enable::FEMC_CLUSTER) FEMC_Clusters();

  if (Enable::FHCAL_TOWER) FHCAL_Towers();
  if (Enable::FHCAL_CLUSTER) FHCAL_Clusters();

  if (Enable::DRCALO_TOWER) DRCALO_Towers();
  if (Enable::DRCALO_CLUSTER) DRCALO_Clusters();

  if (Enable::LFHCAL_TOWER) LFHCAL_Towers();
  if (Enable::LFHCAL_CLUSTER) LFHCAL_Clusters();

  if (Enable::EEMC_TOWER) EEMC_Towers();
  if (Enable::EEMC_CLUSTER) EEMC_Clusters();

  if (Enable::EEMCH_TOWER) EEMCH_Towers();
  if (Enable::EEMCH_CLUSTER) EEMCH_Clusters();

  if (Enable::EHCAL_TOWER) EHCAL_Towers();
  if (Enable::EHCAL_CLUSTER) EHCAL_Clusters();

  if (Enable::BECAL_TOWER) BECAL_Towers();
  if (Enable::BECAL_CLUSTER) BECAL_Clusters();

  if (Enable::DSTOUT_COMPRESS) ShowerCompress();

  // InputRegister();


  // se->skip(skip);
  se->run(1);
 
  
  //-------
  // Export
  //-------

  PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  // freaks out if the file exists
  outname = outbase + outname;
  gSystem->Unlink(outname.c_str());
  g4->Dump_GDML(outname);


  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
  return 0;
}
#endif // EXTRACT_ECCE_GDML_C
