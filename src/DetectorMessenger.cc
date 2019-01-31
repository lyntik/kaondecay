/*
 * DetectorMessenger.cc
 *
 *  Created on:
 *      Author: fna
 */


#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"


DetectorMessenger::DetectorMessenger(DetectorConstruction* detectorConstruction) : G4UImessenger(),
	detectorConstruction_(detectorConstruction),
	directory_(0)
{ 
	directory_ = new G4UIdirectory("/conf/");
	directory_->SetGuidance("Configuration parameters");

	caloDirectory_ = new G4UIdirectory("/conf/calo/");
	cubeDirectory_ = new G4UIdirectory("/conf/cube/");

	setCaloLayerThickCmd_ = new G4UIcmdWithADouble("/conf/calo/setLayerThick",this);
	setCaloLayerThickCmd_->SetParameterName("layerthick", false);
	setCaloLayerThickCmd_->SetGuidance("Layer thickness in cm");
	setCaloRMinCmd_ = new G4UIcmdWithADouble("/conf/calo/setRMin",this);
	setCaloRMinCmd_->SetParameterName("rmin", false);
	setCaloRMinCmd_->SetGuidance("Minimal radius in cm");
	setCaloRMaxCmd_ = new G4UIcmdWithADouble("/conf/calo/setRMax",this);
	setCaloRMaxCmd_->SetParameterName("rmax", false);
	setCaloRMaxCmd_->SetGuidance("Maximal radius in cm");
	setCaloLayersNumberZCmd_ = new G4UIcmdWithAnInteger("/conf/calo/setLayersNumberZ",this);
	setCaloLayersNumberZCmd_->SetParameterName("layersnumberz", false);
	setCaloLayersNumberRadialCmd_ = new G4UIcmdWithAnInteger("/conf/calo/setLayersNumberRadial",this);
	setCaloLayersNumberRadialCmd_->SetParameterName("layersnumberradial", false);
	setCaloLayersNumberPhiCmd_ = new G4UIcmdWithAnInteger("/conf/calo/setLayersNumberPhi",this);
	setCaloLayersNumberPhiCmd_->SetParameterName("layersnumberphi", false);
	setCaloPhiCmd_ = new G4UIcmdWithADouble("/conf/calo/setPhi",this);
	setCaloPhiCmd_->SetParameterName("phi", false);
	setCaloPhiCmd_->SetGuidance("Phi angle in degrees");
	setCaloGapCmd_ = new G4UIcmdWithADouble("/conf/calo/setGap",this);
	setCaloGapCmd_->SetParameterName("gap", false);
	setCaloGapCmd_->SetGuidance("Gap in cm");
	setCaloParticleToColarimeterDistanceCmd_ = new G4UIcmdWithADouble("/conf/calo/setParticleToCalorimeterDistance",this);
	setCaloParticleToColarimeterDistanceCmd_->SetParameterName("particletocalorimeterdistance", false);
	setCaloParticleToColarimeterDistanceCmd_->SetGuidance("Particle to calorimeter distance in cm");

	setCubeUseCmd_ = new G4UIcmdWithABool("/conf/cube/useCube",this);
	setCubeUseCmd_->SetParameterName("usecube", false);
	setCubeUseCmd_->SetGuidance("Whether use of cube to estimate skipped particlues or not");
	setCubeISOToDetectorDistanceCmd_ = new G4UIcmdWithADouble("/conf/cube/setISOToDetectorDistance",this);
	setCubeISOToDetectorDistanceCmd_->SetParameterName("isotodetectordistance", false);
	setCubeISOToDetectorDistanceCmd_->SetGuidance("ISO to detector distance in cm");
	setCubeGapCmd_ = new G4UIcmdWithADouble("/conf/cube/setGap",this);
	setCubeGapCmd_->SetParameterName("gap", false);
	setCubeSamplesNumberCmd_ = new G4UIcmdWithAnInteger("/conf/cube/setSamplesNumber",this);
	setCubeSamplesNumberCmd_->SetParameterName("samplesnumber", false);
	setCubeSamplesNumberCmd_->SetGuidance("Number of samples for one direction of one plane");
}


DetectorMessenger::~DetectorMessenger()
{
  delete directory_;
  delete caloDirectory_;
  delete cubeDirectory_;
  delete setCaloLayerThickCmd_;
  delete setCaloRMinCmd_;
  delete setCaloRMaxCmd_;
  delete setCaloLayersNumberZCmd_;
  delete setCaloLayersNumberRadialCmd_;
  delete setCaloLayersNumberPhiCmd_;
  delete setCaloPhiCmd_;
  delete setCaloGapCmd_;
  delete setCaloParticleToColarimeterDistanceCmd_;
  delete setCubeUseCmd_;
  delete setCubeISOToDetectorDistanceCmd_;
  delete setCubeGapCmd_;
  delete setCubeSamplesNumberCmd_;
}


void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	// TODO: "if (command == command_)" is faster
	detectorConstruction_->SetParam(command->GetCommandPath(), newValue);
}



