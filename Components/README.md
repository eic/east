# eAST detector components

## Overview
*eAST* (eA simulation toolkit) assumes each detector component is descrived by a concrete class derived from _eASTVDetectoComponent_ base class.
Base classes and utility classes that should be used by a detector component are stored in `/Components/Base` sub-directory.

All volumes of a detector component should be fully contained by a mother volume that represents the component, and this mother volume must have a unique region.

## eASTVDetectorComponentbase class

_eASTVDetectoComponent_ may represent both detector component (i.e. including sensitive detector) and support structure.
A concrete class should implement the following vertual methods.

### void SetUP()

UI commands necessary to configure the detector component must be defined in this method. The base-class data member _commandDir_ is already created.
Also, _locate_ and _rotate_ commands are by default created via the base class to locate/rotate the envelope of the detector component to the world volume.
Commands can be for input file name or switching complexities. 
In the future, commands for physics options will be added as well.


### void Construct(G4VPhysicalVolume* worldPhys)

All the volumes should be constructed in this method, and the top level envelope(s) should be located to _worldPhys_ world volume.
You can assume all the UI commands needed for configuration defined in above-mentioned _SetUp()_ method have already been applied.
For the convenience, 
```
    void Locate(G4LogicalVolume* compLogVol, G4VPhysicalVolume* worldPhys);
```
is provided placing top level envelope(s) to _worldPhys_. This method takes _locate_ and _rotate_ UI commands into account.

If geometry is read from a GDML file and it does not have material definitions to each volume, a utility method is provided.
```
    void ReadMaterialFile(G4String fileName);
```
Here, the input file is an ASCII file, and  each line of the file should have the name of a logical volume and its material name.
Material name must be a name that appears in G4NistManager.

A dedicated _G4Region_ object must be instantiated and set to the logical volume(s) of the envelope(s). 
The pointer to this dedicated _G4Region_ object should be kept to _pRegion_ base-class pointer data member.

### void ConstructSD()

This method is invoked for each worker threads, and sensitive detector(s) should be constructed here. If the component is a structure, you don't need to implement this method.

### void ConstructActions()

This method is invoked for each worker threads. If the component needs any of UserRunAction, UserEventAction, UserStackingAction, UserTrackingAction and/or UserSteppingAction, they must be instantiated in this method and registered by the following utility methods.
```
    void RegisterUserAction(G4UserRunAction*);
    void RegisterUserAction(G4UserEventAction*);
    void RegisterUserAction(G4UserStackingAction*);
    void RegisterUserAction(G4UserTrackingAction*);
    void RegisterUserAction(G4UserSteppingAction*);
```

### void ConstructActionForMaster()

This method is invoked only in the master thread, and the component needs UserRunAction for the global run, it must be instantiated in this method and registered by the following utility method.
```
    void RegisterUserAction(G4UserRunAction*);
```

## Defining a component

A component has to be instantiated in [_eASTInitialization_](../Core/src/eASTInitialization.cc) with its unique component name. 

The component is actually constructed only if a user issues a UI command
```
   /eAST/component/<_componentName_> <_verboseLevel_>
```





 

