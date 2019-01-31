/*
 * EventAction.hh
 *
 *  Created on:
 *      Author: fna
 */

#pragma once


#include "G4UserEventAction.hh"
#include "G4String.hh"
class G4Event;

class EventAction : public G4UserEventAction
{
public:
	EventAction();
	virtual ~EventAction() {};

	void BeginOfEventAction(const G4Event* anEvent);
	void EndOfEventAction(const G4Event* anEvent);
private:
};


