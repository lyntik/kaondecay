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


CaloSensitiveDetector::CaloSensitiveDetector(G4String name): G4VSensitiveDetector(name),
	muPlus_("muplus_depos_"),
	muMinus_("muminus_depos_")
{
	Analysis::GetInstance()->AddParticleRecord(&muPlus_);
	Analysis::GetInstance()->AddParticleRecord(&muMinus_);
}

CaloSensitiveDetector::~CaloSensitiveDetector()
{

}


void CaloSensitiveDetector::Initialize(G4HCofThisEvent*)
{
	muPlus_.Clear();
	muMinus_.Clear();

	setMuPlus_.clear();
	setMuMinus_.clear();

}

G4bool CaloSensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
	const G4TouchableHandle touchable = step->GetPreStepPoint()->GetTouchableHandle();
	G4Track* track = step -> GetTrack();
	G4int trackID = track->GetTrackID();
	G4String particleName = track->GetDefinition()->GetParticleName();
	G4String volume = track->GetVolume()->GetName();
	int caloNumber = atoi(volume.c_str() + volume.length() - 1);

	if (track->GetParticleDefinition() == G4MuonPlus::MuonPlusDefinition())
	{
		if (!setMuPlus_.size())
		{
			setMuPlus_.insert(trackID);
			muPlus_.SetIndex(0, caloNumber);
			muPlus_.SetIndex(1, touchable->GetCopyNumber(1) + 1);
			muPlus_.SetIndex(2, touchable->GetCopyNumber(2) + 1);
		}
	}
	else if (track->GetParticleDefinition() == G4MuonMinus::MuonMinusDefinition())
	{
		if (!setMuMinus_.size())
		{
			setMuMinus_.insert(trackID);
			muMinus_.SetIndex(0, caloNumber);
			muMinus_.SetIndex(1, touchable->GetCopyNumber(1) + 1);
			muMinus_.SetIndex(2, touchable->GetCopyNumber(2) + 1);
		}
	}

	G4String layer;
	int i = 0;
	while (volume[i] != '_') layer += volume[i++];

	if (!setMuPlus_.count(trackID) && !setMuMinus_.count(trackID))
	{
		if (setMuPlus_.count(track->GetParentID()))
			setMuPlus_.insert(trackID);
		else if (setMuMinus_.count(track->GetParentID()))
			setMuMinus_.insert(trackID);
	}

	if (layer == "Scin")
	{
		if (setMuPlus_.count(trackID))
			muPlus_.AddEnergy(step->GetTotalEnergyDeposit());
		if (setMuMinus_.count(trackID))
			muMinus_.AddEnergy(step->GetTotalEnergyDeposit());
	}

	return true;
}

void CaloSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{

}


