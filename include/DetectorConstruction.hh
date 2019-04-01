/*
 * DetectorConstruction.hh
 *
 *  Created on:
 *      Author: fna
 */

#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

    void SetParam(const G4String& path, const G4String &value);
               
private:

    DetectorMessenger *messenger_;

    G4bool  fCheckOverlaps_;

    G4VPhysicalVolume* physWorld_;

    G4double caloLayerThick_;
    G4double caloRMin_;
    G4double caloRMax_;
    G4int caloLayersNumberZ_;
    G4int caloLayersNumberRadial_;
    G4int caloLayersNumberPhi_;
    G4double caloPhi_;
    G4double caloGap_;
    G4double caloParticleToCalorimeterDistance_;

    bool cubeUse_;
    G4double cubeISOToDetectorDistance_;
    G4double cubeGap_;
    G4int cubeSamplesNumber_;

    void DefineMaterials();

    /// Constructs the "horseshoe" calorimeter.
    /// @param rot rotation matrix to apply to the entire assembly
    /// @param pos translation to apply to the entire assembly
    /// @param layerThick thickness of individual layer
    /// @param rMin intern radius of the tube-like calorimeter
    /// @param rMax outern radius of the tube-like calorimeter
    /// @param layersNumberZ number of layers in longitudial direction
    /// @param layersNumberRadial number of layers for radial segmentation
    /// @param layersNumberPhi number of layers in azimuthal segmentation
    /// @param phi angular length of the covered sector
    /// @param suffix a naming postfix appended to volume names
    void ConstructTubeMatrixDetector( const G4String& suffix
            , G4RotationMatrix * rot
            , const G4ThreeVector& pos
            , G4double layerThick
            , G4double rMin
            , G4double rMax
            , G4int layersNumberZ
            , G4int layersNumberRadial
            , G4int layersNumberPhi
            , G4double phi
            );

    /// Aux method constructing a cube for probing the missed energy
    void ConstructCube(G4double distanceToCenter, G4double gap, G4int samplesNumber);

    /// Constructs the rectangular calorimeter.
    /// @param rot rotation matrix to apply to the entire assembly
    /// @param pos translation to apply to the entire assembly
    /// @param columns number of horizontal segments
    /// @param rows number of vertical segments
    void ConstructRectangleMatrixDetector(G4RotationMatrix* rot,
            const G4ThreeVector& pos,
            G4int columns,
            G4int rows,
            const G4ThreeVector& pixelSize,
            G4double gap,
            G4Material* material,
            const G4String& suffix
            );
};



