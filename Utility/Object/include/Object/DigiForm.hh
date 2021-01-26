
/// \file DigiForm.hh
/// \brief Definition of the DigiForm class, after waveform digilization, 
/// \brief all hits in one cell be formed to a fixed length waveform(fixed-legth vector). Per events

#ifndef DigiForm_h
#define DigiForm_h 1

#include "TObject.h"
#include <math.h>
#include <iostream>

// #include "Object/OpticalHit.hh"

////hard-coded information for the digitizer
#define _OpticalDigiform_Version 2021012400 //storage format versioning
#define _OpticalDigiform_TimeSeqBase {-INFINITY, 0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.,2.1,2.2,2.3,2.4,2.5,2.6,2.7,2.8,2.9,3,5,10,20,50,100,+INFINITY}
enum DetUnitType 
{ 
  ECAL_cube_v1 = 1,  
  HCAL_XY_v1_X = 2,
  HCAL_XY_v1_Y = 3,
  //...
  UnknownDet = -1
};

enum DigiScheme{
    DigiDebug = 0,
    SIPM_g2_v1_20210124 = 1, //default digitization scheme, with g2 SIPM pulseShape
    APD_measurement_v1 = 2,
    //...
    UnknownDigi = -1
};


/// DigiForm class
///
/// It defines the DigiForm, 
/// Which save the waveform after digitization, and the information extracted. (Along with digitization parameter)

class DigiForm : public TObject {

public:
    DigiForm() = default;

    ~DigiForm() override;

    bool operator==(const DigiForm &rhs) const {
        return fDigiScheme == rhs.fDigiScheme &&
               fNsPerTick == rhs.fNsPerTick &&
               fVoltageToADC == rhs.fVoltageToADC &&
               fRangeMin == rhs.fRangeMin &&
               fRangeMax == rhs.fRangeMax &&
               fPedestal == rhs.fPedestal &&
               fDetID == rhs.fDetID &&
               fDetType == rhs.fDetType &&
               fWaveform == rhs.fWaveform &&
               fNoAheadOfTime == rhs.fNoAheadOfTime &&
               fNoOutOfTime == rhs.fNoOutOfTime &&
               fTimeSeq == rhs.fTimeSeq &&
               fTimeSeqZero == rhs.fTimeSeqZero &&
               fOpticalGen == rhs.fOpticalGen;
    }

    DigiForm &operator=(const DigiForm &rhs) {
        if (&rhs == this) { return *this; }
        TObject::operator=(rhs);
        fDigiScheme = rhs.fDigiScheme;
        fNsPerTick = rhs.fNsPerTick;
        fVoltageToADC = rhs.fVoltageToADC;
        fRangeMin = rhs.fRangeMin;
        fRangeMax = rhs.fRangeMax;
        fPedestal = rhs.fPedestal;
        fDetID = rhs.fDetID;
        fDetType = rhs.fDetType;
        fWaveform = rhs.fWaveform;
        fNoAheadOfTime = rhs.fNoAheadOfTime;
        fNoOutOfTime = rhs.fNoOutOfTime;
        fTimeSeq = rhs.fTimeSeq;
        fTimeSeqZero = rhs.fTimeSeqZero;
        fOpticalGen = rhs.fOpticalGen;
        return *this;
    }

    //main filler
    void AddTimeSeq(double arrivalT);
    bool Initilize(int DetID, DetUnitType DetType, DigiScheme DigiScheme, double NsPerTick){
        if(fDetID<0)
    {   fDetID=DetID;
        fDetType=DetType;
        fDigiScheme=DigiScheme;
        fNsPerTick=NsPerTick;
        return true;}
        else{
            std::cerr<<"DiGiForm Double Initilized!! "<<std::endl;
            return false;
        }
    };

    //complex getter
    std::vector<int> GetDigiForm();
    double GetIntegral(bool inADC=false); // mV*ns or ADC*ns
    int GetOpticalNo(bool isGen=false); //Gen means non-scaled mean value of scint optical photons
    void AddOpticalGen(int v){fOpticalGen+=v;};

    //handle used by digitizer
    std::vector<double>* GetWaveForm(){return &fWaveform;};
    void SetNo_AheadOfTime(std::pair<int,int> vs){fNoAheadOfTime=vs.first;fNoOutOfTime=vs.second;};

