#ifndef DarkMatter_h
#define DarkMatter_h 1

#define Mel 5.109989461E-04 // electron mass in GeV
#define Mmu 0.1056583745 // muon mass in GeV
#define alphaEW (1.0/137.0)
#define MUp 2.79 // protonMu
#define Mpr 0.938 // proton mass
#define max_uint 4294967296.0l
#define GeVtoPb 3.894E+08

struct ParamsForChi {
    double AA;
    double ZZ;
    double MMA;
    double EE0;
};

struct ParamsForMuonTotCS {
    double AA;
    double ZZ;
    double MMA;
    double EE0;
};

double chi(double t, void *pp);

double parinv(double x, double a[], double f[], int n);

class DarkMatter {
    friend class DarkPhotons;

public:

    DarkMatter(double MAIn, double EThreshIn, double SigmaNormIn = 1., double ANuclIn = 184., double ZNuclIn = 74.,
               double DensityIn = 19.25,
               double epsilIn = 0.0001, int IDecayIn = 0);

    //DarkMatter(double MAIn, double EThreshIn, double SigmaNormIn=1., double ANuclIn=207., double ZNuclIn=84., double DensityIn=11.35,
    //           double epsilIn=0.0001 );
    virtual ~DarkMatter();

    void SetSigmaNorm(double SigmaNormIn);

    void ResetNEmissions() { NEmissions = 0; } // For G4 DM classes
    void EmissionSimulated() { NEmissions++; } // For G4 DM classes; in future do it automatically in SimulateEmission
    virtual double TotalCrossSectionCalc(double E0) = 0;

    void PrepareTable();

    double GetMA() const { return MA; }

    virtual void SetMA(double MAIn) { MA = MAIn; }

    double GetEThresh() const { return EThresh; }

    double GetSigmaNorm() const { return SigmaNorm; }

    double Getepsil() const { return epsil; }

    double GetDensity() const { return Density; }

    //int GetDMType() {return DMType;}
    int GetParentPDGID() const { return ParentPDGID; }

    int GetDaughterPDGID() const { return DaughterPDGID; }

    //int Decay() {return IDecay;}
    // usage of normalization below:   Nsign = (Naccepted/Nsimulated)*Normalization*EOT
    double GetNormalization() const {
        return 3.0e-15 * (Density / 19.25) * (184. / ANucl) *
               epsil * epsil / (SigmaNorm * epsilBench * epsilBench);
    }

    double GetMeanFreePathFactor() const { return 1. / (GetNormalization()); }

    double GetSigmaTot0(double E0);

    virtual double GetSigmaTot(double E0) = 0;

    double GetSigmaMax(double E0);

    double GetSigmaAngleMax(double E0);

    virtual bool EmissionAllowed(double E0, double DensityMat); // E0 in GeV, density in g/cm3
    bool Emission(double E0, double DensityMat, double StepLength); // E0 in GeV, density in g/cm3, StepLength in mm
    virtual double CrossSectionDSDX(double XEv, double E0);

    //virtual double CrossSectionDSDX(double Xev, double E0) = 0;
    virtual double CrossSectionDSDXDU(double Xev, double UThetaEv, double E0) = 0;

    virtual double Width() = 0;

    double MaxCrossSectionCalc(double E0);

    double MaxCrossSectionAngleCalc(double E0);

    double SimulateEmission(double E0, double *angles);

    double SimulateEmissionWithAngle(double E0, double *angles);

    double GetAccumulatedProbability() const { return AccumulatedProbability; }

private:

    double MA;
    double EThresh;
    double SigmaNorm;
    double ANucl;
    double ZNucl;
    double Density;
    double epsilBench;
    double epsil;
    //int DMType; // 1 - Dark Photon; 2 - Dark Scalar; 11 - Z'; 21 - ALP
    int ParentPDGID{};
    int DaughterPDGID{};
    //int IDecay; // 1 - DM particle dacays; 2 - Force DM particle decay

    int nptable;
    double ep[15]{};
    double sigmap[15]{};
    double sigmax[15]{};
    double sigmaxa[15]{};

    double AccumulatedProbability;

    int NEmissions;
};

#endif
