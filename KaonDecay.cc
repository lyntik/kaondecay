/*
 * kaonDecay.cc
 *
 *  Created on:
 *      Author: fna
 */

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4Version.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "PhysicsList.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "StackingAction.hh"
#include "EventAction.hh"
#include "RunAction.hh"



int main(int argc, char** argv)
{
	G4RunManager * runManager = new G4RunManager();

	// initializations
	G4VUserDetectorConstruction* detector = new DetectorConstruction();
	runManager->SetUserInitialization(detector);

	G4VUserPhysicsList* physics = new PhysicsList();
	runManager->SetUserInitialization(physics);

	// generator
	G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
	runManager->SetUserAction(gen_action);

	// optional actions
	StackingAction* aStackingAction = new StackingAction();
	runManager->SetUserAction(aStackingAction);

	EventAction* anEventAction = new EventAction();
	runManager->SetUserAction( anEventAction );

	RunAction* aRunAction = new RunAction();
	runManager->SetUserAction( aRunAction );

	// As we want to change configuration parameters via .mac file, we need call /run/initialize after setting the parameters there
	// runManager->Initialize();

	//Initialize the visualization manager
	G4VisManager* visManager = new G4VisExecutive();
	visManager->Initialize();

	G4UImanager * UImanager = G4UImanager::GetUIpointer();

	if (argc != 1)
	{
		// batch mode
		//command line contains name of the macro to execute
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	else
	{
		G4UIExecutive * ui = new G4UIExecutive(argc,argv);
		if (ui->IsGUI())
		  UImanager->ApplyCommand("/control/execute visQt.mac");
		else
		  UImanager->ApplyCommand("/control/execute vis.mac");

		ui->SessionStart();
		delete ui;
	}

	// Free the store: user actions, physics_list and detector_description are
	//                 owned and deleted by the run manager, so they should not
	//                 be deleted in the main() program !

	delete runManager;

	return 0;
}
