// ********************************************************************
//
// eASTRegionInformation.hh
//   Header file of Region Information class to hold parameters of
//   detector regions
//
// History
//   May 8th, 2021 : first implementation - Makoto Asai (SLAC)
//
// ********************************************************************

#ifndef eASTRegionInformation_h
#define eASTRegionInformation_h 1

#include "G4VUserRegionInformation.hh"
#include "globals.hh"

class eASTRegionInformation : public G4VUserRegionInformation
{
  public:
    eASTRegionInformation(G4Stering& name)
    { regionName = name; }
    virtual ~eASTRegionInformation()
    {;}

  public:
    virtual void Print() const
    { G4cout << "eASTRegionInformation : " << regionName << G4endl; }

 private:
    G4String regionName;

    G4bool isExperimentalHall = false;
    G4bool isBeamPipe = false;
    G4bool isMagnet = false;
    G4bool isSupportStructure = false;

    G4bool isVertexDetector = false;
    G4bool isTracker = false;
    G4bool isEMCalorimeter = false;
    G4bool isHadronicCalorimeter = false;
    G4bool isCherenkovDetector = false;

  public:
    G4bool IsExperimentalHall() const { return isExperimentalHall; }
    G4bool IsBeamPipe() const { return isBeamPipe; }
    G4bool IsMagnet() const { return isMagnet; }
    G4bool IsSupportStructure() const { return isSupportStructure; }

    G4bool IsVertexDetector() const { return isVertexDetector; }
    G4bool IsTracker() const { return isTracker; }
    G4bool IsEMCalorimeter() const { return isEMCalorimeter; }
    G4bool IsHadronicCalorimeter() const { return isHadronicCalorimeter; }
    G4bool IsCherenkovDetector() const { return isCherenkovDetector; }
};

#endif

