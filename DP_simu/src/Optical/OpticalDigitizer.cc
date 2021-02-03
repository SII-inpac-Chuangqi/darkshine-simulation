#include <utility>

#include "Optical/OpticalDigitizer.hh"

OpticalDigitizer::OpticalDigitizer(std::string cIn, int DiGi_No) : pulseShapeSpline_(nullptr), fcIn(std::move(cIn)) {
    //Load parameters //TODO: add to dControl
    auto usePositivePolarity_ = dControl->Optical_usePositivePolarity;
    auto sampleInterval_ = dControl->Optical_sampleInterval;
    auto maxTime_ = dControl->Optical_maxTime;
    auto pulseFilePath = dControl->Optical_pulseFilePath;
    auto splineName = dControl->Optical_splineName;
    auto pulseTimeZero_ = dControl->Optical_pulseTimeZero;


    for (int i = 0; i < DiGi_No; ++i) {
        fDiGis.push_back(new DigiForm());
    }

    // Load shape file for single photon response
    // std::cout<<"Loading digitizer..."<<std::endl;
    TFile pulseShapeFile(pulseFilePath);
    if (!pulseShapeFile.IsOpen()) {
        std::cerr << "    SiPM pixel response shape file failed to open." << std::endl;
    }
    pulseShapeSpline_ = (TSpline3 *) pulseShapeFile.Get(splineName);
    pulseShapeFile.Close();

    // pixel response parameters
    pulsePolarity_ = usePositivePolarity_ ? 1. : -1.;
    minPulseTime_ = pulseTimeZero_;                                // use spline's min delta_t instead?
    maxPulseTime_ = pulseShapeSpline_->GetXmax() - pulseTimeZero_; //25ns about
    nPulseClockTicks_ = maxPulseTime_ / sampleInterval_;

    //overall ticks
    nClockTicks_ = maxTime_ / sampleInterval_;
}

OpticalDigitizer::~OpticalDigitizer() {
    ResetPixelHistory();
    ClearHits();
}

void OpticalDigitizer::Print() {
    std::cout << "Digitizer Loaded: cell=" << fDiGis.size() << std::endl;
    std::cout << "    usePositivePolarity_: " << dControl->Optical_usePositivePolarity << std::endl;
    std::cout << "    addClockJitter_: " << dControl->Optical_addClockJitter << std::endl;
    std::cout << "    injectNoise_: " << dControl->Optical_injectNoise << std::endl;
    std::cout << "    sampleInterval_: " << dControl->Optical_sampleInterval << std::endl;
    std::cout << "    maxTime_: " << dControl->Optical_maxTime << std::endl;
    std::cout << "    nClockTicks_: " << nClockTicks_ << std::endl;
    std::cout << "    nPixels: " << dControl->Optical_nPixels << std::endl;
    std::cout << "    pixelRecoveryTau_: " << dControl->Optical_pixelRecoveryTau << std::endl;
    std::cout << "    pixelRecoveryCutoff_: " << dControl->Optical_pixelRecoveryCutoff << std::endl;
    std::cout << "    pulseFilePath: " << dControl->Optical_pulseFilePath << std::endl;
    std::cout << "    splineName: " << dControl->Optical_splineName << std::endl;
    std::cout << "    pulseTimeZero_: " << dControl->Optical_pulseTimeZero << std::endl;
    std::cout << "    nBits: " << dControl->Optical_nBits << std::endl;
    std::cout << "    voltageToSample: " << dControl->Optical_voltageToADC() << std::endl;
    std::cout << "    range_min: " << dControl->Optical_range_min << std::endl;
    std::cout << "    range_max: " << dControl->Optical_range_max << std::endl;
    std::cout << "    pedestalLevel_: " << dControl->Optical_pedestalLevel << std::endl;
    std::cout << "    noiseSigma_: " << dControl->Optical_noiseSigma << std::endl;
    std::cout << "    clockJitterSigma_: " << dControl->Optical_clockJitterSigma << std::endl;
    std::cout << "    apertureJitterSigma_: " << dControl->Optical_apertureJitterSigma << std::endl;
    std::cout << "    pulseScaleFactor: " << dControl->Optical_pulseScaleFactor << std::endl;
    std::cout << "    pulsePolarity_: " << pulsePolarity_ << std::endl;
    std::cout << "    maxPulseTime_: " << maxPulseTime_ << std::endl;
    std::cout << "    minPulseTime_: " << minPulseTime_ << std::endl;
    std::cout << "    nPulseClockTicks_: " << nPulseClockTicks_ << std::endl;
}

