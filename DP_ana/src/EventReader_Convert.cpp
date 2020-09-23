//
// Created by Zhang Yulei on 9/20/20.
//

#include "Core/EventReader.h"

void EventReader::ConvertMC() {
    if (!TRUTH_MC_id->empty()) {
        auto mc = evt->RegisterMCParticleCollection("RawMCParticle");
        for (unsigned i = 0; i < TRUTH_MC_id->size(); ++i) {
            auto mcp = new McParticle();
            mcp->setId(static_cast<int>(i));
            mcp->setPdg(TRUTH_MC_PDG->at(i));
            // Energy
            mcp->setEnergy(TRUTH_MC_E->at(i));
            mcp->setERemain(TRUTH_MC_Eremain->at(i));
            // Momentum
            mcp->setPx(TRUTH_MC_Mom->at(i)[0]);
            mcp->setPy(TRUTH_MC_Mom->at(i)[1]);
            mcp->setPz(TRUTH_MC_Mom->at(i)[2]);
            // Vertex Position
            mcp->setVertexX(TRUTH_MC_VPos->at(i)[0]);
            mcp->setVertexY(TRUTH_MC_VPos->at(i)[1]);
            mcp->setVertexZ(TRUTH_MC_VPos->at(i)[2]);
            // End Position
            mcp->setEndPointX(TRUTH_MC_EPos->at(i)[0]);
            mcp->setEndPointY(TRUTH_MC_EPos->at(i)[1]);
            mcp->setEndPointZ(TRUTH_MC_EPos->at(i)[2]);
            // Create Process
            mcp->setCreateProcess(TRUTH_MC_ProcessName->at(i).Data());

            // insert MCParticle into Collection
            mc->emplace_back(mcp);
        }
    }
}

void EventReader::ConvertStep() {
    if (!ip_Pos->empty()) {
        auto stepC = evt->RegisterStepCollection("Initial_Particle_Step");
        for (unsigned i = 0; i < ip_Pos->size(); ++i) {
            auto step = new DStep();
            // Step Energy
            step->setE(ip_Energy->at(i));
            // Step Momentum
            step->setPx(ip_Momentum->at(i)[0]);
            step->setPy(ip_Momentum->at(i)[1]);
            step->setPz(ip_Momentum->at(i)[2]);
            // Step Position
            step->setX(ip_Pos->at(i)[0]);
            step->setY(ip_Pos->at(i)[1]);
            step->setZ(ip_Pos->at(i)[2]);
            // Step Physics Volume
            step->setPVName(ip_PVName->at(i).Data());
            // Step Process
            step->setProcessName(ip_ProcessName->at(i).Data());

            // insert step into Collection
            stepC->emplace_back(step);
        }
    }
}

void EventReader::ConvertTagTrk() {
    if (TagTrk1_No != 0) {
        auto TagTrk1Collection = evt->RegisterSimulatedHitCollection("TagTrk1");
        for (int i = 0; i < TagTrk1_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(TagTrk1_Eleak_Wrapper);
            Hit->setE(TagTrk1_Edep->at(i));
            Hit->setEdepEm(TagTrk1_EdepEM->at(i));
            Hit->setEdepHad(TagTrk1_EdepHad->at(i));
            Hit->setX(TagTrk1_X->at(i));
            Hit->setY(TagTrk1_Y->at(i));
            Hit->setZ(TagTrk1_Z->at(i));
            Hit->setCellId(TagTrk1_DetectorID->at(i));
            Hit->setT(TagTrk1_Time->at(i));
            Hit->setDetector(TagTrk);

            TagTrk1Collection->emplace_back(Hit);
        }
    }
    if (TagTrk2_No != 0) {
        auto TagTrk2Collection = evt->RegisterSimulatedHitCollection("TagTrk2");
        for (int i = 0; i < TagTrk2_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(TagTrk2_Eleak_Wrapper);
            Hit->setE(TagTrk2_Edep->at(i));
            Hit->setEdepEm(TagTrk2_EdepEM->at(i));
            Hit->setEdepHad(TagTrk2_EdepHad->at(i));
            Hit->setX(TagTrk2_X->at(i));
            Hit->setY(TagTrk2_Y->at(i));
            Hit->setZ(TagTrk2_Z->at(i));
            Hit->setCellId(TagTrk2_DetectorID->at(i));
            Hit->setT(TagTrk2_Time->at(i));
            Hit->setDetector(TagTrk);

            TagTrk2Collection->emplace_back(Hit);
        }
    }
}

