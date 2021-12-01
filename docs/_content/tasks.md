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

* MOU with INFN as we plan to use their converter
* Verify that we can import the latest CAD files from the EIC project
* Make the latest GDML files available as part of the east-geometry repository with Git LFS support
* Document the CAD import
* Test the silicon tracker CAD model by James Fast

## Fast Simulations

* Create an example calorimeter geometry in eAST
  * 12/01/21 kk: Done, can get (components of) ECCE and ATHENA, see [Examples](examples).
* Give an example for fast simulations in eAST, perhaps for one region (Kolja, Maxim, Makoto)

## Full Simulations

* Afterburner (Markus will discuss with Jin)
  * Including option to turn off pre-assigned decay procedure
* Implementation of detector components
  * Separate file (either C++ or GDML) for each component
  * Associated sensitive detector is preferred but not mandatory
  * Define detector regions
* Import detector components from ATHENA or ECCE in eAST, including sensitive detectors if available
* Physics List
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
