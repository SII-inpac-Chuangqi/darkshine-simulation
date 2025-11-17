#include "DarkPhysics/DMParticleAPrime.hh"

#include "G4ParticleTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhaseSpaceDecayChannel.hh"
#include "G4DalitzDecayChannel.hh"
#include "G4DecayTable.hh"

#include "Control/Control.h"

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
    G4bool stable_label = true;
    if (dControl->visible_decay) stable_label= false;
    if (!anInstance) {
        anInstance = new G4ParticleDefinition(
                /* Name ..................... */ name,
                // /* Mass ..................... */ 16.7 * MeV,
                /* Mass ..................... */ dControl->signal_mass,
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
                /* stable ................... */ stable_label,
                /* lifetime.................. */ 0,
                /* decay table .............. */ nullptr,
                /* shortlived ............... */ false,
                /* subType .................. */ "DMParticleAPrime",
                /* anti particle encoding ... */ 500022
        );

        //inset for visible decay 
        if (dControl->visible_decay){
            G4double mA = anInstance->GetPDGMass();
            G4double eplsion= dControl->dp_epsilon;
            G4double width = 1;

            if (dControl->dp_decay_channel == "ee" && mA>2*0.511) width=  1./3. * 1./137. * eplsion * eplsion * mA * sqrt(1-4*0.511*0.511/mA/mA) * (1+2*0.511*0.511/mA/mA);
            if (dControl->dp_decay_channel == "mumu" && mA>2*105.66) width= 1./3. * 1./137. *eplsion * eplsion * mA * sqrt(1-4*105.66*105.66/mA/mA) * (1+2*105.66*105.66/mA/mA) ;


            //G4double widthToEe = mA>2*0.511? 1./3. * 1./137. * 1e-8 * mA * sqrt(1-4*0.511*0.511/mA/mA) * (1+2*0.511*0.511/mA/mA) : 0;
            //G4double widthToMm = mA>2*105.66? 1./3. * 1./137. * 1e-8 * mA * sqrt(1-4*105.66*105.66/mA/mA) * (1+2*105.66*105.66/mA/mA) : 0;
            //G4double totalWidth = widthToEe + widthToMm;
            anInstance->SetPDGLifeTime( hbar_Planck/width );
            std::cout<<"Inject Decay Width:" << width <<std::endl;

            G4DecayTable* table = new G4DecayTable();
        
            G4VDecayChannel *mode = nullptr; // assume onle one channel
            if (dControl->dp_decay_channel == "mumu" && mA>2*105.66) mode = new G4PhaseSpaceDecayChannel("DMParticle", 1, 2, "mu-", "mu+");
            if (dControl->dp_decay_channel == "ee" && mA>2*0.511) mode = new G4PhaseSpaceDecayChannel("DMParticle", 1, 2, "e-", "e+");
            //mode1 = new G4PhaseSpaceDecayChannel("DMParticle", widthToEe/totalWidth, 2, "e-", "e+");
            //mode2 = new G4PhaseSpaceDecayChannel("DMParticle", widthToMm/totalWidth, 2, "mu-", "mu+");
            table->Insert(mode);
            //table->Insert(mode2);
            anInstance->SetDecayTable(table);
        }
        //Here is how to setup decay table
        //create Decay Table
        //G4DecayTable* table = new G4DecayTable();

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
