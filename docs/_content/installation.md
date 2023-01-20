---
title: "Installation"
layout: base
name: installation

tables:
  g4options:
    headers: [Option, Comment]
    rows:
#      - [
#      "GEANT4_BUILD_MULTITHREADED",
#      "-"
#      ]
      - [
      "GEANT4_USE_GDML",
      'Requires <a href="http://xerces.apache.org/xerces-c/" target="_blank"><em>Xerces-C++</em></a> parser.'
      ]
      - [
      "GEANT4_USE_OPENGL_X11",
      "Requires <em>X11</em> and <em>OpenGL</em> libraries."
      ]
      - [
      "GEANT4_USE_QT",
      "Requires <em>Qt5</em> and <em>OpenGL</em> libraries."
      ]
      - [
      "GEANT4_INSTALL_DATA",
      "Installs datasets."
      ]      
---        
<h1>Installation</h1>
* TOC
{:toc}

---

## GEANT4

### Requirements and Procedure

*eAST* runs on top of the latest public version of Geant4 (currently [version 11.0-p01](https://geant4.web.cern.ch/support/download){:target="_blank"}),
so before building eAST you need to install Geant4. The process involves using `cmake`.
Please refer to [the Geant4 installation guide](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html){:target="_blank"}
and make sure you follow all instructions carefully. In particular, it is
important that you use the "build" directory as recommended in the Geant4 guide. If an error was made when
using `cmake` the safest option is to start from clean source i.e. to wipe out the Geant4 directory and
recreate it from the tarball - this helps to avoid possible effects of lingering cmake artefacts.

*In order to successfuly build eAST it is mandatory that you set the options listed below when using `cmake` to build Geant4*.
As you can see from the table, if you start from scratch you may need to install the prerequisits first such as
<em>X11, Qt5, OpenGL and Xerces-C++</em> libraries. Please see the Appendix for comments and some practical
advice about how to proceed.


<table border="1" width="60%">
<tr>
{% for header in page.tables.g4options.headers %}
<th>{{ header }}</th>
{% endfor %}
</tr>
{% for row in page.tables.g4options.rows %}
<tr>
{% for item in row %}
<td>
{{ item }}
</td>
{% endfor %}
</tr>
{% endfor %}
</table>

<p/>

A command line for ```cmake``` using these options might look like this:
```bash
cmake -DCMAKE_INSTALL_PREFIX=/install/path -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_GDML=ON -DGEANT4_USE_QT=ON /path/to/geant/directory
```
**Before you commence the build please make sure that the compilers i.e. `gcc` and `g++` are in the $PATH
and their versions comply with GEANT requirements.**

Certain files maybe downloaded automatically during the build, so make sure your computer is online
suring that process.
If the install path is outside the user folder e.g. under "opt", one may need to use root identity or "sudo" to get the necessary privileges (e.g. to use `chown` etc).
It is a good idea to create such folder (e.g. *geant4.11.00.p01* but the name of course can be different) beforehand and use `chown` to ensure
it's writeable by the user who does the installation, before actually running `make install`.

There is a [post-installation step](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/postinstall.html){:target="_blank"} that should be followed to correctly set the environment variables
important for Geant4 operation.

### The "eAST-specific" Patch

In order to read multiple GDML files through eAST, you will need to apply a specific patch: [https://github.com/eic/east/tree/main/G4_Patches](https://github.com/eic/east/tree/main/G4_Patches){:target="_blank"}.
The patch is safe to apply and it won't alter the default behavior of GEANT4.

---

## Building and Installing eAST

Many use cases involve using the HepMC3 libraries so it is optimal
to consider it a prerequisite. Please visit
[the HepMC3 page](https://gitlab.cern.ch/hepmc/HepMC3/-/tree/master/){:target="_blank"}
if you need to install HepMC3 from binaries or build it from source.


Notes about the build and install procedure:
* It is recommended that the user creates a `build` folder inside the main eAST directory; the example below assumes that this is the case
* During the build proccess, additional files may be downloaded, so internet access is required
* To customize the installation location, use the `CMAKE_INSTALL_PREFIX` option (see below)
* When building eAST `cmake` will attempt to find an existing HepMC3 installation in default locations
* A more reliable method is to specify the correct location with the `HepMC3` option (see below)

The following is a working example of commands for the build and install
procedure (assuming they are run from the eAST directory):

```bash
cmake -DCMAKE_INSTALL_PREFIX=/path/to/installdir -DHepMC3=~/path/to/HepMC3dir/ -S . -B build
cmake --build build
cmake --install build
```

The last command will copy libraries, binaries, data, and cmake configuration files into the
predefined install location.
If you elect to install into a system directory (e.g. under `/opt` etc) please see the notes
in the Geant4 section above, regarding correct permissions.
After installation has completed you probably want to ensure that (in this example) `/path/to/installdir/bin`
is in the `$PATH` environment variable and `/path/to/installdir/lib` in `$LD_LIBRARY_PATH`.

---

## Appendix (GEANT4 dependencies)

### Xerces

The [Xerces build and intallation webpage](https://xerces.apache.org/xerces-c/build-3.html#UNIXl){:target="_blank"}
mentions cmake -- although the latter is not explicitly invoked, and you only need to run `configure` and `make`.
Read carefully.

### Qt

The Qt installation procedure has evolved over the years. Using the GUI installer
provided by the developer appears to frequently cause problems. At the time of writing,
this procedure is working:

```bash
# prerequisites:
sudo apt-get -y install build-essential openssl libssl-dev libssl1.0 libgl1-mesa-dev libqt5x11extras5
# qt:
sudo apt-get install qtbase5-dev
sudo apt-get install qtdeclarative5-dev
```
### libxmu

```bash
sudo apt-get install -y libxmu-dev
```