// --------------
// MAIN FUNCTIONS
// --------------
DigiForm *OpticalDigitizer::GetDiGi(int cId) {

    if (!fDiGis.at(cId) || fDiGis.at(cId)->GetDetID() < 0) {
        fDiGis.at(cId)->Initilize(cId, dControl->Optical_GetDetType(fcIn), dControl->Optical_GetDigiScheme(fcIn),
                                  dControl->Optical_sampleInterval);
    }
    return fDiGis.at(cId);
}


int OpticalDigitizer::AddHits(std::vector<OpticalHit *> hits, int cId) {
    if (fOpticalHits.find(cId) == fOpticalHits.end()) {
        fOpticalHits.insert(std::pair<int, std::vector<OpticalHit *>>(cId, hits));
        //fOpticalHits.at(cId).swap(hits);
    } else {//append
        fOpticalHits.at(cId).insert(fOpticalHits.at(cId).end(), hits.begin(), hits.end());
        //hits.clear();
        //hits.shrink_to_fit();
    }
    return fOpticalHits.at(cId).size();
}

int OpticalDigitizer::DigitizeAll() {
    int valid = 0;
    unsigned counter = 0;
    for (auto *d:fDiGis) {
        if (d && d->GetDetID() >= 0) {
            valid++;
            Digitize(d, static_cast<int>(counter));
        }
        counter++;
    }
    assert(counter == fDiGis.size());
    return valid;
}

bool OpticalDigitizer::Digitize(DigiForm *DiGi, int cId) {
    if (fOpticalHits.find(cId) == fOpticalHits.end())
        return false;
    // ResetPixelHistory();
    auto waveform = DiGi->GetWaveForm();
    SetWaveformInitial(waveform);
    ResetPixelHistoryUnit(cId);
    auto ret = addPixelResponse(fOpticalHits[cId], cId, waveform);
    DiGi->SetNo_AheadOfTime(ret);
    applyRiderBehavior(waveform);
    //debug
    if (dControl->Optical_digitizerDebug) {
        // std::cout<<"TimeSeq "<<DiGi->GetTimeSeq(true)<<std::endl;
        // std::cout<<"Waveform "<<DiGi->GetWaveForm()<<std::endl;
        std::cout << "Int Wave= " << DiGi->GetIntegral(false) << std::endl;
        // std::cout<<"Digiform "<<DiGi->GetDigiForm()<<std::endl;
        std::cout << "Int Digi= " << DiGi->GetIntegral(true) << std::endl;
        std::cout << "Ahead(100ns) " << DiGi->GetNo_AheadOfTime(false) << std::endl;
        std::cout << "Outof(100ns) " << DiGi->GetNo_AheadOfTime(false) << std::endl;
        std::cout << "No(Gen) " << DiGi->GetOpticalNo(true) << std::endl;
        std::cout << "No(LUT saved) " << DiGi->GetOpticalNo(false) << std::endl;
    }
    ResetPixelHistoryUnit(cId);
    ClearHitsUnit(cId);
    return true;
}

