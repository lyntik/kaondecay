/*
 * PhysicsList.cc
 *
 *  Created on:
 *      Author: fna
 */


#include "PhysicsList.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

#include "G4DecayTable.hh"
#include "G4Decay.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4PhaseSpaceDecayChannel.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"




PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{
	//SetVerboseLevel(1);
}


PhysicsList::~PhysicsList()
{ 
}


void PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  


void PhysicsList::ConstructParticle()
{
	//G4MuonPlus::MuonPlusDefinition();
	//G4MuonMinus::MuonMinusDefinition();
	//G4KaonPlus::KaonPlusDefinition();

	// Full particle list
	G4BosonConstructor  pBosonConstructor;
	pBosonConstructor.ConstructParticle();

	G4LeptonConstructor pLeptonConstructor;
	pLeptonConstructor.ConstructParticle();

	G4MesonConstructor pMesonConstructor;
	pMesonConstructor.ConstructParticle();

	G4BaryonConstructor pBaryonConstructor;
	pBaryonConstructor.ConstructParticle();

	G4IonConstructor pIonConstructor;
	pIonConstructor.ConstructParticle();

	G4ShortLivedConstructor pShortLivedConstructor;
	pShortLivedConstructor.ConstructParticle();

	// Redefining decay table
	// kaon+ -> mu+ mu-
	G4DecayTable* table = new G4DecayTable();
	table->Insert(new G4PhaseSpaceDecayChannel("kaon+", 0.6355, 2, "mu+", "mu-"));
	G4KaonPlus::KaonPlusDefinition()->SetDecayTable(table);

}


void PhysicsList::ConstructProcess()
{
	AddTransportation();

	ConstructEM();
	ConstructDecay();
}


void PhysicsList::ConstructEM()
{
	auto theParticleIterator = GetParticleIterator();

	theParticleIterator->reset();
	while( (*theParticleIterator)() )
	{
		G4ParticleDefinition* particle = theParticleIterator->value();
		G4ProcessManager* pmanager = particle->GetProcessManager();
		G4String particleName = particle->GetParticleName();

		if (particleName == "gamma")
		{
			// gamma
			pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
			pmanager->AddDiscreteProcess(new G4ComptonScattering);
			pmanager->AddDiscreteProcess(new G4GammaConversion);
		}
		else if (particleName == "e-")
		{
			//electron
			pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
			pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
			pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);
		}
		else if (particleName == "e+")
		{
			//positron
			pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
			pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
			pmanager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);
			pmanager->AddProcess(new G4eplusAnnihilation,   0,-1, 4);
		}
		else if( particleName == "mu+" || particleName == "mu-")
		{
			// muon
			pmanager->AddProcess(new G4MuMultipleScattering,-1, 1, 1);
			pmanager->AddProcess(new G4MuIonisation,-1, 2, 2);
			pmanager->AddProcess(new G4MuBremsstrahlung,-1, 3, 3);
			pmanager->AddProcess(new G4MuPairProduction,-1, 4, 4);
		}
		else if (particleName == "pi-" || particleName == "pi+")
		{
			//pion
			pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
			pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
			pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
			pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);
		}
	}
}


void PhysicsList::ConstructDecay()
{
	G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
	// Add Decay Process
	ph->RegisterProcess(new G4Decay(), G4KaonPlus::KaonPlusDefinition());
}










