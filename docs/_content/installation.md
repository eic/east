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

### Installation

__eAST__ runs on top of the latest public version of Geant4, so before building eAST you need to install __Geant4__.
At the time of writing, the current Geant4 version is 11.2.0. You can access [this link](https://geant4.web.cern.ch/support/download){:target="_blank"}
to access download pages for this and other versions of Geant4, for installation. Comments:
* Please refer to [the Geant4 installation guide](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/index.html){:target="_blank"}
and follow all instructions. In many cases (e.g. if the platform is Debian) Geant4 will need to be
[installed from source](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/installguide.html){:target="_blank"}.
* The process involves using `cmake`, and it's important that you use a proper _build directory_ as recommended in the installation guide.
__Please see the "_CMake options_" section below for details on options.__
* Before you commence the build please make sure that the compilers i.e. `gcc` and `g++` are in the $PATH
and their versions comply with GEANT requirements.
* `cmake` will produce helpful diagnostics if some dependencies are missing, in which case these
will need to be installed (see the table and Appendix below). In case of this and other errors when
using `cmake` the safest option is clean up the "build" directory and
start the build procedure from scratch.
* During the build process certain files will be downloaded automatically from remote servers,
so make sure your computer is online at that time.
* If the __install path__ is in a folder to which the user doesn't have _write_ access (e.g. under "opt"), one may need to use root/chown or `sudo` to get necessary privileges.
* There is a [post-installation step](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/postinstall.html){:target="_blank"} that should be followed to set the environment variables important for Geant4 operation.


### CMake options

*In order to successfuly build eAST it is mandatory that you set the options listed below when using `cmake` to build Geant4*.
As you can see from the table, if you start from scratch you may need to install the prerequisits first such as
<em>X11, Qt5, OpenGL and Xerces-C++</em> libraries.
Please see the [Appendix](/content/installation.html#appendix-geant4-dependencies) for comments and some practical
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

In certain cases (not very common) the user may need to build a version of Geant4 where
multithreading is disabled. One such cases is creation of Python bindings on certain platforms.
In such cases, the following option will need to be added:

```bash
-DGEANT4_BUILD_MULTITHREADED=OFF
```


---

## Setting up eAST

### Build and Install
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

### HepMC3

Many use cases involve using the HepMC3 libraries so it is effectively a prerequisite for eAST.
This is also implied in the build option illustrated in the previous section (cf. `-DHepMC3`).
Please visit
[the HepMC3 page](https://gitlab.cern.ch/hepmc/HepMC3/-/tree/master/){:target="_blank"}
if you need to install HepMC3 from binaries or build it from source. When downloading the tar
file as per instructions, it is possible that the link is stale, so the version number will
need to be adjusted to point to an existing file.

### Final steps

Once the HepMC3 installation is complete (or is already in place), in order for eAST to work,
the environment variable `LD_LIBRARY_PATH` will need to be updated to include the
path the the HepMC3 libraries. This can be done after sourcing the Geant4
 [post-installation script](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/postinstall.html){:target="_blank"}.

---

## Appendix (GEANT4 dependencies)

### Xerces

The [Xerces build and intallation webpage](https://xerces.apache.org/xerces-c/build-3.html#UNIXl){:target="_blank"}
mentions cmake -- although the latter is not explicitly invoked, and you only need to run `configure` and `make`.
Read the web page carefully.

On some flavors of Linux, e.g. _Rocky_, care must be taken to ensure the `icu` package is correctly installed.
It appears to happen transparently on Debian.

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

Recent versions of Geant may require one more dependency:
```bash
# On Debian platforms
sudo apt-get install -y qt3d5-dev 
```

Running Geant4 applications equipped with GUI based on the Qt package in the WSL2 environment
may result in a blank graphic window. One solution to that is to unset an enironment variable:

```bash
export LIBGL_ALWAYS_INDIRECT=
```



### libxmu

```bash
sudo apt-get install -y libxmu-dev
```

### Passing options to "make"

On many Linux platforms, `cmake` is relying on `make` for the build step.
It is possible in these cases to pass helpful options (such as use multiple cores
for compilation) to `make` by setting an environment variable:

```bash
export MAKEFLAGS=-j4
```


