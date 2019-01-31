/*
 * CubeSensitiveDetector.cc
 *
 *  Created on:
 *      Author: fna
 */

#include "CubeSensitiveDetector.hh"

#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"

#include "Analysis.hh"


CubeSensitiveDetector::CubeSensitiveDetector(G4String name): G4VSensitiveDetector(name),
	muPlus_("muplus_skip_"),
	muMinus_("muminus_skip_")
{
	Analysis::GetInstance()->AddParticleRecord(&muPlus_);
	Analysis::GetInstance()->AddParticleRecord(&muMinus_);

}

CubeSensitiveDetector::~CubeSensitiveDetector()
{

}


void CubeSensitiveDetector::Initialize(G4HCofThisEvent*)
{
	muPlus_.Clear();
	muMinus_.Clear();
}

G4bool CubeSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
	const G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
	G4Track* track = step -> GetTrack();
	G4String particleName = track->GetDefinition()->GetParticleName();
	G4String volume = track->GetVolume()->GetName();
	int matrixNumber = atoi(volume.c_str() + volume.length() - 1);

	auto fill = [&] (ParticleRecordInt3Type& record) {
		record.SetIndex(0, matrixNumber);
		record.SetIndex(1, touchable->GetCopyNumber(0) + 1);
		record.SetIndex(2, touchable->GetCopyNumber(1) + 1);
		record.SetEnergy(track->GetDynamicParticle()->GetTotalEnergy());
	};

	if (track->GetParticleDefinition() == G4MuonPlus::MuonPlusDefinition())
	{
		if (!muPlus_.isInited())
			fill(muPlus_);
	}
	else if (track->GetParticleDefinition() == G4MuonMinus::MuonMinusDefinition())
	{
		if (!muMinus_.isInited())
			fill(muMinus_);
	}

	return false;
}


void CubeSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
}

