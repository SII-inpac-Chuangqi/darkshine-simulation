/** @file  OpticalLUT.cc
 
 A singleton class for reading in a lookup table of Cerenkov photon detection
 efficiencies and cumulative distribution functino of transit times from a
 binary file. Adapted from g2 exp. codes.
 
 @author Thomas Rueter
 @date 2013

 @author David Sweigart
 @date 2015
 */

#include "Optical/OpticalLUT.hh"

// #include "cetlib/exception.h"
// #include "artg4/util/util.hh"
// #include "gm2geom/calo/CalorimeterGeometry.hh"

#include <iostream>
#include <string.h>
#include <fstream>
#include <istream>
#include <cstdlib>
#include <stdio.h>
#include <vector>


// OpticalLUT *OpticalLUT::instance_ = nullptr;

// int OpticalLUT::numDepthBins;
// int OpticalLUT::numCosThetaBins;
// int OpticalLUT::numDistBins;
// int OpticalLUT::maxDistTime;

// double OpticalLUT::xtalDepth;
// double OpticalLUT::factor;

// std::vector<std::vector<double> > OpticalLUT::efficiencyTable;
// std::vector<std::vector<std::vector<double> > > OpticalLUT::transitTable;

OpticalLUT::OpticalLUT() : numDepthBins(-1001),
                           numCosThetaBins(-1001),
                           numDistBins(-1001),
                           maxDistTime(-1001),
                           xtalDepth(-999),
                           factor(-1001),
                           isInit(false)
// efficiencyTable(),
// transitTable()
{
  ;
}

OpticalLUT::~OpticalLUT() {}

//move to LUTMgr since we may have multi LUT for different unit at same time
// OpticalLUT& OpticalLUT::getInstance() {
//   if ( instance_ == nullptr ) {
//     instance_ = new OpticalLUT();
//     // std::string basePath = artg4::basePath("GM2RINGSIM_DIR", "gm2ringsim");
//     instance_->initialize("./CerenkovLUT.dat"); //we need a method to pass in   filepath! //TODO
//   }
//   return *instance_;
// }
void OpticalLUT::Print(){
    std::cout<<"LUT parameter: "<<std::endl;
    std::cout<<"    numDepthBins: "<<numDepthBins<<std::endl;
    std::cout<<"    numCosThetaBins: "<<numCosThetaBins<<std::endl;
    std::cout<<"    numDistBins: "<<numDistBins<<std::endl;
    std::cout<<"    maxDistTime: "<<maxDistTime<<std::endl;
    std::cout<<"    xtalDepth: "<<xtalDepth<<std::endl;
    std::cout<<"    factor: "<<factor<<std::endl;
    std::cout<<"    isInit: "<<isInit<<std::endl;
    std::cout<<"    fPath: "<<fPath<<std::endl;
    std::cout<<"    funitID: "<<funitID<<std::endl;
}


void OpticalLUT::initialize(std::string filename, std::string unitID)
{
  // open the file given by the stream 'filename'
  std::ifstream infile;
  infile.open(filename, std::ios::in | std::ios::binary);

  // verify lookup table file opened successfully
  if (!infile.is_open())
  {
    //   throw cet::exception("XtalLookup") << "Could not open calorimeter photon lookup table. Ending program!\n";
    std::cerr << "Lookup Table Load failed! : " << filename << std::endl;
    return;
  }

  // binary file lookup table

  // ==========================
  // read in header information
  // ==========================

  infile.read((char *)&numDepthBins, sizeof(int));    //see simulation histo setting, can be finer
  infile.read((char *)&numCosThetaBins, sizeof(int)); //
  infile.read((char *)&numDistBins, sizeof(int));     //

  infile.read((char *)&maxDistTime, sizeof(int));     //5ns fixed for v1
  infile.read((char *)&xtalDepth, sizeof(double));    // 25mm fixed for v1
  infile.read((char *) funitID, sizeof(char) * 512);   // unitID for check, now v1

  //   // create the calorimeter geometry, which initializes itself from the parameter set
  //   gm2geom::CalorimeterGeometry caloGeom("calorimeter");

  // // We need a method to load in the geometry!! for check and transformation!!
  // // TODO
  // double GeoDepth=100.;
  //   // verify xtalDepth value matches calorimeter.fcl
  //   if ( xtalDepth != caloGeom.xtalDepth ) {
  //     throw cet::exception("OpticalLUT") << "Mismatch of xtal depth between LUT and calorimeter geometry in LookupHit: " << xtalDepth << ". Ending program!\n";
  //   }

  //   // increase the xtalDepth to account for full calorimeter bound
  //   xtalDepth += (caloGeom.wrappingGap + caloGeom.diffuserDepth + caloGeom.wrappingGap + caloGeom.opticalCouplingDepth + caloGeom.photodetectorDepth);

  // //Now do a dummy:
  // // TODO !
  // xtalDepth  =  36 * cm + 35 * 0.3 * mm;

  //Now we use veriftID to check whether the LUT suits for the unit.
  //e.g. ID = ECAL_cube_2.5_2.5_2_v1
  // validate
  if (0 != strcmp(unitID.c_str(), funitID))
  {
    std::cerr << "Lookup Table validate error! Please check you use correct LUT " << filename << std::endl;
    std::cerr << "LUT unitID=" << funitID << "\t But should be " << unitID << " as specify by DSimu" << std::endl;
    return;
  }

  // ====================
  // read in lookup table
  // ====================

  double content;
  double maxEfficiency = 0.;
  for (int i = 0; i <= numDepthBins+1; ++i) //y, from underflow to overflow
  {

    std::vector<double> eff_data;
    std::vector<std::vector<double>> cdf_data;

    for (int j = 0; j <= numCosThetaBins+1; ++j) //x
    {

      // read in efficiency
      infile.read((char *)&content, sizeof(double));
      eff_data.push_back(content);
      if (maxEfficiency < content)
        maxEfficiency = content;

      // read in distributions
      std::vector<double> dist;
      for (int k = 0; k < numDistBins; ++k) //z
      {
        infile.read((char *)&content, sizeof(double));
        dist.push_back(content);
      }
      cdf_data.push_back(dist);
    }

    efficiencyTable.push_back(eff_data);
    transitTable.push_back(cdf_data);
  }

  // calculate efficiency factor, for speedup, and add
  // small overhead in factor to avoid rounding issues
  // add 1 percent overhead to avoid rounding issues that might cause >1 effeciencies

  // factor = 1. / (1.01*maxEfficiency);
  factor = 0.99 / maxEfficiency;

  // apply factor to efficiencies
  for (int i = 0; i <= numDepthBins+1; ++i) //y
  {
    for (int j = 0; j <= numCosThetaBins+1; ++j) //x
    {
      efficiencyTable.at(i).at(j) *= factor;
    }
  }

  infile.close();
  isInit = true;
  fPath = filename;
}

