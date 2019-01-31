/*
 * PhysicsList.hh
 *
 *  Created on:
 *      Author: fna
 */

#pragma once

#include "G4VModularPhysicsList.hh"


class PhysicsList: public G4VModularPhysicsList
{
public:
	PhysicsList();
	virtual ~PhysicsList();

protected:
  void ConstructParticle();
  void ConstructProcess();
  void SetCuts();
private:

  void ConstructEM();
  void ConstructDecay();
};



