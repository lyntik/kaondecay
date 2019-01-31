/*
 * CaloSensitiveDetector.hh
 *
 *  Created on:
 *      Author: fna
 */

#pragma once

#include "G4VSensitiveDetector.hh"
#include "ParticleRecord.hh"
class G4Step;
class RunAction;


class CaloSensitiveDetector: public G4VSensitiveDetector
{
public:
	CaloSensitiveDetector(G4String name = "");
    ~CaloSensitiveDetector();

    void Initialize(G4HCofThisEvent*);
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    

private:
    ParticleRecordInt3Type muPlus_;
    ParticleRecordInt3Type muMinus_;

    std::set<int> setMuPlus_;
    std::set<int> setMuMinus_;

};

