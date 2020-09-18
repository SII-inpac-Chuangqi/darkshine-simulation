
#ifndef OPTICALPHYSICS_H_
#define OPTICALPHYSICS_H 1_

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"
#include "RootManager.hh"

class OpticalPhysics: public G4VPhysicsConstructor
{                      
  public:
    OpticalPhysics( RootManager* );
    virtual ~OpticalPhysics();

    virtual void ConstructParticle();
    virtual void ConstructProcess();

  private:
    RootManager* frootMng;
};

#endif
