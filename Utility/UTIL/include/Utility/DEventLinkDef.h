//
// Created by Zhang Yulei on 9/24/20.
//

#ifndef DSIMU_DEVENTLINKDEF_H
#define DSIMU_DEVENTLINKDEF_H

#include <memory>
#include <vector>
#include <map>
#include <string>

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ class DStep+;
#pragma link C++ class std::vector<DStep*>+;

#pragma link C++ class DHit+;
#pragma link C++ class std::vector<DHit*>+;

#pragma link C++ class DParticle+;
#pragma link C++ class std::vector<DParticle*>+;

#pragma link C++ class CalorimeterHit+;
#pragma link C++ class std::vector<CalorimeterHit*>+;

#pragma link C++ class SimulatedHit+;
#pragma link C++ class std::vector<SimulatedHit*>+;

#pragma link C++ class McParticle+;
#pragma link C++ class std::vector<McParticle*>+;

#pragma link C++ class ReconstructedParticle+;
#pragma link C++ class std::vector<ReconstructedParticle*>+;

#pragma link C++ class DEvent+;
#pragma link C++ class EventReader_D+;
#pragma link C++ class DSMagneticField+;
#pragma link C++ class EventDisplay+;
#endif

#endif //DSIMU_DEVENTLINKDEF_H
