project (Extract_ATHENA_gdml)
find_package(ROOT 6.18 CONFIG)
if(ROOT_FOUND)
  include_directories(${Geant4_INCLUDE_DIR})
  add_executable(Extract_ATHENA_gdml Extract_ATHENA_gdml.cc )
  
  target_link_libraries(Extract_ATHENA_gdml 
    PUBLIC
    ROOT::Physics ROOT::Core 
    ROOT::RIO ROOT::Geom ROOT::Gdml
    ${Geant4_LIBRARIES}    
    )

  # Bit of a catch 22 here. Newer root versions need tbb, 
  # older ones have it included. I'll try just linking it if it's found
  # but that may have to be refined.
  find_package( TBB CONFIG  )
  if ( ${TBB_FOUND} )
    message ( "-- Found TBB version " ${TBB_VERSION} )
    target_link_libraries(Extract_ATHENA_gdml 
      PUBLIC
      TBB::tbb
      )
  else()
    message ( "-- TBB not found, that's probably okay" )
  endif()

endif()
