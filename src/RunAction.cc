/*
 * RunAction.cc
 *
 *  Created on:
 *      Author: fna
 */


#include "RunAction.hh"
#include "Analysis.hh"

RunAction::RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run* aRun )
{
	Analysis::GetInstance()->PrepareNewRun(aRun);
}

void RunAction::EndOfRunAction( const G4Run* aRun )
{
	Analysis::GetInstance()->EndOfRun(aRun);
}
