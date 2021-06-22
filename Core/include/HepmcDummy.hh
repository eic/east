#ifndef HEPMCDUMMY_H_
#define HEPMCDUMMY_H_

#include <HepMC3/GenEvent.h>
#include <HepMC3/GenVertex.h>
#include <HepMC3/GenParticle.h>

void HepMCDummyFunction( const HepMC3::GenParticlePtr& p, std::unique_ptr<erhic::EventHepMC>& mEvent );

#endif  // HEPMCDUMMY_H_
