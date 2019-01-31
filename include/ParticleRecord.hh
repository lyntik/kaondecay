/*
 * ParticleRecord.hh
 *
 *  Created on:
 *      Author: fna
 */


#pragma once

#include "G4SystemOfUnits.hh"
#include "TTree.h"

template<class T, unsigned int IndexDimension = 3>
class ParticleRecord
{
public:
	ParticleRecord(const G4String& prefix);
	virtual ~ParticleRecord();

	G4double Energy();
	void SetEnergy(G4double value);
	void AddEnergy(G4double value);

	T Index(G4int i);
	void SetIndex(G4int i, T value);

	void Branch(TTree *tree);

	void Clear();
	inline bool isInited();

private:
	T index_[IndexDimension];
	G4String prefix_;
	G4double energy_;
	bool inited_;


};



//========= Implementation: ParticleRecord<T>

template<class T, unsigned int IndexDimension>
ParticleRecord<T, IndexDimension>::ParticleRecord(const G4String& prefix) :
	prefix_(prefix),
	energy_(0),
	inited_(false)
{

}

template<class T, unsigned int IndexDimension>
ParticleRecord<T, IndexDimension>::~ParticleRecord()
{

}


template<class T, unsigned int IndexDimension>
T ParticleRecord<T, IndexDimension>::Index(G4int i)
{
	return index_[i];
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::SetIndex(G4int i, T value)
{
	index_[i] = value;
	inited_ = true;
}

template<class T, unsigned int IndexDimension>
G4double ParticleRecord<T, IndexDimension>::Energy()
{
	return energy_;
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::SetEnergy(G4double value)
{
	energy_ = value;
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::AddEnergy(G4double value)
{
	energy_ += value;
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::Clear()
{
	inited_ = false;
	energy_ = 0;
	memset(&index_, 0, sizeof(index_));
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::Branch(TTree *tree)
{
	tree->Branch((prefix_ + "energy").c_str(), &energy_);
	char sz[50];
	for (unsigned int i = 0; i < IndexDimension; i++)
	{
		sprintf(sz, "%sind%d", prefix_.c_str(), i);
		tree->Branch(sz, &index_[i]);
	}
}

template<class T, unsigned int IndexDimension>
inline bool ParticleRecord<T, IndexDimension>::isInited()
{
	return inited_;
}



typedef ParticleRecord<G4int, 3> ParticleRecordInt3Type;



