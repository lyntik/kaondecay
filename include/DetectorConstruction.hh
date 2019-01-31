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

    void ConstructTubeMatrixDetector(G4RotationMatrix * rot,
    		const G4ThreeVector& pos,
			G4double layerThick,
			G4double rMin,
			G4double rMax,
			G4int layersNumberZ,
			G4int layersNumberRadial,
			G4int layersNumberPhi,
			G4double phi,
			const G4String& suffix
			);

    void ConstructCube(G4double distanceToCenter, G4double gap, G4int samplesNumber);

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



