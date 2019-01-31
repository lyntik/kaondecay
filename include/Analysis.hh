/*
 * Analysis.hh
 *
 *  Created on:
 *      Author: fna
 */

#pragma once

#include "G4Event.hh"
#include "G4Run.hh"
#include "ParticleRecord.hh"

// ROOT
class TTree;
class TH1D;

class G4Track;

class Analysis
{

public:

  static Analysis* GetInstance()
  {
	  if ( Analysis::singleton == nullptr )
		  Analysis::singleton = new Analysis();
	  return Analysis::singleton;
  }
  ~Analysis();

  void PrepareNewRun(const G4Run* aRun);
  void PrepareNewEvent(const G4Event* anEvent);
  void EndOfEvent(const G4Event* anEvent);
  void EndOfRun(const G4Run* aRun);

  void AddTrack( const G4Track * aTrack );
  void AddParticleRecord(ParticleRecordInt3Type *record);
private:
  Analysis();
  static Analysis* singleton;

  TTree *tree_;
  TH1D *histoPos_;
  TH1D *histoTime_;
  int eventID_;
  G4double pos_;
  G4double time_;


};