void EventReader::ConvertRecTrk() {
    if (RecTrk1_No != 0) {
        auto RecTrk1Collection = evt->RegisterSimulatedHitCollection("RecTrk1");
        for (int i = 0; i < RecTrk1_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(RecTrk1_Eleak_Wrapper);
            Hit->setE(RecTrk1_Edep->at(i));
            Hit->setEdepEm(RecTrk1_EdepEM->at(i));
            Hit->setEdepHad(RecTrk1_EdepHad->at(i));
            Hit->setX(RecTrk1_X->at(i));
            Hit->setY(RecTrk1_Y->at(i));
            Hit->setZ(RecTrk1_Z->at(i));
            Hit->setCellId(RecTrk1_DetectorID->at(i));
            Hit->setT(RecTrk1_Time->at(i));
            Hit->setDetector(RecTrk);

            RecTrk1Collection->emplace_back(Hit);
        }
    }
    if (RecTrk2_No != 0) {
        auto RecTrk2Collection = evt->RegisterSimulatedHitCollection("RecTrk2");
        for (int i = 0; i < RecTrk2_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(RecTrk2_Eleak_Wrapper);
            Hit->setE(RecTrk2_Edep->at(i));
            Hit->setEdepEm(RecTrk2_EdepEM->at(i));
            Hit->setEdepHad(RecTrk2_EdepHad->at(i));
            Hit->setX(RecTrk2_X->at(i));
            Hit->setY(RecTrk2_Y->at(i));
            Hit->setZ(RecTrk2_Z->at(i));
            Hit->setCellId(RecTrk2_DetectorID->at(i));
            Hit->setT(RecTrk2_Time->at(i));
            Hit->setDetector(RecTrk);

            RecTrk2Collection->emplace_back(Hit);

        }
    }
}

void EventReader::ConvertECAL_Center() {
    if (ECAL_Center_No != 0) {
        auto ECAL_CenterCollection = evt->RegisterSimulatedHitCollection("ECAL_Center");
        for (int i = 0; i < ECAL_Center_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(ECAL_Center_Eleak_Wrapper);
            Hit->setE(ECAL_Center_Edep->at(i));
            Hit->setEdepEm(ECAL_Center_EdepEM->at(i));
            Hit->setEdepHad(ECAL_Center_EdepHad->at(i));
            Hit->setX(ECAL_Center_X->at(i));
            Hit->setY(ECAL_Center_Y->at(i));
            Hit->setZ(ECAL_Center_Z->at(i));
            Hit->setCellId(ECAL_Center_DetectorID->at(i));
            Hit->setT(ECAL_Center_Time->at(i));
            Hit->setDetector(ECAL_Center);

            ECAL_CenterCollection->emplace_back(Hit);
        }
    }
}

