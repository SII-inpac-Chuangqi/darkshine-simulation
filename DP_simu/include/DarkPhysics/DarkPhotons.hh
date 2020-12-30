#ifndef DarkPhotons_h
#define DarkPhotons_h 1

#include "DarkMatter.hh"

class DarkPhotons : public DarkMatter {

public:

    DarkPhotons(double MAIn, double EThreshIn, double SigmaNormIn = 1., double ANuclIn = 184., double ZNuclIn = 74.,
                double DensityIn = 19.25,
                double epsilIn = 0.00001, int IDecayIn = 0);

    virtual ~DarkPhotons();

    double TotalCrossSectionCalc(double E0);
    double GetSigmaTot(double E0) override;

    double CrossSectionDSDX(double Xev, double E0) override;

    double CrossSectionDSDXDU(double Xev, double UThetaEv, double E0) override;

    double Width() override;

private:

};

#endif
