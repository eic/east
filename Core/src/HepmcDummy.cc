#include "HepmcDummy.hh"

void HepMCDummyFunction( const HepMC3::GenParticlePtr& p ){
  // do nothing, essentially
  auto v = p->production_vertex();
  return;

#ifdef eAST_USE_HepMC3
  // all is well
#else
  generating a compile time error here, this should never happen;
#endif
  
}
