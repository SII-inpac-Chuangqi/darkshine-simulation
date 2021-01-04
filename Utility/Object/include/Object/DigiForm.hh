
/// \file DigiForm.hh
/// \brief Definition of the DigiForm class, after waveform digilization, 
/// \brief all hits in one cell be formed to a fixed length waveform(fixed-legth vector). Per events

#ifndef DigiForm_h
#define DigiForm_h 1

#include "TObject.h"

#include "Object/OpticalHit.hh"

/// DigiForm class
///
/// It defines the DigiForm, 
/// Which save the waveform after digitization, and the information extracted. (Along with digitization parameter)

class DigiForm : public TObject {
public:
    DigiForm() = default;

    ~DigiForm() override;

    int getFadcMax() const;

    void setFadcMax(int fAdcMax);

    double getFAnalogMax() const;

    void setFAnalogMax(double fAnalogmax);

    int getFSamplePoints() const;

    void setFSamplePoints(int fSamplepoints);

    int getFSampleTime() const;

    void setFSampleTime(int fSampletime);

    int getFDigiScheme() const;

    void setFDigiScheme(int fDigischeme);

    int getFSampleLength() const;

    void setFSampleLength(int fSampleLength);

    int getFDetId() const;

    void setFDetId(int fDetId);

    int getFDetType() const;

    void setFDetType(int fDetType);

    int getFCellIdX() const;

    void setFCellIdX(int fCellIdX);

    int getFCellIdY() const;

    void setFCellIdY(int fCellIdY);

    int getFCellIdZ() const;

    void setFCellIdZ(int fCellIdZ);

    const std::vector<int> &getFWaveform() const;

    void setFWaveform(const std::vector<int> &fWaveform);

    double getFStartTime() const;

    void setFStartTime(double fStartTime);

    double getFDetectedEnergy() const;

    void setFDetectedEnergy(double fDetectedEnergy);

    double getFTruthEnergy() const;

    void setFTruthEnergy(double fTruthEnergy);

    bool isFIsOverflow() const;

    void setFIsOverflow(bool fIsOverflow);

    bool isFIsUnderflow() const;

    void setFIsUnderflow(bool fIsUnderflow);

    bool isFIsOutOfTime() const;

    void setFIsOutOfTime(bool fIsOutOfTime);

    const std::vector<OpticalHit *> &getFRawOpticalHits() const;

    void setFRawOpticalHits(const std::vector<OpticalHit *> &fRawOpticalHits);

private:

    // Digitization parameter
    int fADCMax{}; //like 1024=10bit ADC
    double fAnalogMax{}; //like 1024ADC = 5.0V
    int fSamplePoints{}; //like 1000
    int fSampleTime{}; //in ns
    int fDigiScheme{}; //digitization scheme versioning, enum.
    int fSampleLength{};

    // Location information
    int fDetID{}; // 1-D No.
    int fDetType{}; // Optical module type or version, like ECAL_Z_v1, HCAL_XS_v2, ...
    int fCellID_X{}; // 3-D No., or we may store real hit position
    int fCellID_Y{};
    int fCellID_Z{};

    // waveform information
    std::vector<int> fWaveform; //fixed-length int vector
    std::vector<OpticalHit*> fRawOpticalHits;

    double fStartTime{};
    double fDetectedEnergy{}; //effective energy after conversion, that is really detected
    double fTruthEnergy{}; //truth energy which bypass the digitization. But still account for the optical process loss.
    bool fIsOverflow{}; // overflow some bin >max ADC
    bool fIsUnderflow{}; // underflow some bin <1ADC
    bool fIsOutOfTime{}; //Out of time window (too slow git)

ClassDefOverride(DigiForm,1);
};
#endif
