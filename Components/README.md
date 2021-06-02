# eAST detector components

## Overview
*eAST* (eA simulation toolkit) assumes each detector component is descrived by a concrete class derived from _eASTVDetectoComponent_ base class.
Base classes and utility classes that should be used by a detector component are stored in `/Components/Base` sub-directory.

All volumes of a detector component should be fully contained by a mother volume that represents the component, and this mother volume must have a unique region.

## eASTVDetectorComponentbase class

 

