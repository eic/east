////////////////////////////////////////////////////////////////////////////////
// 
//  eASTIonPhysics.hh  
//  Ion hadronic physics constructor for eASTPhysicsList 
//                                                      
//    Jun.21.2018 : original implementation - Dennis H. Wright (SLAC)
//    May.06.2021 : migration to eAST - Makoto Asai (SLAC)
//                                                     
////////////////////////////////////////////////////////////////////////////////

#ifndef eASTIonPhysics_h
#define eASTIonPhysics_h 1

#include "G4VPhysicsConstructor.hh"

class G4TheoFSGenerator;
class G4FTFModel;
class G4ExcitedStringDecay;
class G4LundStringFragmentation;
class G4GeneratorPrecompoundInterface;
class G4VComponentCrossSection;
class G4ComponentGGNuclNuclXsc;


class eASTIonPhysics: public G4VPhysicsConstructor
{
  public:
    eASTIonPhysics();
    ~eASTIonPhysics();

    virtual void ConstructParticle() override;
    virtual void ConstructProcess() override;
    virtual void TerminateWorker() override;

  private: 
    G4TheoFSGenerator* ftfp;
    G4FTFModel* stringModel;
    G4ExcitedStringDecay* stringDecay;
    G4LundStringFragmentation* fragModel;
    G4GeneratorPrecompoundInterface* preCompoundModel;

    G4VComponentCrossSection* theGGNuclNuclXS; 
    G4ComponentGGNuclNuclXsc* ionGGXS;
};

#endif