void EventReader::ConvertECAL_Outer() {
    if (ECAL_Outer_1_No != 0) {
        auto ECAL_Outer_1_Collection = evt->RegisterSimulatedHitCollection("ECAL_Outer_1");
        for (int i = 0; i < ECAL_Outer_1_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(ECAL_Outer_1_Eleak_Wrapper);
            Hit->setE(ECAL_Outer_1_Edep->at(i));
            Hit->setEdepEm(ECAL_Outer_1_EdepEM->at(i));
            Hit->setEdepHad(ECAL_Outer_1_EdepHad->at(i));
            Hit->setX(ECAL_Outer_1_X->at(i));
            Hit->setY(ECAL_Outer_1_Y->at(i));
            Hit->setZ(ECAL_Outer_1_Z->at(i));
            Hit->setCellId(ECAL_Outer_1_DetectorID->at(i));
            Hit->setT(ECAL_Outer_1_Time->at(i));
            Hit->setDetector(ECAL_Outer);

            ECAL_Outer_1_Collection->emplace_back(Hit);
        }
    }
    if (ECAL_Outer_2_No != 0) {
        auto ECAL_Outer_2_Collection = evt->RegisterSimulatedHitCollection("ECAL_Outer_2");
        for (int i = 0; i < ECAL_Outer_2_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(ECAL_Outer_2_Eleak_Wrapper);
            Hit->setE(ECAL_Outer_2_Edep->at(i));
            Hit->setEdepEm(ECAL_Outer_2_EdepEM->at(i));
            Hit->setEdepHad(ECAL_Outer_2_EdepHad->at(i));
            Hit->setX(ECAL_Outer_2_X->at(i));
            Hit->setY(ECAL_Outer_2_Y->at(i));
            Hit->setZ(ECAL_Outer_2_Z->at(i));
            Hit->setCellId(ECAL_Outer_2_DetectorID->at(i));
            Hit->setT(ECAL_Outer_2_Time->at(i));
            Hit->setDetector(ECAL_Outer);

            ECAL_Outer_2_Collection->emplace_back(Hit);
        }
    }
    if (ECAL_Outer_3_No != 0) {
        auto ECAL_Outer_3_Collection = evt->RegisterSimulatedHitCollection("ECAL_Outer_3");
        for (int i = 0; i < ECAL_Outer_3_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(ECAL_Outer_3_Eleak_Wrapper);
            Hit->setE(ECAL_Outer_3_Edep->at(i));
            Hit->setEdepEm(ECAL_Outer_3_EdepEM->at(i));
            Hit->setEdepHad(ECAL_Outer_3_EdepHad->at(i));
            Hit->setX(ECAL_Outer_3_X->at(i));
            Hit->setY(ECAL_Outer_3_Y->at(i));
            Hit->setZ(ECAL_Outer_3_Z->at(i));
            Hit->setCellId(ECAL_Outer_3_DetectorID->at(i));
            Hit->setT(ECAL_Outer_3_Time->at(i));
            Hit->setDetector(ECAL_Outer);

            ECAL_Outer_3_Collection->emplace_back(Hit);
        }
    }
    if (ECAL_Outer_4_No != 0) {
        auto ECAL_Outer_4_Collection = evt->RegisterSimulatedHitCollection("ECAL_Outer_4");
        for (int i = 0; i < ECAL_Outer_4_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(ECAL_Outer_4_Eleak_Wrapper);
            Hit->setE(ECAL_Outer_4_Edep->at(i));
            Hit->setEdepEm(ECAL_Outer_4_EdepEM->at(i));
            Hit->setEdepHad(ECAL_Outer_4_EdepHad->at(i));
            Hit->setX(ECAL_Outer_4_X->at(i));
            Hit->setY(ECAL_Outer_4_Y->at(i));
            Hit->setZ(ECAL_Outer_4_Z->at(i));
            Hit->setCellId(ECAL_Outer_4_DetectorID->at(i));
            Hit->setT(ECAL_Outer_4_Time->at(i));
            Hit->setDetector(ECAL_Outer);

            ECAL_Outer_4_Collection->emplace_back(Hit);
        }
    }
}

