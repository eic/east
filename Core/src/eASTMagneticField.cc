// ********************************************************************
//
// eASTMagneticField.cc
//   Header file of eAST Magnetic Field class
//
// History
//   June 22nd, 2021 : first implementation
//
// ********************************************************************

#include "eASTMagneticField.hh"

#include "G4TransportationManager.hh"
#include "G4FieldManager.hh"

#include "G4PropagatorInField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"

#include <fstream>

eASTMagneticField::eASTMagneticField()
: fFieldMessenger(this,"/eAST/field/","Field properties")
{
  fFieldMessenger.DeclareMethod("create",&eASTMagneticField::CreateField,"Create magnetic field")
                 .SetStates(G4State_PreInit)
                 .SetToBeBroadcasted(false);
  fFieldMessenger.DeclareMethod("print",&eASTMagneticField::PrintFieldValue,"Print magnetic field [mm]")
                 .SetStates(G4State_PreInit, G4State_Init, G4State_Idle)
                 .SetToBeBroadcasted(false);
}

void eASTMagneticField::Activate()
{
  // Point field manager to field
  auto transportationmanager = G4TransportationManager::GetTransportationManager();
  fFieldPropagator = transportationmanager->GetPropagatorInField();
  fFieldManager = transportationmanager->GetFieldManager();
  fFieldManager->SetDetectorField(this);

  // Set equation
  fEquation = new G4Mag_UsualEqRhs(this);
  fEquationDoF = 6;
  // Set stepper
  fStepper = new G4ClassicalRK4(fEquation, fEquationDoF);
  // Set chord finder
  fChordFinder = new G4ChordFinder(this,fMinStep,fStepper);
  fChordFinder->GetIntegrationDriver()->SetVerboseLevel(0);
  fChordFinder->SetDeltaChord(fDeltaChord);
  fFieldManager->SetChordFinder(fChordFinder);
  fFieldManager->SetAccuraciesWithDeltaOneStep(fDeltaOneStep);
  fFieldManager->SetDeltaIntersection(fDeltaIntersection);
  fFieldPropagator->SetMinimumEpsilonStep(fEpsMin);
  fFieldPropagator->SetMaximumEpsilonStep(fEpsMax);
}

void eASTMagneticField::CreateField(const G4String& name) {
  fMaps.emplace_back(name);
}

eASTMagneticFieldMap::eASTMagneticFieldMap(const G4String& name)
: fName(name),fFieldMapMessenger(this,"/eAST/field/" + fName + "/","Field map properties")
{
  fFieldMapMessenger.DeclareMethod("load",&eASTMagneticFieldMap::Load,"Load magnetic field")
                    .SetStates(G4State_PreInit)
                    .SetToBeBroadcasted(false);
  fFieldMapMessenger.DeclareMethod("fieldunit",&eASTMagneticFieldMap::SetFieldUnit,"Set field map value unit")
                    .SetStates(G4State_PreInit)
                    .SetToBeBroadcasted(false);
  fFieldMapMessenger.DeclareMethod("gridunit",&eASTMagneticFieldMap::SetGridUnit,"Set field map grid unit")
                    .SetStates(G4State_PreInit)
                    .SetToBeBroadcasted(false);
  fFieldMapMessenger.DeclareMethod("extent",&eASTMagneticFieldMap::SetGridExtent,"Set field map grid extent")
                    .SetStates(G4State_PreInit)
                    .SetToBeBroadcasted(false);
  fFieldMapMessenger.DeclareMethod("type",&eASTMagneticFieldMap::SetInterpolationType,"Set field map interpolation type")
                    .SetStates(G4State_PreInit)
                    .SetToBeBroadcasted(false);
  fFieldMapMessenger.DeclareMethod("print",&eASTMagneticFieldMap::PrintFieldValue,"Print magnetic field")
                    .SetStates(G4State_PreInit, G4State_Init, G4State_Idle)
                    .SetToBeBroadcasted(false);
}