// Step 1: Reduce gain on some photons, based on probability of hitting a
// recovering pixel
// Step 2: Add SiPM pixel responses to the waveform
std::pair<int, int> OpticalDigitizer::addPixelResponse(const std::vector<OpticalHit *> &hits,
                                                       int cId, std::vector<double> *waveform, double timeZero) {
    //Load parameters //TODO: add to dControl
    auto addClockJitter_ = dControl->Optical_addClockJitter;
    auto sampleInterval_ = dControl->Optical_sampleInterval;
    auto maxTime_ = dControl->Optical_maxTime;
    auto nPixels = dControl->Optical_nPixels;
    auto pixelRecoveryTau_ = dControl->Optical_pixelRecoveryTau;
    auto pixelRecoveryCutoff_ = dControl->Optical_pixelRecoveryCutoff;
    auto clockJitterSigma_ = dControl->Optical_clockJitterSigma;
    auto apertureJitterSigma_ = dControl->Optical_apertureJitterSigma;
    // ----- for step 1
    // Find or create pixelMap
    if (fpixelHitMaps.find(cId) == fpixelHitMaps.end())
        fpixelHitMaps.insert(std::pair<int, std::map<int, double>>(cId, std::map<int, double>()));
    auto pixelHitMap = fpixelHitMaps[cId];

    //create pixelRand
    std::vector<double> pixels(hits.size());
    G4RandFlat::shootArray(hits.size(), &pixels[0], 0., nPixels);
    // ----- for step 2
    // add uncertainties in quadrature to obtain the total sampling jitter
    double samplingJitterSigma =
            sqrt(pow(clockJitterSigma_, 2) + pow(apertureJitterSigma_, 2));

    // get clock jitter array, one number per clock tick in fill
    std::vector<double> samplingJitter(nClockTicks_);
    if (addClockJitter_)
        G4RandGauss::shootArray(nClockTicks_, &samplingJitter[0], 0.,
                                samplingJitterSigma);
    unsigned int iPhotonHit = 0;
    int ahedaOfTime = 0;
    int outOfTime = 0;
    for (auto &hit : hits) {
        double gain = 1.;                                     // pixel response gain at the time of this photon hit
        double photonHitTime = hit->GetArrivalT() - timeZero; // time of photon hit
        if (photonHitTime <
            0) { //now only record the pileup number relative to trigged timeZero, not implement any pileup effect
            ahedaOfTime++;
            continue;
        }
        if (photonHitTime >= maxTime_) {
            outOfTime++;
            continue;
        }
        // ===== step 1 =====
        int pixelNum = pixels.at(iPhotonHit); //global/static enginne
        // reduce gain if this pixel has been previously hit
        auto pixelIter = pixelHitMap.find(pixelNum);
        if (pixelIter != pixelHitMap.end()) {
            double pixelHitTime = pixelIter->second;
            double delta_t = photonHitTime - pixelHitTime;

            // adjust gain only when changes are large enough
            if (delta_t < pixelRecoveryCutoff_ * pixelRecoveryTau_) {
                // this photon hit the same pixel, which is recovering
                gain -= exp(-1. * delta_t / pixelRecoveryTau_);
            }
        }

        // update the pixel hit map
        pixelHitMap[pixelNum] = photonHitTime;

        // ===== step 2 =====
        // add response from this photon to relevant samples in waveform trace
        int startIndex = photonHitTime / sampleInterval_;
        int endIndex = startIndex + nPulseClockTicks_;
        for (int waveformIndex = startIndex; waveformIndex < endIndex;
             ++waveformIndex) {
            if ((waveformIndex >= 0) &&
                (waveformIndex < nClockTicks_)) //now not consider pileup waveform, just count number
            {
                double sampleTime =
                        waveformIndex * sampleInterval_ + samplingJitter[waveformIndex];
                // evaluate pulse shape at t - t0
                waveform->at(waveformIndex) +=
                        gain * getResponse(sampleTime - photonHitTime);
            }
        }
        iPhotonHit++;
    }
    return std::pair<int, int>({ahedaOfTime, outOfTime});
}

// Step 3: Add digitization noise around the pedestal
// Step 4: Digitize ADC sample value
// Step 5: Check for ADC saturation
void OpticalDigitizer::applyRiderBehavior(std::vector<double> *waveform) {
    //Load parameters //TODO: add to dControl
    auto injectNoise_ = dControl->Optical_injectNoise;
    auto noiseSigma_ = dControl->Optical_noiseSigma;
    // get digitization noise array, [ADC count]
    std::vector<double> noise(nClockTicks_);
    if (injectNoise_)
        G4RandGauss::shootArray(nClockTicks_, &noise[0], 0, noiseSigma_);

    // for each clock tick, i.e., for each ADC sample, in the waveform
    for (int iClockTick = 0; iClockTick < nClockTicks_; ++iClockTick) {
        double sampleValue = waveform->at(iClockTick);
        // ===== step 3 =====
        // add digitization noise to sample, [ADC count]
        if (injectNoise_)
            sampleValue += noise.at(iClockTick);
        // ===== step 4,5 =====
        // digitized move to DEvent class. Make bits and min/max more flexible
        // ===== step 6 =====
        // update value in the waveform
        waveform->at(iClockTick) = sampleValue;
    }
}
// ----------------
// HELPER FUNCTIONS
// ----------------

// Approximate SiPM pixel response function, based on lab measurements from UW
double OpticalDigitizer::getResponse(double delta_t) {
    //Load parameters //TODO: add to dControl
    auto pulseTimeZero_ = dControl->Optical_pulseTimeZero;
    auto pulseScaleFactor = dControl->Optical_pulseScaleFactor;
    // auto voltageToSample = dControl->Optical_voltageToADC(); //getting from config

    // check if in range of spline model
    if (minPulseTime_ < delta_t && delta_t < maxPulseTime_) {
        return pulsePolarity_ * pulseScaleFactor *
               pulseShapeSpline_->Eval(delta_t + pulseTimeZero_); //now we only use analog voltage
        //   *  voltageToSample; // [ADC count] //move to DigiForm
    } else {
        // delta_t is out of the spline model's range
        return 0.;
    }
} // end of gm2calo::WaveformBuilder::getResponse
