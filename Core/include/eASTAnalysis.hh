// ********************************************************************
//
// eASTAnalysis.hh
//   Defining the file format of output histogram/n-tuple
//
// History
//   May 8th, 2021 : first implementation - Makoto Asai (SLAC)
//   Dec 22nd, 2021 :
//          This file is no longer needed for Geant4 version 11.0
//
// ********************************************************************

#ifndef eASTAnalysis_h
#define eASTAnalysis_h 1
#include "G4Version.hh"
#if G4VERSION_NUMBER < 1100
//#include "g4root.hh"
#include "g4csv.hh"
//#include "g4xml.hh"
#else
#include "G4AnalysisManager.hh"
#endif
#endif
