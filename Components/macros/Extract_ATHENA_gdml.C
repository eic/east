#include <TFile.h>
#include <TGeoManager.h>
#include <TGeoNode.h>
#include <TGDMLWrite.h>

#include <string>

int Extract_ATHENA_gdml( string subsys="all", const string outbase="athena_")
{
  TFile * file = new TFile("detector_geometry.root","READ");
  gGeoManager = (TGeoManager*) file->Get("default");

  // need to switch to root units 
  // (unclear why, but needed for corectness)
  gGeoManager->LockDefaultUnits(false);
  gGeoManager->SetDefaultUnits(TGeoManager::kRootUnits);

  TGDMLWrite *writer = new TGDMLWrite;
  writer->SetG4Compatibility(true);
  writer->WriteGDMLfile ( gGeoManager, "athena_all.gdml","vg");
  
  vector<string> outnames;
  
  TGeoNodeMatrix* m = (TGeoNodeMatrix*) gGeoManager->GetListOfNodes()->At(0);
  for (int i = 0; i<m->GetNdaughters() ; ++i ){
    TGeoNode* n = (TGeoNode*) m->GetNodes()->At(i);
    string outname = outbase+n->GetName()+".gdml";
    if ( subsys !="all" && n->GetName()!=subsys ) continue;
    writer->WriteGDMLfile ( gGeoManager, n, outname.c_str(),"vg");
    outnames.push_back(outname);
  }

  // Now need to reimport these and add a world_volume
  for ( auto fname : outnames ){
    gGeoManager->LockDefaultUnits(false);
    gGeoManager->SetDefaultUnits(TGeoManager::kRootUnits);
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
  
    TGeoVolume *top = gGeoManager->MakeBox("world_volume", Air, 3000., 3000., 10000.);
    top->AddNode( gGeoManager->GetTopVolume(), 1 );
    gGeoManager->SetTopVolume(top);
    gGeoManager->Export(fname.c_str());
  }

  return 0;
}
