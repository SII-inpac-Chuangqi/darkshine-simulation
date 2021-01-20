//
// Created by Zhang Yulei on 9/24/20.
//

#ifndef DISPLAY_LINKDEF_H
#define DISPLAY_LINKDEF_H

#include <memory>
#include <vector>
#include <map>
#include <string>

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ class EventReader_D+;
#pragma link C++ class DSMagneticField+;
#pragma link C++ class CaloHitsDisplay+;
#pragma link C++ class DEventDisplay+;

#endif

#endif //DISPLAY_LINKDEF_H