bool eASTMagneticFieldMap::Load(const G4String& filename)
{
  // Do not reload map
  if (! fMap.empty()) return false;

  // Create ifstream
  std::ifstream inputfile(filename);
  if (! inputfile.good()) {
    return false;
  }

  // Determine grid size for i = 0,1
  for (unsigned int i = 0; i < 2; i++) {
    if (std::get<0>(fGridExtent[i]) < std::get<1>(fGridExtent[i])
     && std::get<2>(fGridExtent[i]) > 0) {
      fGridSize[i] = (int) rint (std::get<1>(fGridExtent[i]) - std::get<0>(fGridExtent[i])) / std::get<2>(fGridExtent[i]) + 1;
    } else {
      G4cerr << "ERROR: unable to read grid." << G4endl;
      return false;
    }
  }

  // Determine map type
  if (std::get<0>(fGridExtent[2]) == std::get<1>(fGridExtent[2])) {
    G4cout << "Assuming axially symmetric 2D grid." << G4endl;
    fGridSize[2] = 1;
    fFileFormat = kFileFormatRZ;
  } else {
    G4cout << "Assuming R,Z,Phi grid." << G4endl;
    fGridSize[2] = (int) rint (std::get<1>(fGridExtent[2]) - std::get<0>(fGridExtent[2])) / std::get<2>(fGridExtent[2]) + 1;
    fFileFormat = kFileFormatRZPhi;
  }

  // Resize map
  fMap.resize(3,
    std::vector<std::vector<std::vector<double>>>(fGridSize[0],
      std::vector<std::vector<double>>(fGridSize[1],
        std::vector<double>(fGridSize[2],
          0.0
        )
      )
    )
  );

  // Read file from start
  unsigned int counter = 0;
  while (inputfile.good()) {
    double polar[3] = {0.0, 0.0, 0.0};
    double field[3] = {0.0, 0.0, 0.0};
    switch (fFileFormat) {
      case kFileFormatRZPhi:
        if (! (inputfile >>
               polar[0] >> polar[1] >> polar[2] >>
               field[0] >> field[1] >> field[2])) {
          if (inputfile.eof()) continue;
          G4cerr << "ERROR: unable to read grid, no 6 columns." << G4endl;
          return false;
        }
        break;
      case kFileFormatRZ:
        if (! (inputfile >>
               polar[0] >> polar[1] >>
               field[0] >> field[1])) {
          if (inputfile.eof()) continue;
          G4cerr << "ERROR: unable to read grid, no 4 columns." << G4endl;
          return false;
        }
        break;
      case kFileFormatUndefined:
        G4cout << "ERROR: set field map extent first" << G4endl;
        return false;
        break;
    }

    // Determine grid index
    unsigned int index[3] = {0, 0, 0};
    for (unsigned int i = 0; i < (fFileFormat == kFileFormatRZPhi ? 3: 2); i++) {
      index[i] = (int) rint ((polar[i] - std::get<0>(fGridExtent[i])) / std::get<2>(fGridExtent[i]));
    }
    for (unsigned int i = 0; i < 3; i++) {
      fMap[i][index[0]][index[1]][index[2]] = field[i] * fFieldUnit;
    }
    counter++;
  }

  // Check total entries read
  if (counter != fGridSize[0] * fGridSize[1] * fGridSize[2]) {
    G4cout << "ERROR: expected " << fGridSize[0] * fGridSize[1] * fGridSize[2] << " entries "
           << "but read " << counter << G4endl;
    return false;
  }

  return true;
}

void eASTMagneticFieldMap::AddFieldValue(const G4double point[4], G4double *cartesian) const
{
  // Bounds in z
  if (point[2] < std::get<0>(fGridExtent[1])*fGridUnit[1]
   || point[2] > std::get<1>(fGridExtent[1])*fGridUnit[1]) return;

  // Determine polar coordinate
  double polar[3] = {
    sqrt(point[0]*point[0] + point[1]*point[1]),
    point[2],
    atan2(point[1], point[0])
  };

  // Bounds in r
  if (polar[0] < std::get<0>(fGridExtent[0])*fGridUnit[0]
   || polar[0] > std::get<1>(fGridExtent[0])*fGridUnit[0]) return;

  // Determine cell and local coordinate
  unsigned int index[3] = {0, 0, 0};
  double local[3] = {0.0, 0.0, 0.0};
  for (unsigned int i = 0; i < (fFileFormat == kFileFormatRZPhi ? 3: 2); i++) {
    index[i] = (int) floor ((polar[i]/fGridUnit[i] - std::get<0>(fGridExtent[i])) / std::get<2>(fGridExtent[i]));
    local[i] = (polar[i]/fGridUnit[i] - std::get<0>(fGridExtent[i]) - index[i] * std::get<2>(fGridExtent[i])) / std::get<2>(fGridExtent[i]);
  }

  // Fall back to linear interpolation at boundary layer
  EInterpolationType interpolation = fInterpolationType;
  if (interpolation == kCubic) {
    if ((index[0] == 0)
     || (index[1] == 0)
     || (index[2] == 0 && fFileFormat == kFileFormatRZPhi)
     || (index[0] == fGridSize[0] - 2)
     || (index[1] == fGridSize[1] - 2)
     || (index[2] == fGridSize[2] - 2 && fFileFormat == kFileFormatRZPhi)) {
      interpolation = kLinear;
    }
  }

  // Get cell corner values
  size_t n = 8;
  const int (*map)[3] = kLinearMap;
  switch (interpolation) {
    case kLinear:
      map = kLinearMap;
      n = 8;
      break;
    case kCubic:
      map = kCubicMap;
      n = 64;
      break;
  }
  thread_local G4double values[3][64];
  for (unsigned int i = 0; i < n; i++) {
    for (unsigned int j = 0; j < 3; j++) {
      values[j][i] =
        (fFileFormat == kFileFormatRZPhi ?
          fMap[j][index[0] + map[i][0]][index[1] + map[i][1]][index[2] + map[i][2]] :
          fMap[j][index[0] + map[i][0]][index[1] + map[i][1]][0]
        );
    }
  }

  // Interpolate in cylindrical components
  double cylindrical[3] = {0.0, 0.0, 0.0};
  for (unsigned int j = 0; j < 3; j++) {
    cylindrical[j] = _trilinearInterpolate(values[j], local);
  }

  // Rotate into cartesian coordinates:
  //  cartesian[Bx,By,Bz], point[x,y,z]
  //  cylindrical[Br,Bz,Bphi], polar[r,z,phi]
  if (polar[0] > 0) {
    cartesian[0] = (cylindrical[0] * point[0] - cylindrical[2] * point[1]) / polar[0];
    cartesian[1] = (cylindrical[2] * point[0] + cylindrical[0] * point[1]) / polar[0];
  } else {
    cartesian[0] = cylindrical[0] * cos(polar[2]) - cylindrical[2] * sin(polar[2]);
    cartesian[1] = cylindrical[2] * cos(polar[2]) + cylindrical[0] * sin(polar[2]);
  }
  cartesian[2] = cylindrical[1];
}

