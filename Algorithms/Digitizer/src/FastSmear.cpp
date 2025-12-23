//
// Created by Zhang Yulei on 8/3/21.
//

#include "Algo/FastSmear.h"


void FastSmear::RegisterParameters(const std::string &cf, bool aecal, bool ahcal, int seed) {
    config_file = cf;
    apply_to_ecal = aecal;
    apply_to_hcal = ahcal;

    rnd.SetSeed(seed);

    ReadConfigFile();
}

void FastSmear::ReadConfigFile() {
    if (config_file.empty()) {
        Calibration_Table ct = {
                &TRandom::Gaus,
                {0},
                {0},
                {0},
                {0}
        };

        Calibration_Table ct1 = {
                &TRandom::Gaus,
                {8000},
                {31.62e-02},
                {0},
                {0}
        };

    
        Calibration_Table ct2 = {
                &TRandom::Gaus,
                {8000},
                {211.69e-02},
                {0},
                {0.0851}
        };

    
        Calibration_Table ct3 = {
                &TRandom::Gaus,
                {8000},
                {134.56e-02},
                {0.7e-02},
                {0.0001},
        };

    
        Calibration_Table ct4 = {
                &TRandom::Gaus,
                {8000},
                {73.32e-02},
                {0.17e-02},
                {0.7051},
        };

        Calibration_Table hct1 = {
                &TRandom::Gaus,
                {4000}, // E
                {0.12}, // A
                {0}, // B
                {0} // C
        };


        cal_info.at(ECAL_Name).push_back(ct);
        cal_info.at(ECAL_Name).push_back(ct1);
        cal_info.at(ECAL_Name).push_back(ct2);
        cal_info.at(ECAL_Name).push_back(ct3);
        cal_info.at(ECAL_Name).push_back(ct4);

        cal_info.at(HCAL_Name).push_back(ct);
        cal_info.at(HCAL_Name).push_back(hct1);
        cal_info.at(SideHCAL_Name).push_back(ct);
        cal_info.at(SideHCAL_Name).push_back(hct1);
    }
}

double FastSmear::MidPoint(double x1, double x2, double y1, double y2, double x) {
    return (y1 + (y2 - y1) / (x2 - x1) * (x - x1));
}

double FastSmear::CalculateSigma(double E, const Calibration_Table &ct) {
    // (sig/E)^2 = (A/sqrt(E))^2 + B^2 + (C/E)^2

    auto sigma = [](double a, double b, double c, double e) {
        return sqrt(pow(a, 2) * e + pow(b * e, 2) + pow(c, 2));
    };

    if (E <= ct.E.front())
        return sigma(ct.A.front(), ct.B.front(), ct.C.front(), E);

    if (E >= ct.E.back())
        return sigma(ct.A.back(), ct.B.back(), ct.C.back(), E);

    for (unsigned i = 1; i < ct.E.size(); ++i) {
        if (E <= ct.E.at(i)) {
            double A = MidPoint(ct.E.at(i - 1), ct.E.at(i), ct.A.at(i - 1), ct.A.at(i), E);
            double B = MidPoint(ct.E.at(i - 1), ct.E.at(i), ct.B.at(i - 1), ct.B.at(i), E);
            double C = MidPoint(ct.E.at(i - 1), ct.E.at(i), ct.C.at(i - 1), ct.C.at(i), E);

            return sigma(A, B, C, E);
        }
    }

    return 0;
}

void FastSmear::Process(AnaEvent *evt, const string &det_name, const string &cal_name) {
    // Grap Collections
    if (const auto &col = evt->getSimulatedHitCollection(); col.count(det_name) != 0) {
        // Read in collection
        auto hit_col = col.at(det_name);

        for (unsigned i = 0; i < cal_info.at(cal_name).size(); i++) {
            // Register output collection
            auto out_col = evt->RegisterCalorimeterHitCollection(det_name + "_FS" + std::to_string(i));

            // Loop hit to calibrate
            for (auto hit : *hit_col) {
                auto new_hit = new CalorimeterHit();

                double oE = hit->getE();
                auto funcPtr = cal_info.at(cal_name).at(i).funcPtr;
                double new_E = (rnd.*funcPtr)(oE, CalculateSigma(oE, cal_info.at(cal_name).at(i)));
                new_E=std::max(0.,new_E);

                new_hit->setE(new_E);
                new_hit->setId(hit->getId());
                new_hit->setCellId(hit->getCellId());
                new_hit->setCellIdX(hit->getCellIdX());
                new_hit->setCellIdY(hit->getCellIdY());
                new_hit->setCellIdZ(hit->getCellIdZ());
                new_hit->setX(hit->getX());
                new_hit->setY(hit->getY());
                new_hit->setZ(hit->getZ());

                out_col->emplace_back(new_hit);
            }
        }
    }
}



