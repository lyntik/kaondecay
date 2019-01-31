/*
 * PrimaryGeneratorAction.hh
 *
 *  Created on:
 *      Author: fna
 */

#pragma once


#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleGun;

 

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:

  PrimaryGeneratorAction();
  virtual ~PrimaryGeneratorAction();

  void GeneratePrimaries(G4Event*);

private:
  G4ParticleGun* gun_;

};