double OpticalLUT::efficiency(double depth, double cosTheta)
{
  if (!isInit)
    return -1;

  int depthIndex = OpticalLUT::getDepthIndex(depth);
  int cosThetaIndex = OpticalLUT::getCosThetaIndex(cosTheta);

  double efficiency = efficiencyTable.at(depthIndex).at(cosThetaIndex);
  return efficiency;
}

double OpticalLUT::transitTime(double depth, double cosTheta, double rand)
{
  if (!isInit)
    return -1;

  // get depth, cos(theta) bin indices
  int depthIndex = OpticalLUT::getDepthIndex(depth);
  int cosThetaIndex = OpticalLUT::getCosThetaIndex(cosTheta);

  // get the corresponding cumulative distribution function
  std::vector<double> cdf = transitTable.at(depthIndex).at(cosThetaIndex);

  // -------------------------------------------------------
  // find transit time from cumulative distribution function
  // -------------------------------------------------------

  // binary search with O(log N) efficiency:
  // find largest time 't' such that P(0 < time < t) <= rand
  int low = 0, high = numDistBins - 1;
  while (low < high)
  {
    int mid = (int)((low + high) / 2);

    if (cdf.at(mid) <= rand)
    {
      low = mid + 1;
    }
    else
    {
      high = mid;
    }
  }
  //low>=high:

  // get transit time for bin index 'high', really???
  double timePerBin = double(maxDistTime) / double(numDistBins); // nanoseconds per bin index (be careful of integer division!)
  double transitTime = (high + 0.5) * timePerBin;                // shift time to bin's center

  return transitTime;
}

double OpticalLUT::getFactor()
{
  if (!isInit)
    return -1;
  return factor;
}

int OpticalLUT::getDepthIndex(double depth)//(<0),(),(),...(25,inf)
{
  if (!isInit)
    return -1;
  // get index for depth
  double binsPerDepth = numDepthBins / xtalDepth; // 0 < depth < xtalDepth, number of mesh points per 1 mm
  int depthIndex = (int)(binsPerDepth * depth)+1;   // convert a depth to a table index

  // throw exception if 'depth' value is out of range
  if (depth > xtalDepth || depth < 0)
  {
    std::cerr << "Lookup Table : Depth out of bounds in LookupHit! " << depth << ">" << xtalDepth << "LUT: " << fPath << std::endl;
    return -1;
  }

  // avoid going out of array bounds if photon generated at 'depth == xtalDepth'
   if (depth == xtalDepth) depthIndex = numDepthBins;
//  if (depthIndex >= numDepthBins)
//    depthIndex = numDepthBins - 1;

  return depthIndex;
}

int OpticalLUT::getCosThetaIndex(double cosTheta)
{
  if (!isInit)
    return -1;
  // throw exception if 'cosTheta' value is out of range
  if (cosTheta > 1 || cosTheta < -1)
  {
    // throw cet::exception("OpticalLUT") << "cosTheta out of bounds in LookupHit: " << cosTheta  << ". Ending program!\n";
    std::cerr << "Lookup Table : cosTheta out of bounds in LookupHit! abs(" << cosTheta << ")>1"
              << "LUT: " << fPath << std::endl;
    return -1;
  }

  // get index for cosTheta
  double binsPerCosTheta = numCosThetaBins / 2.;               // -1 < cosTheta < 1, number of mesh points per radian
  int cosThetaIndex = (int)(binsPerCosTheta * (cosTheta + 1))+1; // convert a cos(theta) to a table index

  // avoid going out of array bounds if 'cosTheta == 1'
  // if (cosTheta == 1) cosThetaIndex -= 1;
  if (cosTheta == 1)
    cosThetaIndex = numCosThetaBins;
  if (cosTheta == -1)
        cosThetaIndex = 1;

  return cosThetaIndex;
}
