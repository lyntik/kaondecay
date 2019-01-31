/*
 * DetectorMessenger.hh
 *
 *  Created on:
 *      Author: fna
 */


#pragma once

#include "G4UImessenger.hh"
#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;



class DetectorMessenger: public G4UImessenger
{
public:
    DetectorMessenger(DetectorConstruction* );
   ~DetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand* command, G4String newValue);
    
private:
    DetectorConstruction*      detectorConstruction_;
    G4UIdirectory*             directory_;
    G4UIdirectory*             caloDirectory_;
    G4UIdirectory*             cubeDirectory_;
    G4UIcmdWithADouble*        setCaloLayerThickCmd_;
    G4UIcmdWithADouble*        setCaloRMinCmd_;
    G4UIcmdWithADouble*        setCaloRMaxCmd_;
    G4UIcmdWithAnInteger*        setCaloLayersNumberZCmd_;
    G4UIcmdWithAnInteger*        setCaloLayersNumberRadialCmd_;
    G4UIcmdWithAnInteger*        setCaloLayersNumberPhiCmd_;
    G4UIcmdWithADouble*        setCaloPhiCmd_;
    G4UIcmdWithADouble*        setCaloGapCmd_;
    G4UIcmdWithADouble*        setCaloParticleToColarimeterDistanceCmd_;

    G4UIcmdWithABool*			setCubeUseCmd_;
    G4UIcmdWithADouble*			setCubeISOToDetectorDistanceCmd_;
    G4UIcmdWithADouble*			setCubeGapCmd_;
    G4UIcmdWithAnInteger*		setCubeSamplesNumberCmd_;

};




