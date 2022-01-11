#include <TFile.h>
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGDMLWrite.h>
#include <RVersion.h>

#include <G4GDMLWriteStructure.hh>

#include <iostream>
#include <string>
#include <vector>
#include <utility> // for pair

using std::cout;
using std::cerr;
using std::endl;

using std::string;
using std::vector;
using std::pair;

// string subsys="all", const string outbase="athena_")
int main( int argc, char *argv[])
{
  string rootgeo = "./detector_geometry.root";
  string subsys="all";
  string outbase="athena_";
  switch (argc){
  case 4 : 
    outbase = argv[3];
    // fallthrough
  case 3 : 
    subsys = argv[2];
    // fallthrough
  case 2 : 
    rootgeo = argv[1];
    // fallthrough
  case 1 : 
    break;
  default :
    cerr << "Usage: Extract_ATHENA_gdml [infile] [subsys] [prefix]" << endl;
    return -1;
    break;    
  }


  TFile * file = TFile::Open(rootgeo.c_str(),"READ");
  if (!file){
    cout << "Can't open file " << rootgeo << endl;
    return -1;
  }
  gGeoManager = (TGeoManager*) file->Get("default");
  
  
  // need to switch to root units 
  // This flexibility was added in 6.22 patches 
  // And seems to have changed the default
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,24,00)
  gGeoManager->LockDefaultUnits(false);
  gGeoManager->SetDefaultUnits(TGeoManager::kRootUnits);
#endif
    
  TGDMLWrite *writer = new TGDMLWrite;
  writer->SetG4Compatibility(true);
  writer->WriteGDMLfile ( gGeoManager, "athena_all.gdml","vg");
  
  vector< pair<string,string> > names;
  
  TGeoNodeMatrix* m = (TGeoNodeMatrix*) gGeoManager->GetListOfNodes()->At(0);
  for (int i = 0; i<m->GetNdaughters() ; ++i ){
    TGeoNode* n = (TGeoNode*) m->GetNodes()->At(i);
    string outname = outbase+n->GetName()+".gdml";
    // cout << n->GetDaughter(0)->GetName() << endl; return 0;
    if ( subsys !="all" && n->GetName()!=subsys ) continue;
    writer->WriteGDMLfile ( gGeoManager, n, outname.c_str(),"vg");
    names.push_back(pair(n->GetName(),outname));
  }

  // Now need to reimport these and add a world_volume
  for ( auto name : names ){
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,24,00)
    gGeoManager->LockDefaultUnits(false);
    gGeoManager->SetDefaultUnits(TGeoManager::kRootUnits);
#endif
    auto fname = name.second;
    gGeoManager->Import(fname.c_str());

    TGeoMedium *Air = gGeoManager->GetMedium("Air");
    if ( !Air ){
      TGeoElementTable *table = gGeoManager->GetElementTable();
      TGeoMixture *air = new TGeoMixture("air",4, 0.00120479);
      air->AddElement(table->GetElement(6),0.00012);
      air->AddElement(table->GetElement(7),0.754);
      air->AddElement(table->GetElement(8),0.234);
      air->AddElement(table->GetElement(18),0.012827);
      Air = new TGeoMedium("Air",0, air);  
    }
    string worldname = "world_volume_"+name.first;
    TGeoVolume *top = gGeoManager->MakeBox(worldname.c_str(), Air, 3000., 3000., 10000.);
    top->AddNode( gGeoManager->GetTopVolume(), 1 );
    gGeoManager->SetTopVolume(top);
    // writer->WriteGDMLfile ( gGeoManager, fname.c_str(),"vgf");
    //  gGeoManager->Export(fname.c_str(), "vgf");

    G4GDMLWriteStructure gdml_parser;
    xercesc::XMLPlatformUtils::Initialize();
    gdml_parser.Write(fname, top,
    		      "http://service-spi.web.cern.ch/service-spi/app/releases/GDML/schema/gdml.xsd",
    		      0, true);
    xercesc::XMLPlatformUtils::Terminate();

  }

  return 0;
}
