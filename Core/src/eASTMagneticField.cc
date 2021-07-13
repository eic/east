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

#include <fstream>

// Set location of cell vertices
const int eASTMagneticField::kLinearMap[8][3] = {
    {0, 0, 0},  // 00
    {0, 0, 1},
    {0, 1, 0},
    {0, 1, 1},
    {1, 0, 0},  // 04
    {1, 0, 1},
    {1, 1, 0},
    {1, 1, 1},
};
const int eASTMagneticField::kCubicMap[64][3] = {
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

eASTMagneticField::eASTMagneticField()
: fFieldMessenger(this,"/eAST/field/","Field properties")
{
  // Define messenger commands
  fFieldMessenger.DeclareMethod("load",&eASTMagneticField::LoadFieldMap,"Load magnetic field");
  fFieldMessenger.DeclareMethod("fieldunit",&eASTMagneticField::SetFieldUnit,"Set field map value unit");
  fFieldMessenger.DeclareMethod("gridunit",&eASTMagneticField::SetGridUnit,"Set field map grid unit");
  fFieldMessenger.DeclareMethod("extent",&eASTMagneticField::SetGridExtent,"Set field map grid extent");
  fFieldMessenger.DeclareMethod("type",&eASTMagneticField::SetInterpolationType,"Set field map interpolation type");
  fFieldMessenger.DeclareMethod("print",&eASTMagneticField::PrintFieldValue,"Print the field value at point");

  // Point field manager to field
  auto transportationmanager = G4TransportationManager::GetTransportationManager();
  auto fieldmanager = transportationmanager->GetFieldManager();
  fieldmanager->SetDetectorField(this);
}

bool eASTMagneticField::LoadFieldMap(const G4String& filename)
{
  // Create ifstream
  std::ifstream inputfile(filename);
  if (! inputfile.good()) {
    return false;
  }

  // Determine grid size for i = 0,1
  unsigned int count[3] = {0, 0, 0};
  for (unsigned int i = 0; i < 2; i++) {
    if (std::get<0>(fGridExtent[i]) < std::get<1>(fGridExtent[i])
     && std::get<2>(fGridExtent[i]) > 0) {
      count[i] = (int) rint (std::get<1>(fGridExtent[i]) - std::get<0>(fGridExtent[i])) / std::get<2>(fGridExtent[i]) + 1;
    } else {
      G4cerr << "ERROR: unable to read grid." << G4endl;
      return false;
    }
  }

  // Determine map type
  if (std::get<0>(fGridExtent[2]) == std::get<1>(fGridExtent[2])) {
    G4cout << "Assuming axially symmetric 2D grid." << G4endl;
    count[2] = 1;
    fFileFormat = kFileFormat2D;
  } else {
    G4cout << "Assuming 3D grid." << G4endl;
    count[2] = (int) rint (std::get<1>(fGridExtent[2]) - std::get<0>(fGridExtent[2])) / std::get<2>(fGridExtent[2]) + 1;
    fFileFormat = kFileFormat3D;
  }

  // Resize map
  fMap.resize(3);
  for (unsigned int i = 0; i < 3; i++)
    fMap[i].resize(count[0],
      std::vector<std::vector<double>>(count[1],
        std::vector<double>(count[2], 0.0)));

  // Read file from start
  while (inputfile.good()) {
    double polar[3] = {0.0, 0.0, 0.0};
    double field[3] = {0.0, 0.0, 0.0};
    switch (fFileFormat) {
      case kFileFormat3D:
        if (! (inputfile >>
               polar[0] >> polar[1] >> polar[2] >>
               field[0] >> field[1] >> field[2])) {
          if (inputfile.eof()) continue;
          G4cerr << "ERROR: unable to read grid, no 6 columns." << G4endl;
          return false;
        }
        break;
      case kFileFormat2D:
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

    // Check input
    if (inputfile.eof()) G4cout << "|" << G4endl;

    // Determine grid index
    unsigned int index[3] = {0, 0, 0};
    for (unsigned int i = 0; i < (fFileFormat == kFileFormat3D ? 3: 2); i++) {
      index[i] = (int) rint ((polar[i] - std::get<0>(fGridExtent[i])) / std::get<1>(fGridExtent[i]));
    }
    for (unsigned int i = 0; i < 3; i++) {
      fMap[i][index[0]][index[1]][index[2]] = field[i] * fFieldUnit;
    }
  }
  return true;
}

void eASTMagneticField::GetFieldValue(const G4double point[4], G4double *cartesian ) const
{
  // Determine polar coordinate
  double polar[3] = {
    sqrt(point[0]*point[0] + point[1]*point[1]),
    point[2],
    atan2(point[1], point[0])
  };
  G4cout << polar[0] << " " << polar[1] << " " << polar[2] << G4endl;

  // Determine cell and local coordinate
  unsigned int index[3] = {0, 0, 0};
  double local[3] = {0.0, 0.0, 0.0};
  for (unsigned int i = 0; i < (fFileFormat == kFileFormat3D ? 3: 2); i++) {
    index[i] = (int) floor ((polar[i]/fGridUnit[i] - std::get<0>(fGridExtent[i])) / std::get<1>(fGridExtent[i]));
    local[i] = (polar[i]/fGridUnit[i] - std::get<0>(fGridExtent[i]) - index[i] * std::get<1>(fGridExtent[i])) / std::get<1>(fGridExtent[i]);
  }
  G4cout << index[0] << " " << index[1] << " " << index[2] << G4endl;
  G4cout << local[0] << " " << local[1] << " " << local[2] << G4endl;

  // Get cell corner values
  size_t n = 8;
  const int (*map)[3] = kLinearMap;
  switch (fInterpolationType) {
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
        (fFileFormat == kFileFormat3D ?
          fMap[j][index[0] + map[i][0]][index[1] + map[i][1]][index[2] + map[i][2]] :
          fMap[j][index[0] + map[i][0]][index[1] + map[i][1]][0]
        );
      G4cout << values[j][i] << " ";
    }
    G4cout << " at (" << map[i][0] << "," << map[i][1] << "," << map[i][2] << ")" << G4endl;
  }

  // Interpolate in cylindrical components
  double cylindrical[3] = {0.0, 0.0, 0.0};
  for (unsigned int j = 0; j < 3; j++) {
    cylindrical[j] = _trilinearInterpolate(values[j], local);
  }
  G4cout << cylindrical[0] << " " << cylindrical[1] << " " << cylindrical[2] << G4endl;
  G4cout << polar[0] << " " << polar[1] << " " << polar[2] << G4endl;

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

void eASTMagneticField::PrintFieldValue(const G4ThreeVector& point)
{
  G4double B[3];
  G4double p[4] = {point.x(), point.y(), point.z(), 0.0};
  G4cout << "B" << point/CLHEP::m << " [m] = ";
  GetFieldValue(p, B);
  for (unsigned int i = 0; i < 3; i++) G4cout << B[i]/CLHEP::tesla << " ";
  G4cout << "[T]" << G4endl;
}
