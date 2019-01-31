/*
 * DetectorConstruction.cc
 *
 *  Created on:
 *      Author: fna
 */

#include "DetectorConstruction.hh"
#include "CaloSensitiveDetector.hh"
#include "CubeSensitiveDetector.hh"
#include "DetectorMessenger.hh"


#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4ReplicatedSlice.hh"

#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"



using namespace CLHEP;

#define Mat(x) (G4NistManager::Instance()->FindOrBuildMaterial(x))
#define Elem(x) (G4NistManager::Instance()->FindOrBuildElement(x))


DetectorConstruction::DetectorConstruction(): G4VUserDetectorConstruction(),
	fCheckOverlaps_(true),

	// default configuration parameters
	//  calorimeter
	caloLayerThick_(4),
	caloRMin_(20),
	caloRMax_(350),
	caloLayersNumberZ_(10),
	caloLayersNumberRadial_(5),
	caloLayersNumberPhi_(6),
	caloPhi_(180),
	caloGap_(3),
	caloParticleToCalorimeterDistance_(350),
	//  cube
	cubeUse_(false),
	cubeISOToDetectorDistance_(500),
	cubeGap_(10),
	cubeSamplesNumber_(10)
{
	DefineMaterials();

	messenger_ = new DetectorMessenger(this);
}


DetectorConstruction::~DetectorConstruction()
{
	delete messenger_;
}


void DetectorConstruction::DefineMaterials()
{

}


G4VPhysicalVolume* DetectorConstruction::Construct()
{  
	G4NistManager* nist = G4NistManager::Instance();

	G4double halfWorldLength = 1100*cm / 2;

	G4Box * solidWorld= new G4Box("world",halfWorldLength,halfWorldLength,halfWorldLength);
	G4LogicalVolume *logicWorld= new G4LogicalVolume( solidWorld, nist->FindOrBuildMaterial("G4_AIR"), "World", 0, 0, 0);

	physWorld_ = new G4PVPlacement(0,               // no rotation
			G4ThreeVector(), // at (0,0,0)
			logicWorld,      // its logical volume
			"World",         // its name
			0,               // its mother  volume
			false,           // no boolean operations
			0,
			fCheckOverlaps_);              // copy number


	// Construct calorimeter
	G4RotationMatrix * rot;
	rot = new  G4RotationMatrix();
	rot->rotateZ(-(180-caloPhi_)/2*deg);
	ConstructTubeMatrixDetector(rot,
			G4ThreeVector(0.*cm, caloGap_*cm, -caloParticleToCalorimeterDistance_*cm),
			caloLayerThick_*cm,
			caloRMin_*cm,
			caloRMax_*cm,
			caloLayersNumberZ_,
			caloLayersNumberRadial_,
			caloLayersNumberPhi_,
			caloPhi_*deg,
			"_1"
	);

	rot = new G4RotationMatrix();
	rot->rotateZ(-((180-caloPhi_)/2+180)*deg);
	ConstructTubeMatrixDetector(rot,
			G4ThreeVector(0.*cm, -caloGap_*cm, -caloParticleToCalorimeterDistance_*cm),
			caloLayerThick_*cm,
			caloRMin_*cm,
			caloRMax_*cm,
			caloLayersNumberZ_,
			caloLayersNumberRadial_,
			caloLayersNumberPhi_,
			caloPhi_*deg,
			"_2"
	);

	// Construct cube to monitor skipped particles if necessary
	if (cubeUse_)
		ConstructCube(cubeISOToDetectorDistance_*cm, cubeGap_*cm, cubeSamplesNumber_);

	logicWorld->SetVisAttributes (G4Color(1, 1, 1, 0));

	return physWorld_;

}



