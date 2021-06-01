// ********************************************************************
//
// eASTRegionInformation.hh
//   Header file of Region Information class to hold parameters of
//   detector region
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
    eASTRegionInformation(G4String name)
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
    void SetExperimentalHall(G4bool val = true) { isExperimentalHall = val; }
    void SetBeamPipe(G4bool val = true) { isBeamPipe = val; }
    void SetMagnet(G4bool val = true) { isMagnet = val; }
    void SetSupportStructure(G4bool val = true) { isSupportStructure = val; }

    void SetVertexDetector(G4bool val = true) { isVertexDetector = val; }
    void SetTracker(G4bool val = true) { isTracker = val; }
    void SetEMCalorimeter(G4bool val = true) { isEMCalorimeter = val; }
    void SetHadronicCalorimeter(G4bool val = true) { isHadronicCalorimeter = val; }
    void SetCherenkovDetector(G4bool val = true) { isCherenkovDetector = val; }

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

