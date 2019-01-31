/*
 * CubeSensitiveDetector.hh
 *
 *  Created on:
 *      Author: fna
 */

#pragma once

#include "G4VSensitiveDetector.hh"
#include "G4ThreeVector.hh"
#include "ParticleRecord.hh"

class G4Step;
class RunAction;


class CubeSensitiveDetector: public G4VSensitiveDetector
{
public:
	CubeSensitiveDetector(G4String name = "");
    ~CubeSensitiveDetector();

    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    

private:

    ParticleRecordInt3Type muPlus_;
    ParticleRecordInt3Type muMinus_;

};