void EventReader::ConvertHCAL() {
    if (HCAL_0_No != 0) {
        auto HCAL_0_Collection = evt->RegisterSimulatedHitCollection("HCAL_0");
        for (int i = 0; i < HCAL_0_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(HCAL_0_Eleak_Wrapper);
            Hit->setE(HCAL_0_Edep->at(i));
            Hit->setEdepEm(HCAL_0_EdepEM->at(i));
            Hit->setEdepHad(HCAL_0_EdepHad->at(i));
            Hit->setX(HCAL_0_X->at(i));
            Hit->setY(HCAL_0_Y->at(i));
            Hit->setZ(HCAL_0_Z->at(i));
            Hit->setCellId(HCAL_0_DetectorID->at(i));
            Hit->setT(HCAL_0_Time->at(i));
            Hit->setDetector(HCAL);

            HCAL_0_Collection->emplace_back(Hit);
        }
    }
    if (HCAL_1_No != 0) {
        auto HCAL_1_Collection = evt->RegisterSimulatedHitCollection("HCAL_1");
        for (int i = 0; i < HCAL_1_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(HCAL_1_Eleak_Wrapper);
            Hit->setE(HCAL_1_Edep->at(i));
            Hit->setEdepEm(HCAL_1_EdepEM->at(i));
            Hit->setEdepHad(HCAL_1_EdepHad->at(i));
            Hit->setX(HCAL_1_X->at(i));
            Hit->setY(HCAL_1_Y->at(i));
            Hit->setZ(HCAL_1_Z->at(i));
            Hit->setCellId(HCAL_1_DetectorID->at(i));
            Hit->setT(HCAL_1_Time->at(i));
            Hit->setDetector(HCAL);

            HCAL_1_Collection->emplace_back(Hit);
        }
    }
    if (HCAL_2_No != 0) {
        auto HCAL_2_Collection = evt->RegisterSimulatedHitCollection("HCAL_2");
        for (int i = 0; i < HCAL_2_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(HCAL_2_Eleak_Wrapper);
            Hit->setE(HCAL_2_Edep->at(i));
            Hit->setEdepEm(HCAL_2_EdepEM->at(i));
            Hit->setEdepHad(HCAL_2_EdepHad->at(i));
            Hit->setX(HCAL_2_X->at(i));
            Hit->setY(HCAL_2_Y->at(i));
            Hit->setZ(HCAL_2_Z->at(i));
            Hit->setCellId(HCAL_2_DetectorID->at(i));
            Hit->setT(HCAL_2_Time->at(i));
            Hit->setDetector(HCAL);

            HCAL_2_Collection->emplace_back(Hit);
        }
    }
    if (HCAL_3_No != 0) {
        auto HCAL_3_Collection = evt->RegisterSimulatedHitCollection("HCAL_3");
        for (int i = 0; i < HCAL_3_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(HCAL_3_Eleak_Wrapper);
            Hit->setE(HCAL_3_Edep->at(i));
            Hit->setEdepEm(HCAL_3_EdepEM->at(i));
            Hit->setEdepHad(HCAL_3_EdepHad->at(i));
            Hit->setX(HCAL_3_X->at(i));
            Hit->setY(HCAL_3_Y->at(i));
            Hit->setZ(HCAL_3_Z->at(i));
            Hit->setCellId(HCAL_3_DetectorID->at(i));
            Hit->setT(HCAL_3_Time->at(i));
            Hit->setDetector(HCAL);

            HCAL_3_Collection->emplace_back(Hit);
        }
    }
    if (HCAL_4_No != 0) {
        auto HCAL_4_Collection = evt->RegisterSimulatedHitCollection("HCAL_4");
        for (int i = 0; i < HCAL_4_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(HCAL_4_Eleak_Wrapper);
            Hit->setE(HCAL_4_Edep->at(i));
            Hit->setEdepEm(HCAL_4_EdepEM->at(i));
            Hit->setEdepHad(HCAL_4_EdepHad->at(i));
            Hit->setX(HCAL_4_X->at(i));
            Hit->setY(HCAL_4_Y->at(i));
            Hit->setZ(HCAL_4_Z->at(i));
            Hit->setCellId(HCAL_4_DetectorID->at(i));
            Hit->setT(HCAL_4_Time->at(i));
            Hit->setDetector(HCAL);

            HCAL_4_Collection->emplace_back(Hit);
        }
    }
    if (HCAL_5_No != 0) {
        auto HCAL_5_Collection = evt->RegisterSimulatedHitCollection("HCAL_5");
        for (int i = 0; i < HCAL_5_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(HCAL_5_Eleak_Wrapper);
            Hit->setE(HCAL_5_Edep->at(i));
            Hit->setEdepEm(HCAL_5_EdepEM->at(i));
            Hit->setEdepHad(HCAL_5_EdepHad->at(i));
            Hit->setX(HCAL_5_X->at(i));
            Hit->setY(HCAL_5_Y->at(i));
            Hit->setZ(HCAL_5_Z->at(i));
            Hit->setCellId(HCAL_5_DetectorID->at(i));
            Hit->setT(HCAL_5_Time->at(i));
            Hit->setDetector(HCAL);

            HCAL_5_Collection->emplace_back(Hit);
        }
    }
    if (HCAL_6_No != 0) {
        auto HCAL_6_Collection = evt->RegisterSimulatedHitCollection("HCAL_6");
        for (int i = 0; i < HCAL_6_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(HCAL_6_Eleak_Wrapper);
            Hit->setE(HCAL_6_Edep->at(i));
            Hit->setEdepEm(HCAL_6_EdepEM->at(i));
            Hit->setEdepHad(HCAL_6_EdepHad->at(i));
            Hit->setX(HCAL_6_X->at(i));
            Hit->setY(HCAL_6_Y->at(i));
            Hit->setZ(HCAL_6_Z->at(i));
            Hit->setCellId(HCAL_6_DetectorID->at(i));
            Hit->setT(HCAL_6_Time->at(i));
            Hit->setDetector(HCAL);

            HCAL_6_Collection->emplace_back(Hit);
        }
    }
    if (HCAL_7_No != 0) {
        auto HCAL_7_Collection = evt->RegisterSimulatedHitCollection("HCAL_7");
        for (int i = 0; i < HCAL_7_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(HCAL_7_Eleak_Wrapper);
            Hit->setE(HCAL_7_Edep->at(i));
            Hit->setEdepEm(HCAL_7_EdepEM->at(i));
            Hit->setEdepHad(HCAL_7_EdepHad->at(i));
            Hit->setX(HCAL_7_X->at(i));
            Hit->setY(HCAL_7_Y->at(i));
            Hit->setZ(HCAL_7_Z->at(i));
            Hit->setCellId(HCAL_7_DetectorID->at(i));
            Hit->setT(HCAL_7_Time->at(i));
            Hit->setDetector(HCAL);

            HCAL_7_Collection->emplace_back(Hit);
        }
    }
    if (HCAL_8_No != 0) {
        auto HCAL_8_Collection = evt->RegisterSimulatedHitCollection("HCAL_8");
        for (int i = 0; i < HCAL_8_No; ++i) {
            auto Hit = new SimulatedHit();
            Hit->setId(i);
            Hit->setELeakWrapper(HCAL_8_Eleak_Wrapper);
            Hit->setE(HCAL_8_Edep->at(i));
            Hit->setEdepEm(HCAL_8_EdepEM->at(i));
            Hit->setEdepHad(HCAL_8_EdepHad->at(i));
            Hit->setX(HCAL_8_X->at(i));
            Hit->setY(HCAL_8_Y->at(i));
            Hit->setZ(HCAL_8_Z->at(i));
            Hit->setCellId(HCAL_8_DetectorID->at(i));
            Hit->setT(HCAL_8_Time->at(i));
            Hit->setDetector(HCAL);

            HCAL_8_Collection->emplace_back(Hit);
        }
    }
}

