/*
 * RunAction.hh
 *
 *  Created on:
 *      Author: fna
 */


#pragma once

#include "G4UserRunAction.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
	RunAction();
	virtual ~RunAction() {};

	void BeginOfRunAction(const G4Run*);
	void EndOfRunAction(const G4Run*);
private:
};


