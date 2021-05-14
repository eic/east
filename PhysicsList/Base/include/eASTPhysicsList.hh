////////////////////////////////////////////////////////////////////////////////
//                             
//  eASTPhysicsList.hh 
//  Geant4 physics list for Electron Ion Collider detector simulation
//                                                                  
//  History                                                        
//    Jun.21.2018 : original implementation - Dennis H. Wright (SLAC) 
//    May.06.2021 : migration to eAST - Makoto Asai (SLAC)
//                                                                      
////////////////////////////////////////////////////////////////////////////////

#ifndef eASTPhysicsList_h
#define eASTPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"


class eASTPhysicsList: public G4VModularPhysicsList
{
  public:
    eASTPhysicsList();
    ~eASTPhysicsList();

    virtual void ConstructParticle();
    virtual void SetCuts();

};

#endif
