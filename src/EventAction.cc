/*
 * EventAction.cc
 *
 *  Created on:
 *      Author: fna
 */

#include "EventAction.hh"
#include "Analysis.hh"

EventAction::EventAction()
{
}


void EventAction::BeginOfEventAction(const G4Event* anEvent )
{
	if ( anEvent->GetEventID() % 1000 == 0 )
	{
		G4cout<<"Starting Event: "<<anEvent->GetEventID()<<G4endl;
	}
	Analysis::GetInstance()->PrepareNewEvent(anEvent);
}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
	Analysis::GetInstance()->EndOfEvent(anEvent);
}

