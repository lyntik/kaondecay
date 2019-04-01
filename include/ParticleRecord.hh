/*
 * ParticleRecord.hh
 *
 *  Created on:
 *      Author: fna
 */


#pragma once

#include "G4SystemOfUnits.hh"
#include "TTree.h"

///@brief A particle record memorizing the calorimeter hit event.
///
/// With these record instances we accumulate the full energy deposited by
/// various particle type.
///
/// Templated indices refer to numbers within the replicas sequence:
///     * longitudial, radial, azimuthal -- for radial calorimeter
///     * longitudial, columnar, row -- for rectangular calo
template<class T, unsigned int IndexDimension = 3>
class ParticleRecord {
private:
    T index_[IndexDimension];
    G4String prefix_;
    G4double energy_;
    bool inited_;
public:
    ParticleRecord(const G4String& prefix);
    virtual ~ParticleRecord();

    /// Returns energy value accumulated.
    G4double Energy() const;
    /// Directly sets the edep.
    void SetEnergy(G4double value);
    /// Linear e-dep scoring.
    void AddEnergy(G4double value);

    /// I-th index getter.
    T Index(G4int i) const;
    /// I-th index setter.
    void SetIndex(G4int i, T value);

    /// Binds the given TTree instance with indices branch (one branch per
    /// index)
    void Branch(TTree *tree);

    /// Returns the instance to pre-initialized state.
    void Clear();
    inline bool isInited();
};



//========= Implementation: ParticleRecord<T>

template<class T, unsigned int IndexDimension>
ParticleRecord<T, IndexDimension>::ParticleRecord(const G4String& prefix)
    : prefix_(prefix)
    , energy_(0)
    , inited_(false) { }

template<class T, unsigned int IndexDimension>
ParticleRecord<T, IndexDimension>::~ParticleRecord() { }


template<class T, unsigned int IndexDimension>
T ParticleRecord<T, IndexDimension>::Index(G4int i) const {
    return index_[i];
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::SetIndex(G4int i, T value) {
    index_[i] = value;
    inited_ = true;
}

template<class T, unsigned int IndexDimension>
G4double ParticleRecord<T, IndexDimension>::Energy() const {
    return energy_;
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::SetEnergy(G4double value) {
    energy_ = value;
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::AddEnergy(G4double value) {
    energy_ += value;
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::Clear() {
    inited_ = false;
    energy_ = 0;
    memset(&index_, 0, sizeof(index_));
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::Branch(TTree *tree) {
    tree->Branch((prefix_ + "energy").c_str(), &energy_);
    char sz[50];
    for (unsigned int i = 0; i < IndexDimension; i++) {
        sprintf(sz, "%sind%d", prefix_.c_str(), i);
        tree->Branch(sz, &index_[i]);
    }
}

template<class T, unsigned int IndexDimension>
inline bool ParticleRecord<T, IndexDimension>::isInited() {
    return inited_;
}



typedef ParticleRecord<G4int, 3> ParticleRecordInt3Type;

