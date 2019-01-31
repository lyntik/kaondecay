/*
 * PrimaryGeneratorAction.cc
 *
 *  Created on:
 *      Author: fna
 */

#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

using namespace CLHEP;


PrimaryGeneratorAction::PrimaryGeneratorAction()
{
	gun_ = new G4ParticleGun(1);

	G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition* kaon = particleTable->FindParticle("kaon+");
	gun_->SetParticleDefinition(kaon);
	gun_->SetParticlePosition(G4ThreeVector(0.,0.,0.));
	gun_->SetParticleEnergy(0.1*GeV);
	gun_->SetParticleMomentumDirection( G4ThreeVector(0., 0., -1.) );
  
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{ 
	gun_->GeneratePrimaryVertex(anEvent);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete gun_;
}



