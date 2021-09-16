---
title: "Installation"
layout: base
name: installation
---

# Installing eAST

*eAST* runs on top of the latest public version of Geant4 (currently [version 10.7.p02](https://geant4.web.cern.ch/support/download)). You need to install Geant4 in advance to installing eAST with the following options. Please refer to [the Geant4 installation guide](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html).
- GEANT4_BUILD_MULTITHREADED
- GEANT4_USE_GDML (it requires _Xerces-C++_ pre-installed)
- GEANT4_USE_OPENGL_X11 (if you use event display. it requires _X11_ and _OpenGL_ libraries)
- GEANT4_USE_QT (if you use Qt window. it requires _Qt5_ and _OpenGL_ libraries)

Building is done via cmake, using the `CMakeLists.txt` file at the top level. It is recommended to use a separate build directory. Please make sure all the necessary Geant4 environment variables are set in advance.
```
cd east
cmake -S . -B build
cmake --build build
```

Or alternatively,
```
cd east
mkdir build
cd build
cmake ..
make
```

Important note: During this configuration proccess, additional files may be downloaded, so internet access is required.

## Configuration options

To customize an installation location, use the `CMAKE_INSTALL_PREFIX` option:
```
cmake -DCMAKE_INSTALL_PREFIX=/path/to/installdir -S . -B build
cmake --build build
cmake --install build
```
This will copy libraries, binaries, data, and cmake configuration files into a central location. Note that you probably want to ensure that (in this example) `/path/to/installdir/bin` is in the `$PATH` environment variable and `/path/to/installdir/lib` in the `$LD_LIBRARY_PATH` one.

## HepMC support

cmake will pick up an existing HepMC3 installation in default spots, or allow using a specified location with the `HepMC3` option:
```c++
cmake -DCMAKE_INSTALL_PREFIX=/path/to/installdir -DHepMC3=~/path/to/HepMC3dir/  -S . -B build
```

