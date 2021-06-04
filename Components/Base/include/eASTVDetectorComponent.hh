// ********************************************************************
//
// eASTVDetectorComponent.hh
//   Header file of the abstract base class of eAST detector component
//
// History
//   May 8th, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTVDetectorComponent_H
#define eASTVDetectorComponent_H 1

class G4VPhysicalVolume;
class G4Region;
class eASTDetectorComponentMessenger;
class eASTUserActionDispatcher;

class G4UserRunAction;
class G4UserEventAction;
class G4UserStackingAction;
class G4UserTrackingAction;
class G4UserSteppingAction;

#include "globals.hh"

class eASTVDetectorComponent
{
  public:
    eASTVDetectorComponent(G4String compName, G4int vl = 0);
    virtual ~eASTVDetectorComponent();

  public:
    void SetUpBase(G4int vl = 0);

  public:
    // invoked only in the master thread
    virtual void SetUp() = 0;
    virtual void Construct(G4VPhysicalVolume*) = 0;
    virtual void ConstructActionForMaster()
    {;}

    // invoked in the worker threads (not in master)
    virtual void ConstructSD()
    {;}
    virtual void ConstructActions()
    {;}

  protected:
    // Utility methods to register component-specific
    // user action classes. These methods should be used 
    // in ConstructActions() and ConstructActionForMaster()
    // methods.
    void RegisterUserAction(G4UserRunAction*);
    void RegisterUserAction(G4UserEventAction*);
    void RegisterUserAction(G4UserStackingAction*);
    void RegisterUserAction(G4UserTrackingAction*);
    void RegisterUserAction(G4UserSteppingAction*);

  protected:
    // Utility method to define a material to a logical
    // volume. The input file is an ASCII file, and 
    // each line of the file should have the name of
    // a logical volume and its material name.
    // Material name must be a name that appears in
    // G4NistManager.
    void ReadMaterialFile(G4String);

  protected:
    G4String componentName;
    G4String commandDir;
    G4int verboseLevel;
    G4VPhysicalVolume* pEnvelopePhys = nullptr;
    G4Region* pRegion = nullptr;

  public:
    virtual G4VPhysicalVolume* GetEnvelope()
    { return pEnvelopePhys; }
    virtual G4Region* GetRegion()
    { return pRegion; }

  private:
    eASTDetectorComponentMessenger* baseMessenger;
    eASTUserActionDispatcher* userActionDispatcher;

  private:
    G4bool CheckRegion();
};

#endif