    //simple getter
    const std::vector<int> GetTimeSeq(bool withPU=false){return withPU?std::vector<int>(fTimeSeq.begin()+1, fTimeSeq.end()-1):fTimeSeq;};
    const std::vector<double> GetTimeSeqBase(bool withPU=false);
    int GetDigiFormVersion();
    double GetTimeTick(){return fNsPerTick;};
    unsigned int GetNticks(){return fWaveform.size();};
    double GetTimeWindow(){return fNsPerTick*fWaveform.size();};
    int GetNo_AheadOfTime(bool isSeq=false){return isSeq?fTimeSeq.front():fNoAheadOfTime;};
    int GetNo_OutOfTime(bool isSeq=false){return isSeq?fTimeSeq.back():fNoOutOfTime;};
    // DigiScheme GetDigiScheme(){return fDigiScheme;};
    DetUnitType GetDetType(){return fDetType;};
    int GetDetID(){return fDetID;};
    // int GetDetIDX(){};
    // int GetDetIDY(){};
    // int GetDetIDZ(){};


    //simple setter&getter
    void SetVoltageToADC(int v){fVoltageToADC=v;};
    int GetVoltageToADC(){return fVoltageToADC;};
    void SetRangeMin(int v){fRangeMin=v;};
    int GetRangeMin(){return fRangeMin;};
    void SetRangeMax(int v){fRangeMax=v;};
    int GetRangeMax(){return fRangeMax;};
    void SetPedestal(int v){fPedestal=v;};
    int GetPedestal(){return fPedestal;};
    // void SetTimeSeqZero(int v) //now we use global TimeSeqZero=0, later it can be trig time.
    // {
    //     if (fTimeSeqZero >= 0)
    //         std::cout << "Warning: TimeSeqZero overwrite!" << std::endl;
    //     fTimeSeqZero = v;
    // }; //can only be set once!
    double GetTimeSeqZero(){return fTimeSeqZero;};

private:

    // Digitization parameter I: needed in primary digitization
    DigiScheme fDigiScheme{UnknownDigi}; //digitization scheme tagging, used in production to mark the parameters used in digitizer
    double fNsPerTick{-1.}; //initize when create object
    //TODO: how to assemble the digitizer configuration information?? 
    // const int fMaxLength; 
    // int fADCMax{}; //like 1024=10bit ADC
    // double fAnalogMax{}; //like 1024ADC = 5.0V
    // int fSamplePoints{}; //like 1000
    // int fSampleTime{}; //in ns
    
    // int fSampleLength{};

    // Digitization parameter II: needed in calculating the final digitized waveform
    double fVoltageToADC{5000./4096}; // [mv/ADC], e.g. 2^12 ADC
    int fRangeMin{-2047};
    int fRangeMax{2048};
    int fPedestal{0};

    // Location information
    int fDetID{-1}; // 1-D No.
    DetUnitType fDetType{UnknownDet}; // Optical module type or version, like ECAL_Z_v1, HCAL_XS_v2, ...
    // int fCellID_X{}; // 3-D No., or we may store real hit position
    // int fCellID_Y{};
    // int fCellID_Z{};

    // waveform information
    std::vector<double> fWaveform; //fixed-length double vector
    double fNoAheadOfTime{-1};
    double fNoOutOfTime{-1};
    // std::vector<OpticalHit*> fRawOpticalHits; //we will not save the raw hits since impossible for storage
    
    std::vector<int> fTimeSeq; //instead, we save the time sliced result, with underflow and overflow bin at first and last
    double fTimeSeqZero{0}; //actually this is the trigger time. Now we fixed to 0, but still save this for each unit
    int fOpticalGen{0};
    // double fDetectedEnergy{}; //effective energy after conversion  // we calculate this by integral waveform
    // double fTruthEnergy{}; //truth energy which bypass the digitization. But still account for the optical process loss. // we calculate this by integral digitized waveform
    // bool fIsOverflow{}; // overflow some bin >max ADC
    // bool fIsUnderflow{}; // underflow some bin <1ADC
    // bool fIsOutOfTime{}; //Out of time window 

ClassDefOverride(DigiForm,1);
};
#endif
