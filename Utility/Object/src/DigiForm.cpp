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
    {
        auto temp = std::vector<int>(_TimeBase.size(), 0);
        fTimeSeq.assign(temp.begin(), temp.end());
    }
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
