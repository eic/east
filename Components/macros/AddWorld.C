int AddWorld(){
  gGeoManager->LockDefaultUnits(false);
  gGeoManager->SetDefaultUnits(TGeoManager::kRootUnits);
  gGeoManager->Import("athena_EcalBarrelImaging_26.gdml");
  
  // TGeoMedium *Vacuum = gGeoManager->GetMedium("Vacuum");
  // if ( !Vacuum ){
  //   TGeoMaterial *matVacuum = new TGeoMaterial("Vacuum", 0,0,0);
  //   Vacuum = new TGeoMedium("Vacuum",1, matVacuum);  
  // }

  TGeoMedium *Air = gGeoManager->GetMedium("Air");
  if ( !Air ){
    TGeoElementTable *table = gGeoManager->GetElementTable();
;

    TGeoMixture *air = new TGeoMixture("air",4, 0.00120479);
    air->AddElement(table->GetElement(6),0.00012);
    air->AddElement(table->GetElement(7),0.754);
    air->AddElement(table->GetElement(8),0.234);
    air->AddElement(table->GetElement(18),0.012827);

    //   TGeoMaterial *matAir = new TGeoMaterial("Air", 0,0,0);
  // should be
  // <D unit="g/cm3" value="0.0011999999999999999"/>
  //  <fraction n="0.012000000104308128" ref="Ar_elm"/>
  //  <fraction n="0.75400000810623169" ref="N_elm"/>
  //  <fraction n="0.23399999737739563" ref="O_elm"/>
    Air = new TGeoMedium("Air",0, air);  
  }

   TGeoVolume *top = gGeoManager->MakeBox("world_volume", Air, 3000., 3000., 10000.);
   top->AddNode( gGeoManager->MakeBox("world_volume", Air, 3000., 3000., 10000.), 1 );
   top->AddNode( gGeoManager->GetTopVolume(), 1 );
   gGeoManager->SetTopVolume(top);
   gGeoManager->Export("test.gdml");

   return 0;
}
