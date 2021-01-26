//
// Created by Zhang Yulei on 1/4/21.
//

#include "Object/DigiForm.hh"


DigiForm::~DigiForm() {
    fWaveform.clear();
    fTimeSeq.clear();

    // for (auto hit : fRawOpticalHits) delete hit;
    // fRawOpticalHits.clear();
}

// int DigiForm::getFadcMax() const {
//     return fADCMax;
// }

// void DigiForm::setFadcMax(int fAdcMax) {
//     fADCMax = fAdcMax;
// }

// double DigiForm::getFAnalogMax() const {
//     return fAnalogMax;
// }

// void DigiForm::setFAnalogMax(double fAnalogmax) {
//     DigiForm::fAnalogMax = fAnalogmax;
// }

// int DigiForm::getFSamplePoints() const {
//     return fSamplePoints;
// }

// void DigiForm::setFSamplePoints(int fSamplepoints) {
//     DigiForm::fSamplePoints = fSamplepoints;
// }

// int DigiForm::getFSampleTime() const {
//     return fSampleTime;
// }

// void DigiForm::setFSampleTime(int fSampletime) {
//     DigiForm::fSampleTime = fSampletime;
// }

// int DigiForm::getFDigiScheme() const {
//     return fDigiScheme;
// }

// void DigiForm::setFDigiScheme(int fDigischeme) {
//     DigiForm::fDigiScheme = fDigischeme;
// }

// int DigiForm::getFSampleLength() const {
//     return fSampleLength;
// }

// void DigiForm::setFSampleLength(int fSamplelength) {
//     DigiForm::fSampleLength = fSamplelength;
// }

// int DigiForm::getFDetId() const {
//     return fDetID;
// }

// void DigiForm::setFDetId(int fDetId) {
//     fDetID = fDetId;
// }

// int DigiForm::getFDetType() const {
//     return fDetType;
// }

// void DigiForm::setFDetType(int fDettype) {
//     DigiForm::fDetType = fDettype;
// }

// int DigiForm::getFCellIdX() const {
//     return fCellID_X;
// }

// void DigiForm::setFCellIdX(int fCellIdX) {
//     fCellID_X = fCellIdX;
// }

// int DigiForm::getFCellIdY() const {
//     return fCellID_Y;
// }

// void DigiForm::setFCellIdY(int fCellIdY) {
//     fCellID_Y = fCellIdY;
// }

// int DigiForm::getFCellIdZ() const {
//     return fCellID_Z;
// }

// void DigiForm::setFCellIdZ(int fCellIdZ) {
//     fCellID_Z = fCellIdZ;
// }

// const std::vector<int> &DigiForm::getFWaveform() const {
//     return fWaveform;
// }

// void DigiForm::setFWaveform(const std::vector<int> &fWaveForm) {
//     DigiForm::fWaveform = fWaveForm;
// }

// double DigiForm::getFStartTime() const {
//     return fStartTime;
// }

// void DigiForm::setFStartTime(double fStarttime) {
//     DigiForm::fStartTime = fStarttime;
// }

// double DigiForm::getFDetectedEnergy() const {
//     return fDetectedEnergy;
// }

// void DigiForm::setFDetectedEnergy(double fDetectedenergy) {
//     DigiForm::fDetectedEnergy = fDetectedenergy;
// }

// double DigiForm::getFTruthEnergy() const {
//     return fTruthEnergy;
// }

// void DigiForm::setFTruthEnergy(double fTruthenergy) {
//     DigiForm::fTruthEnergy = fTruthenergy;
// }

// bool DigiForm::isFIsOverflow() const {
//     return fIsOverflow;
// }

// void DigiForm::setFIsOverflow(bool fIsoverflow) {
//     DigiForm::fIsOverflow = fIsoverflow;
// }

// bool DigiForm::isFIsUnderflow() const {
//     return fIsUnderflow;
// }

// void DigiForm::setFIsUnderflow(bool fIsunderflow) {
//     DigiForm::fIsUnderflow = fIsunderflow;
// }

// bool DigiForm::isFIsOutOfTime() const {
//     return fIsOutOfTime;
// }

// void DigiForm::setFIsOutOfTime(bool fIsOutofTime) {
//     DigiForm::fIsOutOfTime = fIsOutofTime;
// }

// const std::vector<OpticalHit *> &DigiForm::getFRawOpticalHits() const {
//     return fRawOpticalHits;
// }

// void DigiForm::setFRawOpticalHits(const std::vector<OpticalHit *> &frawOpticalHits) {
//     DigiForm::fRawOpticalHits = frawOpticalHits;
// }

std::vector<int> DigiForm::GetDigiForm()
{
    auto i = fWaveform.size();
    auto ret = std::vector<int>(i, fPedestal);
    i = 0;
    for (auto &sampleValue : fWaveform)
    {
        int dValue = round(sampleValue / fVoltageToADC);
        if (dValue < fRangeMin)
        {
            // under-range value found
            dValue = fRangeMin;
        }
        else if (dValue > fRangeMax)
        {
            // over-range value found
            dValue = fRangeMax;
        }
        ret[i] += dValue;
        i++;
    }
    return ret;
}

double DigiForm::GetIntegral(bool inADC)
{
    double ret = 0.;
    if (inADC)
    {
        for (auto &v : GetDigiForm())
            ret += v;
    }
    else
    {
        for (auto &v : fWaveform)
            ret += v;
    }
    return ret * fNsPerTick;
}

void DigiForm::AddTimeSeq(double arrivalT)
{
    double rT = arrivalT - fTimeSeqZero;
    auto _TimeBase = GetTimeSeqBase(true); //with PU effect
    if (fTimeSeq.size() != _TimeBase.size())
        fTimeSeq = std::vector<int>(_TimeBase.size(), 0);
    auto const itl = std::lower_bound(_TimeBase.begin(), _TimeBase.end(), rT);
    fTimeSeq[std::distance(_TimeBase.begin(), itl)]++;
}

int DigiForm::GetDigiFormVersion()
{
    return _OpticalDigiform_Version;
}

const std::vector<double> DigiForm::GetTimeSeqBase(bool withPU)
{
    auto ret = std::vector<double>(_OpticalDigiform_TimeSeqBase);
    if (withPU)
        return ret;
    else
        return std::vector<double>(ret.begin() + 1, ret.end() - 1);
}

int DigiForm::GetOpticalNo(bool isGen)
{
    if (isGen)
    {
        return fOpticalGen;
    }
    else
    {
        int No = 0;
        for (auto c : fTimeSeq)
            No += c;
        return No;
    }
}
