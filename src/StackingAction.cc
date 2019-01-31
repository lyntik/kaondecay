/*
 * StackingAction.cc
 *
 *  Created on:
 *      Author: fna
 */

#include "StackingAction.hh"
#include "G4ClassificationOfNewTrack.hh"
#include "G4Track.hh"
#include "Analysis.hh"



StackingAction::StackingAction()
{

}


StackingAction::~StackingAction()
{

}


G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack( const G4Track * aTrack )
{
	G4ClassificationOfNewTrack result( fUrgent );

	if (aTrack->GetParentID() > 0) //This is a secondary
		if (aTrack->GetDefinition()->GetPDGEncoding() == -13) // mu+
			Analysis::GetInstance()->AddTrack(aTrack);

	return result;
}


