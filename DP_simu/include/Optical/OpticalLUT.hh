/** @file  OpticalLUT.hh
 
 A singleton class for reading in a lookup table of Cerenkov photon detection
 efficiencies and transit times from a file. Adapted from g2 experiment codes.
 

 @author Thomas Rueter
 @date 2013

 @author David Sweigart
 @date 2015
 */

#ifndef OpticalLUT_hh
#define OpticalLUT_hh

#include <string>
#include <vector>

#include "G4SystemOfUnits.hh"

// #define UNITID "ECAL_cube_2.5_2.5_2_v1"

class OpticalLUT {
public:
  OpticalLUT();  // only public access is via getInstance()
  ~OpticalLUT(); // include a destructor as well
  // OpticalLUT& getInstance();
  // bool validateLUT(double val_MaxDepth) {return (val_MaxDepth==xtalDepth);};
  
  void initialize(std::string,std::string unitID); //default "ECAL_cube_2.5_2.5_2_v1"

  double efficiency(double, double);  
  double transitTime(double, double, double);
  double getFactor();

  // double GetMaxDepth() {return xtalDepth;};
  bool isReady() {return isInit;};
  void Print();
  
private:
  // // the singleton instance
  // static OpticalLUT* instance_; // to be returned by getInstance()

  int numDepthBins;
  int numCosThetaBins;
  int numDistBins;
  int maxDistTime;

  double xtalDepth;
  double factor; //use to normalize the maxEfficiency to 1
//  double weightFactor; //used to reweight, like sample 1/10 but assign 10 weight
//  //then how about let weight = efficiency???
//??? weight of hit???

  std::vector<std::vector<double> > efficiencyTable;
  std::vector<std::vector<std::vector<double> > > transitTable;

 
  // helper functions
  int getDepthIndex(double);
  int getCosThetaIndex(double);

  bool isInit;
  // std::string unitType; //specify the LUT unit, like "ECAL_Center_LYSO"

  std::string fPath; //LUT file path, saved for debug.
  char funitID[513]="";
};

#endif // OpticalLUT_hh