void DetectorConstruction::ConstructTubeMatrixDetector(G4RotationMatrix * rot,
		const G4ThreeVector& pos,
		G4double layerThick,
		G4double rMin,
		G4double rMax,
		G4int layersNumberZ,
		G4int layersNumberRadial,
		G4int layersNumberPhi,
		G4double phi,
		const G4String& suffix)
{
	layerThick = layerThick / 2;
	rMin = rMin / 2;
	rMax = rMax / 2;
	G4double dividedPhi	= phi / layersNumberPhi;
	G4double layerHeight = (rMax - rMin) / layersNumberRadial;

	G4VSolid * tube = new G4Tubs("tube" + suffix, rMin, rMax, layerThick * layersNumberZ, 0., phi);
	G4LogicalVolume * logTube = new G4LogicalVolume(tube, Mat("G4_Galactic"), "tubeL" + suffix, 0, 0, 0);
	new G4PVPlacement(rot, pos, "tubeP" + suffix, logTube, physWorld_, false, 0);

	G4VSolid * tubeZ = new G4Tubs("tubeZ" + suffix, rMin, rMax, layerThick, 0., phi);
	G4LogicalVolume * logTubeZ = new G4LogicalVolume(tubeZ, Mat("G4_Galactic"), "TubeZ" + suffix, 0, 0, 0);
	new	G4PVReplica("TubeZ" + suffix, logTubeZ, logTube, kZAxis, layersNumberZ, layerThick*2);

	G4VSolid * tubeRadial = new G4Tubs("tubeRadial" + suffix, 0, layerHeight, layerThick, 0., phi);
	G4LogicalVolume * logTubeRadial = new G4LogicalVolume(tubeRadial, Mat("G4_Galactic"), "TubeRadial" + suffix, 0, 0, 0);
	new	G4PVReplica("TubeRadial" + suffix, logTubeRadial, logTubeZ, kRho, layersNumberRadial, layerHeight, rMin);

	G4VSolid * tubePhi = new G4Tubs("tubePhi" + suffix, rMin, rMax, layerThick, -dividedPhi/2., dividedPhi);
	G4LogicalVolume * logTubePhi = new G4LogicalVolume(tubePhi, Mat("G4_Galactic"), "TubePhi" + suffix, 0, 0, 0);
	new	G4PVReplica("TubePhi" + suffix, logTubePhi, logTubeRadial, kPhi, layersNumberPhi, dividedPhi);

	G4VSolid * att = new G4Tubs("att"+ suffix, rMin, rMax, layerThick/2, -dividedPhi/2., dividedPhi);
	G4LogicalVolume * logAtt = new G4LogicalVolume(att, Mat("G4_Pb"), "Att" + suffix, 0, 0, 0);
	new G4PVPlacement(0, G4ThreeVector(0, 0, layerThick/2), logAtt, "Att" + suffix, logTubePhi, false, 0);

	G4VSolid * scin = new G4Tubs("scin" + suffix, rMin, rMax, layerThick/2, -dividedPhi/2., dividedPhi);
	G4LogicalVolume * logScin = new G4LogicalVolume(scin, Mat("G4_Si"), "Scin" + suffix, 0, 0, 0);
	new G4PVPlacement(0, G4ThreeVector(0, 0, -layerThick/2), logScin, "Scin" + suffix, logTubePhi, false, 0);

	logTube->SetVisAttributes (G4VisAttributes::GetInvisible());
	logTubeZ->SetVisAttributes (G4VisAttributes::GetInvisible());
	logTubeRadial->SetVisAttributes (G4VisAttributes::GetInvisible());
	logTubePhi->SetVisAttributes (G4VisAttributes::GetInvisible());
	logAtt->SetVisAttributes (G4Colour(0.5, 0.5, 0.5, 1));
	logScin->SetVisAttributes (G4Colour(0.0, 0.0, 1.0, 1));
}

void DetectorConstruction::ConstructCube(G4double distanceToCenter, G4double gap, G4int samplesNumber)
{
	G4double thickness = 1*um;

	G4double step = (distanceToCenter * 2 - thickness - 1*um) / samplesNumber;
	if (gap >= step)
	{
		char msg[100];
		sprintf(msg, "gap must be less than %.3f mm", step);
		G4Exception("DetectorConstruction::ConstructCube", "", RunMustBeAborted, msg);
	}

	G4double pixelSize = step - gap;
	if (pixelSize <= 0)
		G4Exception("DetectorConstruction::ConstructCube", "", RunMustBeAborted, "gap is too large");


	char szSuffix[20];
	G4RotationMatrix * rot = 0;
	G4ThreeVector pos;
	for (int i = 0; i < 6; i++)
	{
		switch (i)
		{
			case 0: rot = 0; pos = G4ThreeVector(0, 0, distanceToCenter); break;
			case 1: rot = 0; pos = G4ThreeVector(0, 0, -distanceToCenter);   break;
			case 2: rot = new G4RotationMatrix(); rot->rotateY(90*deg); pos = G4ThreeVector(-distanceToCenter, 0, 0);   break;
			case 3: rot = new G4RotationMatrix(); rot->rotateY(90*deg); pos = G4ThreeVector(distanceToCenter, 0, 0);   break;
			case 4: rot = new G4RotationMatrix(); rot->rotateX(90*deg); pos = G4ThreeVector(0, distanceToCenter, 0);   break;
			case 5: rot = new G4RotationMatrix(); rot->rotateX(90*deg); pos = G4ThreeVector(0, -distanceToCenter, 0);   break;
		}
		sprintf(szSuffix, "_%d", i + 1);

		ConstructRectangleMatrixDetector(rot,
				pos,
				samplesNumber,
				samplesNumber,
				G4ThreeVector(pixelSize, pixelSize, thickness),
				gap,
				Mat("G4_Galactic"),
				szSuffix
				);
	}
}


