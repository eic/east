# Python bindings for eAST

## About
This folder serves as the development area for Python bindings to the eAST C++ code.
In the course of prior investigation, two promising solutions were compared:
* pybind11
* cppyy

In general, the latter option requires substantially less additional code.
The `cppyy` software parses the C++ header files at runtime using [Cling](https://github.com/vgvassilev/cling).
This is the opposite of what `pybind11` does, which is to build a shared library that can be loaded
into a running Python application with no additional mapping of symbols necessary.

## Setting up

At the time of writing this work is done using Python 3.10, in a virtual environment.
The `cppyy` package is pip-installable. For it to work the environment variable `CPATH`
needs to contain location of relevant header files, such as Geant4 header files.
Example:
```bash
# NB. The location of the include files will vary depending on your installation
export CPATH=/opt/geant4/include/Geant4/
```

