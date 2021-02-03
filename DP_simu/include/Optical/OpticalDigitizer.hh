#ifndef OpticalDigitizer_h
#define OpticalDigitizer_h

// ROOT
#include <TFile.h>
#include <TSpline.h> // for pixel response shape input

#include <vector>
#include <numeric>

// CLHEP includes
//#include "CLHEP/Random/RandFlat.h"
//#include "CLHEP/Random/RandGauss.h"
//#include "CLHEP/Random/RanecuEngine.h"
#include "Randomize.hh"
//#include "CLHEP/Random/RandomEngine.h"

#include "OpticalHit.hh"
#include "Object/DEvent.h"
#include "Control/Control.h"

class DigiForm;

class OpticalDigitizer {
public:
    explicit OpticalDigitizer(std::string cIn = "", int DiGi_No = 0);

    ~OpticalDigitizer();

    //    Long64_t digilize(int No, std::vector<int> *calIDs, std::vector<double> *arrTimes);
    //    Bool_t save();
    // void addPixelResponseInc(double photonArrivalTime,std::map<int, double> &pixelHitMap,std::vector<double> &waveform);
    std::pair<int, int> addPixelResponse(const std::vector<OpticalHit *> &hits, int cId, std::vector<double> *waveform,
                                         double timeZero = 0);

    void applyRiderBehavior(std::vector<double> *waveform);

    void SetWaveformInitial(std::vector<double> *wf) const {
        wf->clear();
        wf->insert(wf->end(), nClockTicks_, 0.);
    };

    bool Digitize(DigiForm *DiGi, int cId);

    int DigitizeAll();

    int AddHits(std::vector<OpticalHit *>* hits, int cId);

    DigiForm *GetDiGi(int cId); //initilize or return

    [[nodiscard]] const std::vector<DigiForm *> &GetDiGis() const {
        return fDiGis;
    }

    void ResetPixelHistory() {
        fpixelHitMaps.clear();
    }; //call at event end to clear pixel history.
    void ResetPixelHistoryUnit(int cId) {
        fpixelHitMaps.erase(cId);
    }; //Later will implement pileup use this.
    void ClearHits() {
        for (auto &item : fOpticalHits) {
            for (auto h : item.second)
                delete h;
            (item.second).clear();
        }
        fOpticalHits.clear();
    };

    void ClearDigits() {
        auto dig_size = fDiGis.size();
        for (auto d : fDiGis) {
            delete d;
        }
        fDiGis.clear();

        for (unsigned i = 0; i < dig_size; ++i) {
            fDiGis.push_back(new DigiForm());
        }
    }

    void ClearHitsUnit(int cId) {
        if (fOpticalHits.find(cId) == fOpticalHits.end())
            return;
        for (auto h : fOpticalHits.at(cId))
            delete h;
        fOpticalHits.at(cId).clear();
        fOpticalHits.erase(cId);
    };

    void Print();

private:
    // main functions
    //    void addPixelResponse(const vector<double> &photonArrivalTimes,
    //                          vector<double> &waveform);
    //    void applyRiderBehavior(std::vector<double> &waveform);
    // helper functions
    double getResponse(double delta_t);

    /////////////////////////////////////////////////////////////////////////   below can be calclulated
    int nClockTicks_; // number of clock ticks in waveform
    // SiPM pixel response parameters
    TSpline3 *pulseShapeSpline_;
    double pulsePolarity_;
    double maxPulseTime_;
    double minPulseTime_;
    int nPulseClockTicks_; // number of clock ticks in pulse shape template

    // We save the pixel History here...
    //it is in the stack?
    std::string fcIn;
    std::map<int, std::map<int, double>> fpixelHitMaps;
    std::map<int, std::vector<OpticalHit *>> fOpticalHits; //also involke clear here!
    std::vector<DigiForm *> fDiGis; //use to temporarily save ptr to DiGi. Then fill in FillSim
};

#endif

//random generator //Now change to global/static generator
//better use g2 style "create engine" to have MT independent engine
//    CLHEP::HepRandomEngine& createEngine(long seed=123456){
//        std::shared_ptr<CLHEP::HepRandomEngine> eptr=std::make_shared<CLHEP::RanecuEngine>();
//        eptr->setSeed(seed,0);
//        return *eptr;
//    }
//    const long seed_ = 12345;
// CLHEP::HepRandomEngine& engine_;
//    CLHEP::RanecuEngine engine_;
//    G4RandFlat flatDist_(CLHEP::HepRandom::getTheEngine());
//    G4RandGauss gaussDist_(CLHEP::HepRandom::getTheEngine());