void DetectorConstruction::ConstructRectangleMatrixDetector(G4RotationMatrix* rot,
		const G4ThreeVector& pos,
		G4int columns,
		G4int rows,
		const G4ThreeVector& pixelSize,
		G4double gap,
		G4Material* material,
		const G4String& suffix)
{
	G4double sizeX = (pixelSize[0] + gap) * columns;
	G4double sizeY = (pixelSize[1] + gap) * rows;

	G4VSolid * mother = new G4Box("mother" + suffix ,sizeX/2, sizeY/2, pixelSize[2]/2);
	G4LogicalVolume * logMother = new G4LogicalVolume(mother, Mat("G4_Galactic"), "Mother" + suffix);
	new G4PVPlacement(rot, pos, "Mother" + suffix, logMother, physWorld_, false, 0);

	G4VSolid * repY = new G4Box("repY" + suffix, sizeX/2, pixelSize[1]/2, pixelSize[2]/2);
	G4LogicalVolume * logRepY = new G4LogicalVolume(repY, Mat("G4_Galactic"), "RepY" + suffix, 0, 0, 0);
	new G4ReplicatedSlice("RepY" + suffix, logRepY, logMother, kYAxis, pixelSize[1]+gap, gap/2, 0);

	G4VSolid * repX = new G4Box("cubePixel"+suffix,pixelSize[0]/2,pixelSize[1]/2,pixelSize[2]/2);
	G4LogicalVolume * logRepX = new G4LogicalVolume(repX, material, "CubePixel" + suffix, 0, 0, 0);
	new G4ReplicatedSlice("CubePixel" + suffix, logRepX, logRepY, kXAxis, pixelSize[0]+gap, gap/2, 0);

	logMother->SetVisAttributes (G4VisAttributes::Invisible);
	logRepY->SetVisAttributes (G4VisAttributes::Invisible);
	logRepX->SetVisAttributes (G4Colour(82/255.,130/255.,96/255., 0.1));
}

void DetectorConstruction::ConstructSDandField()
{
	//return;

	G4SDManager* sdMan = G4SDManager::GetSDMpointer();

	G4VSensitiveDetector* sens = new CaloSensitiveDetector("calo");
	sdMan->AddNewDetector(sens);
	char name[30];
	for (int i = 0; i < 2; i++)
	{
		sprintf(name, "Att_%d", i + 1);
		SetSensitiveDetector(name, sens);

		sprintf(name, "Scin_%d", i + 1);
		SetSensitiveDetector(name, sens);
	}

	if (cubeUse_)
	{
		sens = new CubeSensitiveDetector("cube");
		sdMan->AddNewDetector(sens);

		for (int i = 0; i < 6; i++)
		{
			sprintf(name, "CubePixel_%d", i + 1);
			SetSensitiveDetector(name, sens);
		}
	}
}

void DetectorConstruction::SetParam(const G4String& path, const G4String &value)
{
	if (path == "/conf/calo/setLayerThick") caloLayerThick_ = atof(value.c_str());
	else if (path == "/conf/calo/setRMin") caloRMin_ = atof(value.c_str());
	else if (path == "/conf/calo/setRMax") caloRMax_ = atof(value.c_str());
	else if (path == "/conf/calo/setLayersNumberZ") caloLayersNumberZ_ = atoi(value.c_str());
	else if (path == "/conf/calo/setLayersNumberRadial") caloLayersNumberRadial_ = atoi(value.c_str());
	else if (path == "/conf/calo/setLayersNumberPhi") caloLayersNumberPhi_ = atoi(value.c_str());
	else if (path == "/conf/calo/setPhi")
	{
		caloPhi_ = atof(value.c_str());
		if (caloPhi_ < 0) caloPhi_ = 0;
		else if (caloPhi_ > 180) caloPhi_ = 180;
	}
	else if (path == "/conf/calo/setGap") caloGap_ = atof(value.c_str());
	else if (path == "/conf/calo/setParticleToColarimeterDistance") caloParticleToCalorimeterDistance_ = atof(value.c_str());

	else if (path == "/conf/cube/useCube") cubeUse_ = atoi(value.c_str());
	else if (path == "/conf/cube/setISOToDetectorDistance") cubeISOToDetectorDistance_ = atof(value.c_str());
	else if (path == "/conf/cube/setGap") cubeGap_ = atof(value.c_str());
	else if (path == "/conf/cube/setSamplesNumber") cubeSamplesNumber_ = atoi(value.c_str());
}



