#include "HepmcDummy.cc"


void HepMCDummyFunction( const HepMC3::GenParticlePtr& p, std::unique_ptr<erhic::EventHepMC>& mEvent ){
  // do nothing, essentially
  auto v = p->production_vertex();
  auto e = mEvent.get();
  return;
}
