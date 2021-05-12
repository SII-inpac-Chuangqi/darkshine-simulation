//................................................................................//
//CPP STL
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

//................................................................................//
//ROOT
#include "TMath.h"
#include "TString.h"
#include "TGeoManager.h"

//................................................................................//
//Framework
#include "Object/SimulatedHit.h"

//................................................................................//
//GBL

//................................................................................//
//Tracking
#include "Algo/TrkHit.h"
#include "Algo/GBLFitting.h"

//................................................................................//
//Constructor
GBLFitting::GBLFitting(const TrkHitPVec &track, std::initializer_list<double> list)
{
    auto it = list.begin();
    double preR = *it; it++;
    double By = *it;

    const int noLayer = track.size();

    std::vector<gbl::GblDetectorLayer> layers;
    for(int i = 0; i < noLayer; i++)
    {
        layers.push_back(CreateLayerSit(std::to_string(i),    //name
                                        i,                    //layer
                                        track.at(i)->GetZ(),  //x position of center, mm
                                        0.,                   //y position of center, mm
                                        0.,                   //z position of center, mm
                                        0.001,                //thickness/radiation length
                                        0.,                   //angle of u-direction in YZ plane, degree
                                        0.02,                 //resolution of u-direction in YZ plane, mm
                                        0.1*180/TMath::Pi(),  //angle of v-direction in YZ plane, degree
                                        0.02));               //resolution of v-direction in YZ plane, mm
    }

    double qbyp = 1./(0.3*By*preR); //q/p, GeV^-1
    const double bfac = 0.003*By;   //c*B, T

    std::vector<Eigen::Vector2d> hits;
    std::vector<gbl::GblPoint>   listOfPoints;

    const double cosLambdaSeed = 1.;
    for(int i = 0; i < noLayer; i++)
    {
        double sArc;
        double sOld;
        Eigen::Vector2d res();

        gbl::Matrix5d jacPointToPoint = gbl::gblSimpleJacobian((sArc - sOld)/cosLambdaSeed, cosLambdaSeed, bfac);
        gbl::GblPoint point(jacPointToPoint);

        
        //Eigen::Matrix2d proM2l = uvDir * mDirT;
        //Eigen::Matrix2d proL2m = proM2l.inverse();       

        double radlen = layers.at(i).getRadiationLength();
        double errMs = gbl::gblMultipleScatteringError(qbyp, radlen);
        if (errMs > 0.)
        {
            Eigen::Vector2d scat(0., 0.);
            Eigen::Vector2d scatPrec(1./(errMs*errMs), 1./(errMs*errMs)); // scattering precision matrix is diagonal in curvilinear system
            point.addScatterer(scat, scatPrec);
        }

        listOfPoints.push_back(point);
    }

    gbl::GblTrajectory traj(listOfPoints, bfac != 0.);
    int Ndf;
    double lostWeight;
    unsigned int ierr = traj.fit(fChi2, Ndf, lostWeight);
}

//................................................................................//
//Processor
//................................................................................//
//Initialize the fitter, set up magnetic, material manager, track representation, fitter and track model
void GBLFitting::Init(const TrkHitPVec &track, std::initializer_list<double> list)
{
}

//................................................................................//
//Do the fit
void GBLFitting::Fit(const TrkHitPVec &track, std::initializer_list<double> list)
{
}

//................................................................................//
//Fill results
void GBLFitting::Fill(const TrkHitPVec &track, std::initializer_list<double> list)
{
}

//................................................................................//
//Get
//................................................................................//



gbl::GblDetectorLayer GBLFitting::CreateLayerSit(const std::string aName, unsigned int layer,
                                                 double xPos, double yPos, double zPos, double thickness, double uAngle,
                                                 double uRes, double vAngle, double vRes)
{
    Eigen::Vector3d aCenter(xPos, yPos, zPos);
    Eigen::Vector2d aResolution(uRes, vRes);
    Eigen::Vector2d aPrecision(1./(uRes*uRes), 1./(vRes*vRes));
    Eigen::Matrix3d measTrafo;
    const double cosU = cos(uAngle/180.*M_PI);
    const double sinU = sin(uAngle/180.*M_PI);
    const double cosV = cos(vAngle/180.*M_PI);
    const double sinV = sin(vAngle/180.*M_PI);
    measTrafo << 0., cosU, sinU, 0., cosV, sinV, 1., 0., 0.; // U(u1, u2, u3), V(v1, v2, v3), N(n1, n2, n3)
    Eigen::Matrix3d alignTrafo;
    alignTrafo << 0., 1., 0., 0., 0., 1., 1., 0., 0.; // Y(y1, y2, y3), Z( z1, z2, z3), X( x1, x2, x3)
    return gbl::GblDetectorLayer(aName, layer, 2, thickness, aCenter, aResolution,
                                 aPrecision, measTrafo, alignTrafo);
}
