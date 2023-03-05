# Python bindings for eAST

This folder serves as the development area for Python bindings to the eAST C++ code.
In the course of prior investigation, two promising solutions were compared:
* pybind11
* cppyy

In general, the latter option requires substantially less additional code.
The `cppyy` software does some parsing using [Cling](https://github.com/vgvassilev/cling)
