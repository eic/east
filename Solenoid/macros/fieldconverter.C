// Convert ECCE's root-based field map to .Bmap format
// r, z, B_r, Bz
// Leave second argument empty to just replace .root with .Bmap
// run with root -l -q fieldconverter.C

#include <TString.h>
#include <TFile.h>
#include <TTreeReader.h>
#include <TTreeReader.h>

#include <fstream>

int fieldconverter( TString infilename="sPHENIX.2d.root",
		    TString outfilename ="",
		    float scalefactor = -1.4 / 1.5 // BaBar -> sPHENIX
		    ){
  if (outfilename==""){
    outfilename=infilename;
    outfilename.ReplaceAll(".root",".Bmap");
  }

  auto infile = TFile::Open(infilename);
  TTreeReader reader("fieldmap", infile);
  TTreeReaderValue<float> r(reader, "r");
  TTreeReaderValue<float> z(reader, "z");
  TTreeReaderValue<float> br(reader, "br");
  TTreeReaderValue<float> bz(reader, "bz");

  ofstream outfile;
  outfile.open(outfilename);
  
  while (reader.Next()){
    outfile << *r << '\t'
	    << *z << '\t'
	    << *br *scalefactor  << '\t'
	    << *bz *scalefactor  << endl;
  }
  
  return 0;
}
