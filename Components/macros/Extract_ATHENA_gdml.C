#ifndef EXTRACT_ATHENA_GDML_C
#define EXTRACT_ATHENA_GDML_C

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
  // writer->WriteGDMLfile ( gGeoManager, "athena_all.gdml","vg");

  
  TGeoNodeMatrix* m = (TGeoNodeMatrix*) gGeoManager->GetListOfNodes()->At(0);
  
  auto top = (TGeoVolume*) gGeoManager->GetTopVolume()->Clone();
  auto n1 = (TGeoNode*) m->GetNodes()->At(1);
  auto t1 = (TGeoVolume*) n1->GetVolume()->Clone();
  top->Draw();

  // gGeoManager = new TGeoManager("helper", "World plus one subsys");
  // gGeoManager->SetTopVolume(top);
  // gGeoManager->Print();
  // top->Draw();
  //top->AddNode(n1);
  // gGeoManager->AddVolume(t1);
  // t1->Print();
  // top->Print();
  // n1->Print();
  // writer->WriteGDMLfile ( gGeoManager, "test.gdml","vg");


  // for (int i = 0; i<m->GetNdaughters() ; ++i ){
  //   TGeoNode* n = (TGeoNode*) m->GetNodes()->At(i);
  //   string outname = outbase+n->GetName()+".gdml";
  //   if ( subsys !="all" && n->GetName()!=subsys ) continue;
  //   cout << n->GetName() << " " << n->GetVolume()->IsAssembly() << endl;
  //   writer->WriteGDMLfile ( gGeoManager, n, outname.c_str(),"vg");
  // }

  // string outname = outbase+subsys+".gdml";
  // for (int i = 0; i<m->GetNdaughters() ; ++i ){
  //   TGeoNode* n = (TGeoNode*) m->GetNodes()->At(i);
  //   // if ( subsys !="all" && n->GetName()!=subsys ) m->GetNodes()->RemoveAt(i);
  //   // gGeoManager->ClearShape(n);
  //   cout << n->GetName() << " " << n->GetVolume()->IsAssembly() << endl;
  //   // writer->WriteGDMLfile ( gGeoManager, n, outname.c_str(),"vg");
  // }
  // writer->WriteGDMLfile ( gGeoManager, outname.c_str(),"vg");



  return 0;
}
#endif
