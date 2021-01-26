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

class OpticalDigitizer
{
public:
    OpticalDigitizer(std::string cIn="", int DiGi_No=0);
    ~OpticalDigitizer();
    //    Long64_t digilize(int No, std::vector<int> *calIDs, std::vector<double> *arrTimes);
    //    Bool_t save();
    // void addPixelResponseInc(double photonArrivalTime,std::map<int, double> &pixelHitMap,std::vector<double> &waveform);
    std::pair<int, int> addPixelResponse(std::vector<OpticalHit *> hits, int cId, std::vector<double> &waveform, double timeZero = 0);
    void applyRiderBehavior(std::vector<double> &waveform);
    void SetWaveformInitial(std::vector<double>& wf) { wf.clear(); wf.insert(wf.end(),nClockTicks_, 0.);};
    bool Digitize(DigiForm *DiGi, int cId);
    int DigitizeAll();
    int AddHits(std::vector<OpticalHit *>& hits, int cId);
    DigiForm* GetDiGi(int cId); //initilize or return
    const std::vector<DigiForm *> GetDiGis() {return fDiGis;};
    void ResetPixelHistory()
    {
        fpixelHitMaps.clear();
    }; //call at event end to clear pixel history.
    void ResetPixelHistoryUnit(int cId)
    {
        fpixelHitMaps.erase(cId);
    }; //Later will implement pileup use this.
    void ClearHits()
    {
        for (auto &item : fOpticalHits)
            for (auto h : item.second)
                delete h;
        fOpticalHits.clear();
    };
    void ClearHitsUnit(int cId)
    {
        if (fOpticalHits.find(cId) == fOpticalHits.end())
            return;
        for (auto h : fOpticalHits[cId])
            delete h;
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

    ///////////////////////////////////////////////////////////////////////// below can be set: default setup
    // general parameters
    // const int verboseLevel_ = 0; // controls amount of screen printout
    // const Bool_t debug = false;
    // Digitizer Configuration Versioning:
    // const DigiScheme _DigiScheme = SIPM_g2_v1_20210124;
   //now move default to dControl
   /*
    // process options
 
    //    const bool digitize_ = false;            // "false" will sample waveform without digitizing; now save to two form
    // bool usePositivePolarity_ = false; // "false" will use negative polarity
    // bool addClockJitter_ = true;       // "false" will not clock jitters
    // bool injectNoise_ = true;          // "false" will not add digitization noise

    // hardware parameters: global
    double sampleInterval_ = 1.25; // [ns]
    double maxTime_ = 100.;        // [ns]

    // hadrware parameters:SIPM response
    // 16 channels, 3600 pixels per channel
    int nPixels = 16 * 3600 - 1;       // counting from zero
    double pixelRecoveryTau_ = 50.;    // [ns]
    double pixelRecoveryCutoff_ = 10.; // [sigma]
    // hardware parameters: SIPM pulse
    G4String pulseFilePath = "pulseShape.root";
    G4String splineName = "pulseShapeSpline";
    double pulseTimeZero_ = -5.; // appropriate t0 value?

    // hadrware parameters:ADC and FE (now we only save the analog waveform in voltage.)
    //digitized later in DEvent
    // int nBits = 4096; // 2^12 ADC bits
    // double voltageToSample = (nBits / 1000./ 1);  // sample/voltage conversion [sample/mV]
    // ADC input range, for TI ADS5401 configured for two's complement
    // int range_min = -2047; // -(2^12 - 1)
    // int range_max = 2048;  // +(2^12)
    // int pedestalLevel_ = 1750;          // [ADC count]
    // int pedestalLevel_ = 0;          // [ADC count]
    int noiseSigma_ = 4;                  // [ADC count]
    double clockJitterSigma_ = 0.05;      // [ns]
    double apertureJitterSigma_ = 0.0001; // [ns]
    // scaling factor to allow for two overlapped 3.1 GeV positrons to be in ADC's
    // range
    //    const double pulseScaleFactor = (nBits / 3160.); // found from testBeamCal
    double pulseScaleFactor = 1; //dummy, can be setted later in DEvent or here.
*/
    /////////////////////////////////////////////////////////////////////////   below can be calclulated
    int nClockTicks_ ; // number of clock ticks in waveform
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