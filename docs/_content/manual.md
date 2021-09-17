---
title: "User Manual"
layout: base
name: manual
---

# Running eAST

*eAST* runs both in batch mode and interactive mode. To run it in batch mode, execute the application with a macro file,
```bash
$ ./eAST <run.mac>
```
while to run it in interactive mode, execute the application without a macro file.
```
$ ./eAST
```
and type UI commands once command field of the Qt window becomes available.

*eAST* reads several GDML files. GDML requires GDML schema. All methods that read gdml have an option to verify that is by default turned off since validation requires an internet connection at run-time.
<!--By default, schema file is downloaded while reading a GDML file from [CERN site](http://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd) so that you must execute *eAST* while you are connected online.  
Alternatively, you may download the schema file in advance and modify the second line of each GDML file accordingly. Please refer to [the GDML manual](http://gdml.web.cern.ch/GDML/). -->

# eAST UI commands

*eAST* is controlled by UI commands, and it works both in interactive mode with Qt window and in batch mode with an input macro file.
UI commands starting with `/eAST/` are implemented in *eAST* code and thus they are not available in Geant4 itself (or other Geant4 applications). Some of these *eAST*-specific commands are listed here.
Commands that do not start with `/eAST/` are provided in Geant4 version 10.7 that can be found in [the Geant4 user's guide](https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Control/commands.html).
For demonstration purposes, *eAST* includes a macro file `run.mac` that can be used for both interactive mode and batch mode.

# Initialization

*eAST* has to be initialized by the following command.
```
/gorad/initialize
```
This command internally calls `/run/initialize` to initialize _G4MTRunManager_, so that the user should *not* use `/run/initialize`. Prior to this initialization command, *eAST* needs the following to be defined:
- Simulation geometry
- Choice of Physics models

Other simulation settings, e.g. primary generator settings as well as scoring and histogramming should be defined after *eAST* is initialized.

## Simulation geometry

Prior to the initialization, each detector component has to be activated by its dedicated UI command. Once a component is activated, its specific commands become available. Please refer to the README file associated with each detector component for its specific UI commands.

As an example, following are the UI commands for the beampipe component.
```
# activating "beampipe"
/eAST/component/beampipe <verboseLevel>
# specifying input GDML files
/eAST/component/beampipe/envelopeGdmlFile <envelopeGdmlFile>
/eAST/component/beampipe/gdmlFile <gdmlFile>
# specifying input material definition file
/eAST/component/beampipe/materialFile <materialDefinitionFile>
```

Important: You can use paths relative to the installation location, for example:
```
# specifying input GDML files
/eAST/component/DIRC_support/gdmlFile Components/Beampipe/data/DIRC_support_06-04-21.gdml
```
will be expanded to
```
/path/to/installdir/share/Components/Beampipe/data/DIRC_support_06-04-21.gdml
```
if the file or folder is not found in the working directory first.

## Physics options

Prior to the initialization, optional physics processes have to be defined.

To add radioactive decay process
```
/eAST/physics/addRDM  
```

To add optical photon processes
```
/eAST/physics/addOptical
```
In addition to this command, optical properties of the materials and surfaces have to be defined to the volumes where optical photons should be tracked.

To add a user-specific step limitation process
```
/eAST/physics/addStepLimit <particle>
```
where _particle_ could be "_charges_" (default), "_neutral_", "_all_" or "_e+/-_". In addition to this command, maximum step lenth(s) have to be defined after initialization (see the following section).

# UI commands available after initialization

## Production cuts and artificial step limits

Production thresholds (a.k.a. cuts) for electron, positron, gamma and proton are by default set to 7 mm. They can be changed by :
```
/eAST/physics/cuts/setCuts <length> <unit>
```
To define a specific value for a region :
```
/eAST/physics/cuts/setRegionCuts <regionName> <length> <unit>
```
In general, a step of a track is limited either by a volume boundary crossing or discrete physics interaction (e.g. decay, inelastic scattering, etc.).
Artificial limitation of the maximum step length applied to a specified particle type may be defined. Maximum step length could be assigned to a region. Artificial step limitation is useful for particles generating optical photons.
```
/eAST/physics/limit/regionStepLimit <regionName> <length> <unit>
```

## Geometry sanity-check commands

To display defined solid, logical volumes, physical volumes and regions :
```
/eAST/geometry/listSolids <verboseLevel>
/eAST/geometry/listLogicalVolumes <verboseLevel>
/eAST/geometry/listPhysicalVolumes <verboseLevel>
/eAST/geometry/listRegions <verboseLevel>
```

To check volume overlap for a physical volume :
```
/eAST/geometry/checkOverlap <physVolName> <nSpots> <maxError> <tolerance> <unit>
```

# UI commands for histogramming particle flux

While *eAST* relies upon sensitive detector classes of each (sensitive) detector components for creation of outfut "hit" files, it also has a mechanism of measuring particle flux and creating histogram. This measuring and histogramming functionality uses "probe" functionality of Geant4 that is newly introduced with Geant4 version 10.7.

## Defining probe

Please note that this section descrives a Geant4 functionality that is not specific to *eAST*. This functionality is newly introduced with Geant4 version 10.7.

This functionality allows the user to locate scoring “probes” at arbitrary locations. A “probe” is a virtual cube, to which any Geant4 primitive scorers could be assigned. This is a concept alternative to a scoring mesh, where cells of three-dimensional mesh are touching each other.
```
/score/create/probe <probeName> <halfWidth> <unit>
```

The user can locate an arbitrary number of probes by repeating /score/probe/locate commands, but all of these probes are the same shape. Given these probes are located in an artificial “parallel world”, probes may overlap to the volumes defined in the mass geometry, as long as probes themselves are not overlapping to each other or protruding from the world volume.
```
/score/probe/locate <x> <y> <z> <unit>
```

Once a probe is defined, the user can associate arbitrary number of primitive scorers and filters like the ordinary scoring mesh. All probes have the same scorers but score individually. The following is a sample macro to locate 4 probes and score total flux and proton flux for each of these probes.
```
/score/create/probe Probes 1 cm
/score/probe/locate 0 0 0 cm
/score/probe/locate 25 0 0 cm
/score/probe/locate 0 25 0 cm
/score/probe/locate 0 0 25 cm
/score/quantity/flatSurfaceFlux volFlx
/score/quantity/flatSurfaceFlux protonFlux
/score/filter/particle protonFilter proton
/score/close
```
Please note that the size of the probe is specified by half width. In the above sample, the actual size of the probe is 2*2*2 cm^3.
If probes are placed more than once, scores are dumped separately for each probe, with the order of the `/score/probe/locate` command.

After a simulation run, scored data can be dumped into a file by usual Geant4 `/score/dump` commands. Also, histograms are created by `/eAST/analysis/` commands descrived in the following section.

## 1-D energy spectrum histogram directly filled by a primitive scorer

The following UI command creates a 1-D energy spectrum histogram that is directly filled by the associated primitive scorer.
```
/eAST/analysis/1D/spectrum <probeName> <scorerName>
```
where, _probeName_ is the name of the probe defined by `/score/create/probe` command described in the previous sections.
The x-axis of the histogram defined by this command is the kinetic energy of the track.
If probe is located more than once, histograms are created and filled separately to each probe with their sequential order of `/score/probe/locate` command.
`/eAST/analysis/1D/spectrum` command is available only for primitive scorers that score flux.

`/eAST/analysis/1D/spectrum`command has to be immediately followed by `/eAST/analysis/1D/config` command to specify the histogram parameters.
```
/eAST/analysis/1D/config <nBin> <minVal> <maxVal> <unit> <scale> <logVal>
```
Details of each parameters should be consulted to onlne help. <scale>_is for the x-axis and it can be either “linear” or “log”. Binning of the histogram is determined by this parameter so that scale cannot be changed once configured. For the y-axis, linear-/log-scale can be chosen with the following command.
```
/eAST/analysis/1D/yaxisLog <flag>
```
Please note that the y-axis scale is used for plotting this histogram to a PostScript file and does not affect the dumped data.

At the end of a run, each histogram is dumped to a separate file. File name should be specified by this command
```
/eAST/analysis/file <fileName>
```
but the actual file name will be appended by the histogram type and scorer name, and also by copy number of the probes.
By default, *eAST* dumps histograms in CSV format. The file format of the histogram file is described in [the g4tools section of Geant4 Application Developers Guide](https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/Analysis/g4tools.html).
If another format (Root or xml) is preferred, `eASTAnalysis.hh` has to be modified accordingly and *eAST* has to be recompiled.

In addition to dumping to CSV files, histograms can be plotted to a PostScript file if `/eAST/analysis/plot` command is set. _fileName_ is also used for this PostScript file.
