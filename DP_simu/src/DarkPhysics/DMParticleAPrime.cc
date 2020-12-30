#include "DarkPhysics/DMParticleAPrime.hh"

#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4DalitzDecayChannel.hh"
#include "G4DecayTable.hh"

DMParticleAPrime *DMParticleAPrime::theInstance = nullptr;

DMParticleAPrime *DMParticleAPrime::Definition() {
    if (theInstance) {
        return theInstance;
    }
    const G4String name = "DMParticle";
    // search in particle table]
    G4ParticleTable *pTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *anInstance = pTable->FindParticle(name);
    //here don't define decay width
    //G4double BenchmarkWidth = 1./(64.*pi)*30.*30.*30.*0.001*0.001;
    if (!anInstance) {
        anInstance = new G4ParticleDefinition(
                /* Name ..................... */ name,
                // /* Mass ..................... */ 16.7 * MeV,
                /* Mass ..................... */ 16.7 * MeV,
                /* Decay width .............. */ 0.,
                /* Charge ................... */ 0.,
                /* 2*spin ................... */ 0,
                /* parity ................... */ +1,
                /* C-conjugation ............ */ 0,
                /* 2*Isospin ................ */ 0,
                /* 2*Isospin3 ............... */ 0,
                /* G-parity ................. */ 0,
                /* type ..................... */ "boson",
                /* lepton number ............ */ 0,
                /* baryon number ............ */ 0,
                /* PDG encoding ............. */
                                                 500012, // https://pdg.lbl.gov/2019/reviews/rpp2019-rev-monte-carlo-numbering.pdf
                /* stable ................... */ true,
                /* lifetime.................. */ 0,
                /* decay table .............. */ nullptr,
                /* shortlived ............... */ false,
                /* subType .................. */ "DMParticleAPrime",
                /* anti particle encoding ... */ 500022
        );

        // Life time is given from width
        //anInstance->SetPDGLifeTime( hbar_Planck/(anInstance->GetPDGWidth()) ); // Benchmark life time

        //Here is how to setup decay table
        //create Decay Table
        //G4DecayTable* table = new G4DecayTable();

        // create a decay channel
        //G4VDecayChannel* mode;
        // ALP -> gamma + gamma
        //mode = new G4PhaseSpaceDecayChannel("DMParticleALP", 1., 2, "gamma", "gamma");
        //table->Insert(mode);
        //anInstance->SetDecayTable(table);
    }
    theInstance = reinterpret_cast<DMParticleAPrime *>(anInstance);
    return theInstance;
}
