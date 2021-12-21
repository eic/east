---
title: "Open Tasks"
layout: base
name: tasks
---

<h1> Open Tasks</h1>

---

* TOC
{:toc}

---

## CAD import

* MOU with INFN as we plan to use their converter (Makoto, Andrea, Markus)
* Verify that we can import the latest CAD files from the EIC project
* Make the latest GDML files available as part of the east-geometry repository with Git LFS support
* Document the CAD import (Wouter, Makoto, Maxim)
* Test the silicon tracker CAD model by James Fast (not for now)

## Fast Simulations

* Create an example calorimeter geometry in eAST
  * 12/01/21 kk: Done, can get (components of) ECCE and ATHENA, see [Examples](examples).
  * Periodically make ECCE and ATHENA gdml files available in eAST repository (Kolja)
* Give an example for fast simulations in eAST, perhaps for one region (Kolja, Maxim, Makoto)

## Full Simulations

* Afterburner (Markus will discuss with Jin, Makoto)
  * Including option to turn off pre-assigned decay procedure
* Time structure (Makoto)
  * For streaming readout study
* Implementation of detector components
  * Separate file (either C++ or GDML) for each component
  * Associated sensitive detector is preferred but not mandatory
  * Define detector regions
* Import detector components from ATHENA or ECCE in eAST, including sensitive detectors if available
* Physics List
  * Migration to Geant4 version 11.0 (Makoto)
  * Validation with CLAS12 simulations (Maurizio)
  * Add options (e.g. NeutronHP, Optical photon)
* Validation
  * Ask for test-beam data (Markus, Torre)
  * Can we work with the test setup from the CUA calorimeter tests?

## Documentation

* Document key eAST features on the website
* Document how to reuse existing detector simulation in eAST
* Prepare an online tutorial

## Promotion

* Select proper graphics including event display snapshots to demonstrate eAST capabilities
