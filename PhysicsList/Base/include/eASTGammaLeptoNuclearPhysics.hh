////////////////////////////////////////////////////////////////////////////////
//                                                                       
//  eASTGammaLeptoNuclearPhysics.hh    
//  Description: Gamma-nuclear, electro-nuclear and muon-nuclear physics  
//                 constructor for eASTPhysicsList      
//                                                      
//    Jun.21.2018 : original implementation - Dennis H. Wright (SLAC)
//    May.06.2021 : migration to eAST - Makoto Asai (SLAC)
//                                                       
////////////////////////////////////////////////////////////////////////////////

#ifndef eASTGammaLeptoNuclearPhysics_h
#define eASTGammaLeptoNuclearPhysics_h 1

#include "G4VPhysicsConstructor.hh"
#include "G4GammaParticipants.hh"
#include "G4QGSModel.hh"

class G4TheoFSGenerator;
class G4ExcitedStringDecay;
class G4QGSMFragmentation;
class G4GeneratorPrecompoundInterface;


class eASTGammaLeptoNuclearPhysics: public G4VPhysicsConstructor
{
  public:
    eASTGammaLeptoNuclearPhysics();
    ~eASTGammaLeptoNuclearPhysics();

    virtual void ConstructProcess() override;
    virtual void ConstructParticle() override;

  private: 
    G4TheoFSGenerator* qgsp;
    G4QGSModel<G4GammaParticipants>* stringModel;
    G4ExcitedStringDecay* stringDecay;
    G4QGSMFragmentation* fragModel;
    G4GeneratorPrecompoundInterface* preCompoundModel;
 
};

#endif
