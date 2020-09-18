
#ifndef MCPARTICLE_H
#define MCPARTICLE_H 1

#include "G4Allocator.hh"
#include "G4THitsCollection.hh"
#include "G4ThreeVector.hh"
#include "G4String.hh"

class MCParticle
{
    public:
        MCParticle();
        MCParticle(const MCParticle&);
        ~MCParticle();

        // operators
        const MCParticle& operator=(const MCParticle&);
        G4int operator==(const MCParticle&) const;
        
        inline void* operator new(size_t);
        inline void  operator delete(void*);

        void Initialize();
        // Set methods
        void Setid      (G4int id)          { _id = id; };
        void SetPDG     (G4int PDG)         { _PDG = PDG; };
        void SetParentID(G4int ParentID)    { _ParentID = ParentID; };
        void SetEnergy  (G4double E)        { _Energy = E; };
        void SetMomentum(G4ThreeVector xyz) { _Momentum = xyz; };
        void SetVPos    (G4ThreeVector xyz) { _VPos = xyz; };
        void SetEPos    (G4ThreeVector xyz) { _EPos = xyz; };
        void SetProcess (G4String p)        { _CreateProcess = p; };
        void SetProcessType (G4int p)       { _ProcessType = p; };
        void SetProcessSubType (G4int p)    { _ProcessSubType = p; };
        
        // Get methods
        G4int Getid() const                 { return _id; };
        G4int GetPDG() const                { return _PDG; };
        G4int GetParentID() const           { return _ParentID; };
        G4double GetEnergy() const          { return _Energy; };
        G4ThreeVector GetMomentum() const   { return _Momentum; };
        G4ThreeVector GetVPos() const       { return _VPos; };
        G4ThreeVector GetEPos() const       { return _EPos; };
        G4String      GetProcess() const    { return _CreateProcess; };
        G4int      GetProcessType() const{ return _ProcessType; };
        G4int   GetProcessSubType() const{ return _ProcessSubType; };


    private:

        G4int           _id;
        G4int           _PDG;
        G4int           _ParentID;
        G4double        _Energy;
        G4ThreeVector   _Momentum;
        G4ThreeVector   _VPos; // vertex position
        G4ThreeVector   _EPos; // end position
        G4String        _CreateProcess;
        G4int           _ProcessType;
        G4int           _ProcessSubType;


};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<MCParticle> MCParticleCollection;

extern G4Allocator<MCParticle> MCParticleAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* MCParticle::operator new(size_t)
{
  void *particle;
  particle = (void *) MCParticleAllocator.MallocSingle();
  return particle;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void MCParticle::operator delete(void *p)
{
  MCParticleAllocator.FreeSingle((MCParticle*) p);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



#endif
