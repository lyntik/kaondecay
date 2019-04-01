/*
 * CaloSensitiveDetector.hh
 *
 *  Created on:
 *      Author: fna
 */

#pragma once

# include "G4VSensitiveDetector.hh"
# include "ParticleRecord.hh"

class G4Step;
class RunAction;

///@brief A sensitive detector class for calorimeter detectors.
///
/// This class implements an important interface: while being bound to some
/// logical volume, the instance of this class provides a callback methods that
/// will be called each time the particle enters the volume.
class CaloSensitiveDetector : public G4VSensitiveDetector {
private:
    ParticleRecordInt3Type muPlus_;
    ParticleRecordInt3Type muMinus_;

    std::set<int> setMuPlus_;
    std::set<int> setMuMinus_;
public:
    /// Ctr: binds the records references
    CaloSensitiveDetector(G4String name = "");
    ~CaloSensitiveDetector() {}

    /// Clears particle records.
    void Initialize(G4HCofThisEvent*);
    /// Invoked on "hit" (particle enters the bound volume).
    G4bool ProcessHits(G4Step*, G4TouchableHistory*);
    /// Invoked on "end of event" -- Geant4 is done simulating single event.
    void EndOfEvent(G4HCofThisEvent*);
};

