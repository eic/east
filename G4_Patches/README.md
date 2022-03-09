# eAST-specific patch for Geant4

This directory holds eAST-specific patches that should be applied to each specific version of Geant4.
Each sub-directory corresponds to the specific version of Geant4 and thus should not be applied to ealier or newer versions.
Please note that the patch is not incremental. It should directly be applied to the fresh release version of Geant4.

Each sub-directory has a tar-file that should be expanded at the directory where you see "source". For example of Geant4 version 11.0.p01

   % cd .../geant4-v11.0.1
   % ls
   CMakeLists.txt		LICENSE			ReleaseNotes		config			examples		
   CONTRIBUTING.rst	README.rst		cmake			environments		patch-v11.0.1		source
   % cp .../east/G4_Patches/G4.v11.0.p01/patch-v11.0.1.tar .
   % tar xf patch-v11.0.1.tar
   
Then, you need to re-compile and re-install the entire Geant4.
