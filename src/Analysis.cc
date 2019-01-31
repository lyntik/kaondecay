/*
 * Analysis.cc
 *
 *  Created on:
 *      Author: fna
 */

#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4Track.hh"

#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"

using namespace CLHEP;


Analysis* Analysis::singleton = nullptr;

Analysis::Analysis() :
	eventID_(0),
	pos_(0),
	time_(0)
{
	tree_ = new TTree("decay", "title");
	tree_->Branch("eventID", &eventID_);
	tree_->Branch("pos", &pos_);
	tree_->Branch("time", &time_);

	histoPos_ = new TH1D("decayPos", "Z Position of Decay", 100, 0, 3500);
	histoPos_->GetYaxis()->SetTitle("events");
	histoPos_->GetXaxis()->SetTitle("z_{decay} / mm");
	histoPos_->StatOverflows();

	histoTime_ = new TH1D("decayTime", "Time of Decay", 100, 0, 3);
	histoTime_->GetYaxis()->SetTitle("events");
	histoTime_->GetXaxis()->SetTitle("t_{decay} / ns");
	histoTime_->StatOverflows();
}

Analysis::~Analysis() 
{
	delete tree_;
	delete histoPos_;
	delete histoTime_;
}

void Analysis::AddTrack( const G4Track * aTrack ) 
{
    pos_ = aTrack->GetPosition().z();
    time_ = aTrack->GetGlobalTime();
}


void Analysis::AddParticleRecord(ParticleRecordInt3Type *record)
{
	record->Branch(tree_);
}


void Analysis::PrepareNewRun(const G4Run* /*aRun*/ )
{
	eventID_ = 0;
	tree_->Reset();
	histoPos_->Reset();
	histoTime_->Reset();
}

void Analysis::PrepareNewEvent(const G4Event* /*anEvent*/)
{
	pos_ = 0;
	time_ = 0;
	eventID_++;
}


void Analysis::EndOfEvent(const G4Event* /*anEvent*/)
{
	if (pos_) // should we add event without decay ?
	{
		tree_->Fill();
		histoPos_->Fill(-pos_);
		histoTime_->Fill(time_);
	}
}

void Analysis::EndOfRun(const G4Run* /*aRun*/)
{
	TFile* outfile = TFile::Open("tree.root","recreate");
	tree_->Write();
	outfile->Close();

	outfile = TFile::Open("histo.root","recreate");
	histoPos_->Write();
	histoTime_->Write();
	outfile->Close();

}