void eASTMagneticFieldMap::PrintFieldValue(const G4ThreeVector& point)
{
  G4double B[3] = {0,0,0};
  G4double p[4] = {point.x(), point.y(), point.z(), 0.0};
  AddFieldValue(p, B);
  G4cout << "B" << point/CLHEP::m << " [m] = ";
  for (unsigned int i = 0; i < 3; i++)
    G4cout << B[i]/CLHEP::tesla << " ";
  G4cout << "[T]" << G4endl;
}


// Set location of cell vertices
const int eASTMagneticFieldMap::kLinearMap[8][3] = {
    {0, 0, 0},  // 00
    {0, 0, 1},
    {0, 1, 0},
    {0, 1, 1},
    {1, 0, 0},  // 04
    {1, 0, 1},
    {1, 1, 0},
    {1, 1, 1},
};
const int eASTMagneticFieldMap::kCubicMap[64][3] = {
    {-1, -1, -1},  // 00
    {-1, -1, 0},
    {-1, -1, 1},
    {-1, -1, 2},
    {-1, 0, -1},  // 04
    {-1, 0, 0},
    {-1, 0, 1},
    {-1, 0, 2},
    {-1, 1, -1},  // 08
    {-1, 1, 0},
    {-1, 1, 1},
    {-1, 1, 2},
    {-1, 2, -1},  // 12
    {-1, 2, 0},
    {-1, 2, 1},
    {-1, 2, 2},
    {0, -1, -1},  // 16
    {0, -1, 0},
    {0, -1, 1},
    {0, -1, 2},
    {0, 0, -1},  // 20
    {0, 0, 0},
    {0, 0, 1},
    {0, 0, 2},
    {0, 1, -1},  // 24
    {0, 1, 0},
    {0, 1, 1},
    {0, 1, 2},
    {0, 2, -1},  // 28
    {0, 2, 0},
    {0, 2, 1},
    {0, 2, 2},
    {1, -1, -1},  // 22
    {1, -1, 0},
    {1, -1, 1},
    {1, -1, 2},
    {1, 0, -1},  // 26
    {1, 0, 0},
    {1, 0, 1},
    {1, 0, 2},
    {1, 1, -1},  // 40
    {1, 1, 0},
    {1, 1, 1},
    {1, 1, 2},
    {1, 2, -1},  // 44
    {1, 2, 0},
    {1, 2, 1},
    {1, 2, 2},
    {2, -1, -1},  // 48
    {2, -1, 0},
    {2, -1, 1},
    {2, -1, 2},
    {2, 0, -1},  // 52
    {2, 0, 0},
    {2, 0, 1},
    {2, 0, 2},
    {2, 1, -1},  // 56
    {2, 1, 0},
    {2, 1, 1},
    {2, 1, 2},
    {2, 2, -1},  // 60
    {2, 2, 0},
    {2, 2, 1},
    {2, 2, 2},
};
