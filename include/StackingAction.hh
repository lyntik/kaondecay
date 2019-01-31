/*
 * StackingAction.hh
 *
 *  Created on:
 *      Author: fna
 */

#pragma once


#include "globals.hh"
#include "G4UserStackingAction.hh"

class G4Track;
class Analysis;

class StackingAction : public G4UserStackingAction
{
public:
	StackingAction();
	virtual ~StackingAction();

	virtual G4ClassificationOfNewTrack ClassifyNewTrack( const G4Track* aTrack );

private:

};

