// ********************************************************************
//
// eASTMagneticField.hh
//   Header file of eAST Magnetic Field class
//
// History
//   June 22nd, 2021 : first implementation
//
// ********************************************************************

#ifndef eASTMagneticField_h
#define eASTMagneticField_h 1

#include "G4MagneticField.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <vector>

class G4EquationOfMotion;
class G4MagIntegratorStepper;
class G4ChordFinder;
class G4FieldManager;
class G4PropagatorInField;

class eASTMagneticFieldMap {

  public:
    eASTMagneticFieldMap(const G4String& name);
    ~eASTMagneticFieldMap() = default;

  private:

    // Static maps to indices of neighboring points
    static const int kLinearMap[8][3];
    static const int kCubicMap[64][3];

  private:

    // Field name
    G4String fName{""};

  public:

    // Load field map from filename
    bool Load(const G4String& filename);

    // Get field value at point
    void GetFieldValue(const G4double point[4], G4double *field) const {
      field[0] = 0;
      field[1] = 0;
      field[2] = 0;
      AddFieldValue(point, field);
    };

    // Add field value at point
    void AddFieldValue(const G4double point[4], G4double *field) const;

    // Print field value at point
    void PrintFieldValue(const G4ThreeVector& point);

  private:

    // Field messenger
    G4GenericMessenger fFieldMapMessenger;

  private:

    // File format
    enum EFileFormat {kFileFormatUndefined, kFileFormat3D, kFileFormat2D};
    EFileFormat fFileFormat{kFileFormatUndefined};

  private:

    // Map units and extent
    double fFieldUnit{CLHEP::tesla};
    std::array<double,3> fGridUnit{CLHEP::cm, CLHEP::cm, CLHEP::deg};
    std::array<std::tuple<double,double,double>,3> fGridExtent; // min, max, spacing
    std::array<unsigned int,3> fGridSize{0,0,0};

  public:

    // Map units and extent getters and setters
    void SetFieldUnit(const G4String& unit) {
      fFieldUnit = G4UnitDefinition::GetValueOf(unit);
    }
    void SetGridUnit(unsigned int i, const G4String& unit) {
      if (i < fGridExtent.size()) {
        fGridUnit[i] = G4UnitDefinition::GetValueOf(unit);
      } else {
        G4cerr << "ERROR: cannot set magnetic field grid unit" << G4endl;
      }
    }
    void SetGridExtent(unsigned int i, const G4ThreeVector& extent) {
      if (i < fGridExtent.size() && extent.x() < extent.y() && extent.z() > 0.0) {
        fGridExtent[i] = std::make_tuple(extent.x(), extent.y(), extent.z());
      } else {
        G4cerr << "ERROR: cannot set magnetic field grid extent" << G4endl;
      }
    }

  private:

    // Map data
    std::vector<std::vector<std::vector<std::vector<double>>>> fMap;

  private:

    // Interpolation type to allow for linear and cubic interpolation
    enum EInterpolationType {kLinear, kCubic};
    EInterpolationType fInterpolationType{kLinear};

  public:

    // Interpolation type setters and getters
    void SetInterpolationType(const G4String& type) {
      if (type == "linear") fInterpolationType = kLinear;
      if (type == "cubic") fInterpolationType = kCubic;
    }
    EInterpolationType GetInterpolationType() const {
      return fInterpolationType;
    }

  private:

    // Linear interpolation functions
    double _linearInterpolate(const double p[2 << 0], double x) const {
      return p[0] + x * (p[1] - p[0]);
    }
    double _bilinearInterpolate(const double p[2 << 1], const double x[2]) const {
      double c[2];
      c[0] = _linearInterpolate(&(p[0]), x[1]);
      c[1] = _linearInterpolate(&(p[2]), x[1]);
      return _linearInterpolate(c, x[0]);
    }
    double _trilinearInterpolate(const double p[2 << 2], const double x[3]) const {
      double c[2];
      c[0] = _bilinearInterpolate(&(p[0]), &(x[1]));
      c[1] = _bilinearInterpolate(&(p[4]), &(x[1]));
      return _linearInterpolate(c, x[0]);
    }

    // Cubic interpolation functions
    double _cubicInterpolate(const double p[4 << 0], double x) const {
      return p[1] + 0.5 * x * (p[2] - p[0] +
                               x * (2. * p[0] - 5. * p[1] + 4. * p[2] - p[3] +
                                    x * (3. * (p[1] - p[2]) + p[3] - p[0])));
    }
    double _bicubicInterpolate(const double p[4 << 1], const double x[2]) const {
      double c[4];
      c[0] = _cubicInterpolate(&(p[0]),  x[1]);
      c[1] = _cubicInterpolate(&(p[4]),  x[1]);
      c[2] = _cubicInterpolate(&(p[8]),  x[1]);
      c[3] = _cubicInterpolate(&(p[12]), x[1]);
      return _cubicInterpolate(c, x[0]);
    }
    double _tricubicInterpolate(const double p[4 << 2], const double x[3]) const {
      double c[4];
      c[0] = _bicubicInterpolate(&(p[0]),  &(x[1]));
      c[1] = _bicubicInterpolate(&(p[16]), &(x[1]));
      c[2] = _bicubicInterpolate(&(p[32]), &(x[1]));
      c[3] = _bicubicInterpolate(&(p[48]), &(x[1]));
      return _cubicInterpolate(c, x[0]);
    }

};

class eASTMagneticField : public G4MagneticField {

  public:

    eASTMagneticField();
    ~eASTMagneticField() = default;

    // Activate in ConstuctSDandField
    void Activate();

    // Create new field
    void CreateField(const G4String& name);

    // Get field value at point
    void GetFieldValue(const G4double point[4], G4double *field) const {
      field[0] = 0;
      field[1] = 0;
      field[2] = 0;
      for (auto &map: fMaps) {
        map.AddFieldValue(point, field);
      }
    };

    // Print field value at point
    void PrintFieldValue(const G4ThreeVector& point) {
      for (auto &map: fMaps) {
        map.PrintFieldValue(point);
      }
    };

  private:

    // Field messenger
    G4GenericMessenger fFieldMessenger;

  private:

    // Field maps
    std::vector<eASTMagneticFieldMap> fMaps;

  private:

    G4double fMinStep{0.01*mm};
    G4double fDeltaChord{3.0*mm};
    G4double fDeltaOneStep{0.01*mm};
    G4double fDeltaIntersection{0.1*mm};
    G4double fEpsMin{1.0e-5*mm};
    G4double fEpsMax{1.0e-4*mm};

    G4EquationOfMotion*     fEquation{nullptr};
    G4int                   fEquationDoF{0};
    G4FieldManager*         fFieldManager{nullptr};
    G4PropagatorInField*    fFieldPropagator{nullptr};
    G4MagIntegratorStepper* fStepper{nullptr};
    G4ChordFinder*          fChordFinder{nullptr};

};

#endif
