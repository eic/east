#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.8)
if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(eAST VERSION 0.1.0)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# You can set eAST_USE_HepMC3 to OFF via the command line or ccmake/cmake-gui
option(eAST_USE_HepMC3 "Build example with HepMC support, requires HepMC3" ON)
if(eAST_USE_HepMC3)
  message ( "---- You can specify a HepMC3 installation using \"-DHepMC3=[path/to/lib]\" " )
  # message ( "---- HepMC3=" ${HepMC3} )
  find_package(HepMC3 REQUIRED HINTS ${HepMC3} ${HepMC3_DIR} )
  message ( "-- HepMC3 library found at ${HEPMC3_LIB}" )
  # message ( "-- HepMC3 headers found in ${HEPMC3_INCLUDE_DIR}" )
  add_definitions(-DeAST_USE_HepMC3)
endif()

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../PhysicsList/Base/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../PrimGenInterface/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../Components/Base/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../Components/Beampipe/include
                    ${Geant4_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc
                  ${PROJECT_SOURCE_DIR}/../PhysicsList/Base/src/*.cc
                  ${PROJECT_SOURCE_DIR}/../PrimGenInterface/src/*.cc
                  ${PROJECT_SOURCE_DIR}/../Components/Base/src/*.cc
                  ${PROJECT_SOURCE_DIR}/../Components/Beampipe/src/*.cc
                  )
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh
                  ${PROJECT_SOURCE_DIR}/../PhysicsList/Base/include/*.hh
                  ${PROJECT_SOURCE_DIR}/../PrimGenInterface/include/*.hh
                  ${PROJECT_SOURCE_DIR}/../Components/Base/include/*.hh
                  ${PROJECT_SOURCE_DIR}/../Components/Beampipe/include/*.hh
                  )

# In order to keep using glob, we have to manually remove optional sources
if(eAST_USE_HepMC3)
  include_directories ( ${HEPMC3_INCLUDE_DIR} )
  list(APPEND headers "${PROJECT_SOURCE_DIR}/../PrimGenInterface/include/HepMC_3_2_4_ReaderFactory.h")
else()
  list(REMOVE_ITEM sources "${PROJECT_SOURCE_DIR}/../PrimGenInterface/src/eASTPrimaryGeneratorAction.cc")
  list(REMOVE_ITEM headers "${PROJECT_SOURCE_DIR}/../PrimGenInterface/include/eASTPrimaryGeneratorAction.hh")
  list(REMOVE_ITEM headers "${PROJECT_SOURCE_DIR}/../PrimGenInterface/include/HepMC_3_2_4_ReaderFactory.h")
endif()
		
#----------------------------------------------------------------------------
# Add the field maps
#
file(DOWNLOAD
  https://github.com/eic/BeastMagneticField/raw/master/data/EIC_Magnetic_Field_Map_2021_05_07_radial_coords_%5Bcm%5D_%5BT%5D.120000.lines.Bmap
  ${PROJECT_SOURCE_DIR}/data/EIC_Magnetic_Field_Map_2021_05_07_radial_coords__cm___T_.120000.lines.Bmap
)
file(DOWNLOAD
  https://github.com/eic/BeastMagneticField/raw/master/data/EIC_v.0.1.0_Magnetic_Field_Map_2021_05_28_radial_coords_%5Bcm%5D_%5BT%5D.401301.line.Bmap
  ${PROJECT_SOURCE_DIR}/data/EIC_v.0.1.0_Magnetic_Field_Map_2021_05_28_radial_coords__cm___T_.401301.line.Bmap
)

#----------------------------------------------------------------------------
# Add the executable, and link it to external libraries
#
add_executable(eAST eAST.cc ${sources} ${headers})
if(eAST_USE_HepMC3)
  target_link_libraries(eAST ${Geant4_LIBRARIES}  ${HEPMC3_LIB} )
else()
  target_link_libraries(eAST ${Geant4_LIBRARIES})
endif()
  
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build RS. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(RS_SCRIPTS
    gps_point.mac run.mac vis.mac
  )

foreach(_script ${RS_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS eAST
  EXPORT east-export
  DESTINATION bin)
# Below should go into share?
install(FILES DESTINATION bin)

#----------------------------------------------------------------------------
## Generate and install cmake configuration file
install(EXPORT east-export
  FILE
    eastConfig.cmake
  NAMESPACE
    east::
  DESTINATION
  cmake
  )


