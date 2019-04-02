/*
 * CaloSensitiveDetector.cc
 *
 *  Created on:
 *      Author: fna
 */


#include "CaloSensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"

#include "Analysis.hh"

# ifdef GENERATE_POINTCLOUDS
static FILE * tmpStorage[3];
# endif

CaloSensitiveDetector::CaloSensitiveDetector(G4String name)
        : G4VSensitiveDetector(name)
        , muPlus_("muplus_depos_")
        , muMinus_("muminus_depos_") {
    Analysis::GetInstance()->AddParticleRecord(&muPlus_);
    Analysis::GetInstance()->AddParticleRecord(&muMinus_);
    # ifdef GENERATE_POINTCLOUDS
    tmpStorage[0] = fopen( "/tmp/points-mu+.dat", "w" );
    tmpStorage[1] = fopen( "/tmp/points-mu-.dat", "w" );
    tmpStorage[2] = fopen( "/tmp/points-other.dat", "w" );
    # endif
}

CaloSensitiveDetector::~CaloSensitiveDetector() {
    # ifdef GENERATE_POINTCLOUDS
    fclose(tmpStorage[0]);  // XXX
    fclose(tmpStorage[1]);  // XXX
    fclose(tmpStorage[2]);  // XXX
    # endif
}

void CaloSensitiveDetector::Initialize(G4HCofThisEvent*) {
    muPlus_.Clear();
    muMinus_.Clear();

    setMuPlus_.clear();
    setMuMinus_.clear();
}

// NOTE: this method is invoked each time the particle is traced WITHIN the
// sensitive volume associated with the sensitive detector.
// TODO: get rid of dirty atoi() conversion figuring out the calorimeter ID
// based on the volume name.
G4bool CaloSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    // Get the "touchable" -- an instance of the volume affected by hit
    const G4TouchableHandle touchable = step
                                      ->GetPreStepPoint()
                                      ->GetTouchableHandle()
                                      ;
    // Obtain the entry point of the particle
    const G4ThreeVector & ep = step->GetPreStepPoint()->GetPosition()
                      , & p = step->GetPreStepPoint()->GetMomentum()
                      ;
    // Get the pointer to current track instance
    G4Track * track = step->GetTrack();
    // Obtain the track identifier
    G4int trackID = track->GetTrackID();
    // Obtain the particle name (from definition being bound to track)
    //G4String particleName = track->GetDefinition()->GetParticleName();
    // Get the volume name
    G4String volume = track->GetVolume()->GetName();
    // Get the calorimeter number based on the volume name
    int caloNumber = atoi(volume.c_str() + volume.length() - 1);

    // Fill the records in case of muons (+/-)
    if( track->GetParticleDefinition() == G4MuonPlus::MuonPlusDefinition() ) {
        if( !setMuPlus_.size() ) {  // store only the first mu+ particle
            setMuPlus_.insert(trackID);
            muPlus_.SetIndex(0, caloNumber);
            muPlus_.SetIndex(1, touchable->GetCopyNumber(1) + 1);
            muPlus_.SetIndex(2, touchable->GetCopyNumber(2) + 1);
            muPlus_.SetEP( ep );
            muPlus_.SetMomentum( p );
            # ifdef GENERATE_POINTCLOUDS
            fprintf( tmpStorage[0], "%e\t%e\t%e\n", ep[0], ep[1], ep[2] );
            # endif
        }
    } else if (track->GetParticleDefinition() == G4MuonMinus::MuonMinusDefinition()) {
        if (!setMuMinus_.size()) {  // store only the first mu- particle
            setMuMinus_.insert(trackID);
            muMinus_.SetIndex(0, caloNumber);
            muMinus_.SetIndex(1, touchable->GetCopyNumber(1) + 1);
            muMinus_.SetIndex(2, touchable->GetCopyNumber(2) + 1);
            muMinus_.SetEP( ep );
            muMinus_.SetMomentum( p );
            # ifdef GENERATE_POINTCLOUDS
            fprintf( tmpStorage[1], "%e\t%e\t%e\n", ep[0], ep[1], ep[2] );
            # endif
        }
    }
    # ifdef GENERATE_POINTCLOUDS
    else {
        // a LOT of data here
        fprintf( tmpStorage[2], "%e\t%e\t%e\n", ep[0], ep[1], ep[2] );
    }
    # endif

    G4String layer;
    int i = 0;
    while (volume[i] != '_') layer += volume[i++];

    if (!setMuPlus_.count(trackID) && !setMuMinus_.count(trackID)) {
        if (setMuPlus_.count(track->GetParentID()))
            setMuPlus_.insert(trackID);
        else if (setMuMinus_.count(track->GetParentID()))
            setMuMinus_.insert(trackID);
    }

    if (layer == "Scin") {
        if (setMuPlus_.count(trackID))
            muPlus_.AddEnergy(step->GetTotalEnergyDeposit());
        if (setMuMinus_.count(trackID))
            muMinus_.AddEnergy(step->GetTotalEnergyDeposit());
    }

    return true;
}

void CaloSensitiveDetector::EndOfEvent(G4HCofThisEvent*) {

}


