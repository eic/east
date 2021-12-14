---
title: "Examples"
layout: base
name: examples
---

# Examples

---

* TOC
{:toc}

---


## ECCE geometry import

1. Prepare the ECCE environment. Detailed instructions are
[here](https://github.com/eic/Singularity/blob/master/README.md), but both on
BNL and JLAB farms you should only need, for csh,
```sh
source /cvmfs/eic.opensciencegrid.org/ecce/gcc-8.3/opt/fun4all/core/bin/ecce_setup.csh -n
```
or, for bash,
```sh
source /cvmfs/eic.opensciencegrid.org/ecce/gcc-8.3/opt/fun4all/core/bin/ecce_setup.sh -n
```

1. Obtain the latest detector Implementation:
```sh
mkdir ecce
cd ecce
git clone https://github.com/ECCE-EIC/macros.git
```
1. Copy (or soft-link) the script included in eAST to extract individual components.
For this example, we will assume that eAST is installed in `/home/eicuser/east/`,
please adjust accordingly.
```sh
cd macros/detectors/EICDetector
cp /home/eicuser/east/Components/ECCE/Extract_ECCE_gdml.C .
```
Optional: If you know or suspect that subsystems have been added or significantly
changed recently, compare this macro to `Fun4All_G4_EICDetector.C`.
`Extract_ECCE_gdml.C` is largely a copy of that segment, just grouped with
sensible names, e.g.
```c++
if ( subsys=="tracking" || subsys=="all" ) {
  if (outname=="") outname = "ecce_" + subsys + ".gdml";
  Enable::RWELL = true;
  // barrel tracker
  Enable::TrackingService = true;
  // Enable::TrackingService_VERBOSITY = INT_MAX - 10;
  Enable::BARREL = true;
  // fst
  Enable::FST = true;
}
```
4. Finally, extract the subsystem gdml descriptions. The script can be called with
the name of a subsystem as argument:
```sh
root Extract_ECCE_gdml.C\(\"tracker\"\)
```
You can use a simple shell script to gt them all at once. For `csh`, use
```sh
foreach s ( all pipe magnet magfarfwd magfarbwd gems tracking tofs becal hcalin hcalout dirc femc drcalo lfhcal eemc ehcal )
    root -l -b -q Extract_ECCE_gdml.C\(\"$s\"\)
end
```
and for `bash` use
```sh
for s in all pipe magnet magfarfwd magfarbwd gems tracking tofs becal hcalin hcalout dirc femc drcalo lfhcal eemc ehcal
do
    root -l -b -q Extract_ECCE_gdml.C\(\"$s\"\)
done
```
1. The `ecce.mac` macro in eAST expects these gdml files in a specific location.
You can adjust the paths in the macro, but we recommend moving the gdml files:
```sh
mv *.gdml /home/eicuser/east/Components/ECCE/
```

You can now run eAST in batch mode,
```sh
cd /home/eicuser/east/build
./eAST ecce.mac
```
or interactively:
```sh
cd /home/eicuser/east/build
./eAST
/control/execute ecce.mac
/run/beamOn 10
```

A closer look at `ecce.mac` shows that components are added via
```
/eAST/component/GenericGDML1 1
/eAST/component/GenericGDML1/gdmlFile Components/ECCE/ecce_becal.gdml
/eAST/component/GenericGDML2 1
/eAST/component/GenericGDML2/gdmlFile Components/ECCE/ecce_hcalin.gdml
...
```
As of now, up to 5 generic components an be added this way. An arbitrary number,
but in the near future we want to switch to less generic classes anyway.

**WARNING**: An invocation of `make install` will overwrite any changes you make to the `*.mac`
by automatically copying the originals from `Core/`, so rename them or copy them.

#### Notes
* If you want to regenerate the list of subsystems, you can use
```sh
grep 'if ( subsys==' Extract_ECCE_gdml.C | cut -d'"' -f 2
```
* Further reading for [ECCE software/fun4all](https://ecce-eic.github.io/)
* If you ever need to regenerate the magnetic field map, the recipe is similar but easier:
```
source /cvmfs/eic.opensciencegrid.org/ecce/gcc-8.3/opt/fun4all/core/bin/ecce_setup.csh -n
# or source /cvmfs/eic.opensciencegrid.org/ecce/gcc-8.3/opt/fun4all/core/bin/ecce_setup.sh -n
git clone git@github.com:ECCE-EIC/calibrations.git
cd calibrations/Field/Map/
cp /home/eicuser/east/Solenoid/macros/fieldconverter.C .
root -l -b -q fieldconverter.C
mv sPHENIX.2d.Bmap  /home/eicuser/east/Solenoid/data/
```
