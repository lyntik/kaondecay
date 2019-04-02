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
    G4double position_[3];
    G4double momentum_[3];
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

    /// Entry point setter
    void SetEP(const G4ThreeVector &);

    /// Momentum at entry point setter
    void SetMomentum(const G4ThreeVector &);

};



//========= Implementation: ParticleRecord<T>

// Enable this to get the arrow-like branches
//#define ARRAY_BRANCHES_ALLOWED

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
    for(unsigned char i = 0; i < 3; ++i){
        position_[i] = momentum_[i] = std::nan("0");
    }
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::SetEP( const G4ThreeVector & v ) {
   position_[0] = v[0];
   position_[1] = v[1];
   position_[2] = v[2];
}

template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::SetMomentum( const G4ThreeVector & v ) {
   momentum_[0] = v[0];
   momentum_[1] = v[1];
   momentum_[2] = v[2];
}


template<class T, unsigned int IndexDimension>
void ParticleRecord<T, IndexDimension>::Branch(TTree *tree) {
    char bf[128];
    tree->Branch((prefix_ + "energy").c_str(), &energy_);
    # ifdef ARRAY_BRANCHES_ALLOWED
    tree->Branch( (prefix_ + "entryPoint").c_str()
                , position_
                , "x/D:y/D:z/D"
                , sizeof(position_) );
    tree->Branch( (prefix_ + "momentum").c_str()
                , momentum_
                , "x/D:y/D:z/D"
                , sizeof(momentum_) );
    # else
    for( unsigned char i = 0; i < 3; ++i ) {
        snprintf( bf, sizeof(bf), "%sentryPoint_%d"
               , prefix_.c_str(), (int) i );
        tree->Branch( bf, position_ + i  );
        snprintf( bf, sizeof(bf), "%smomentum_%d"
               , prefix_.c_str(), (int) i );
        tree->Branch( bf, momentum_ + i  );
    }
    # endif
    char sz[50];
    for( unsigned int i = 0; i < IndexDimension; i++ ) {
        sprintf(sz, "%sind%d", prefix_.c_str(), i);
        tree->Branch(sz, &index_[i]);
    }
}

template<class T, unsigned int IndexDimension>
inline bool ParticleRecord<T, IndexDimension>::isInited() {
    return inited_;
}



typedef ParticleRecord<G4int, 3> ParticleRecordInt3Type;

