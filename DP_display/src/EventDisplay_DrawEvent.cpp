//
// Created by Zhang Yulei on 12/20/20.
//

#include "EventDisplay.h"

#include "TEveStraightLineSet.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "TEveTrackGL.h"
#include "TEveTrackEditor.h"
#include "TParticle.h"
#include "TEvePathMark.h"

#include <iostream>

bool EventDisplay::drawEvent(int id, bool resCam) {
    // Draw Events

    // Draw Geometry first
    if (_drawDetector) drawDetector();

    EvtReader->ReadEntry(id);

    EvtReader->Convert();
    evt = EvtReader->getEvt();
    if (!evt) return false;
    std::cout << "[Event Display] ==> Plotting event: " << evt->getEventId() << std::endl;

    if (_drawTracks) {
        // Draw Event collection by collection
        // Step Collection
        auto StepCols = evt->getStepCollection_Old();
        for (const auto &StepCol : StepCols) {
            auto Steps = StepCol.second;
            std::cout << "[Event Display] ==> Draw Collection: " << StepCol.first;
            std::cout << ", with total steps: " << Steps->size() << std::endl;
            if (Steps->size() < 2) continue;
            auto lineSet = new TEveStraightLineSet(StepCol.first.data());
            for (unsigned i = 0; i < Steps->size() - 1; ++i) {
                // loop from 0 to n-1 to draw n-1 lines
                auto step = Steps->at(i);
                auto next_step = Steps->at(i + 1);

                TVector3 start(step->getX() / 10, step->getY() / 10, step->getZ() / 10);
                TVector3 end(next_step->getX() / 10, next_step->getY() / 10, next_step->getZ() / 10);

                makeLines(lineSet, start, end, kRed, 1, false, 3, 0);
            }
            gEve->AddElement(lineSet);
        }

        // MC Collection ( e.g. Particle Track)
        // Initialize Track
        auto *gMCTrackList = new TEveTrackList("MC Tracks");
        gEve->AddElement(gMCTrackList);
        auto *trkProp = gMCTrackList->GetPropagator();
        trkProp->SetMagFieldObj(new DSMagneticField());

        auto MCCols = evt->getMcParticleCollection_Old();
        for (const auto &MCCol : MCCols) {
            auto MCs = MCCol.second;
            std::cout << "[Event Display] ==> Draw Collection: " << MCCol.first;
            std::cout << ", with total particles: " << MCs->size() << std::endl;
            if (MCs->size() < 2) continue;
            for (unsigned i = 0; i < MCs->size(); ++i) {
                if (MCs->at(i)->getId() == 1) continue;
                TEveTrack *track = makeMCTrack(trkProp,i, MCs->at(i));
                gMCTrackList->AddElement(track);
            }
        }
        gMCTrackList->MakeTracks();
    }
    // Tracker Hits
    // Working...

    if (_drawHits) {
        /************/
        /*   CALO   */
        /************/
        // Ploting raw hits
        // basic idea is to draw a box on the hit point
        auto CALCols = evt->getSimulatedHitCollection_Old();
        auto *SimHitsList = new TEveElementList("Simulated Hits");
        gEve->AddElement(SimHitsList);
        for (const auto &CALCol : CALCols) {
            // only count hits in calorimeter
            if (!TString(CALCol.first).Contains("CAL")) continue;
            if (CALCol.second->empty()) continue;
            auto *SimHits = new TEveElementList(CALCol.first.data());
            SimHitsList->AddElement(SimHits);
            auto CALs = CALCol.second;
            std::cout << "[Event Display] ==> Draw Collection: " << CALCol.first;
            std::cout << ", with total hits: " << CALs->size() << std::endl;
            if (CALs->empty()) continue;
            double EMax = 0.;
            for (auto cal_hit : *CALs)
                EMax = (cal_hit->getE() > EMax) ? cal_hit->getE() : EMax;

            for (auto cal_hit : *CALs) {
                // Skip calo hits under threshold
                if (cal_hit->getE() < r_min * EMax) continue;
                if (cal_hit->getE() < ECAL_Emin && TString(CALCol.first).Contains("ECAL")) continue;
                if (cal_hit->getE() < HCAL_Emin && TString(CALCol.first).Contains("HCAL")) continue;
                auto *box = makeCaloBox(cal_hit, EMax);
                SimHits->AddElement(reinterpret_cast<TEveElement *>(box));
            }
        }
    }

    // Finalize
    gEve->Redraw3D(resCam);

    return true;
}

void EventDisplay::makeLines(TEveStraightLineSet *lineSet, const TVector3 &start, const TVector3 &end,
                             const Color_t &color, const Style_t &style, bool drawMarkers,
                             double lineWidth, int markerPos) {
    // Make a straight line between two input points
    lineSet->AddLine(start.x(), start.y(), start.z(), end.x(), end.y(), end.z());
    lineSet->SetLineColor(color);
    lineSet->SetLineStyle(style);
    lineSet->SetLineWidth(lineWidth);
    if (drawMarkers) {
        if (markerPos == 0)
            lineSet->AddMarker(start.x(), start.y(), start.z());
        else
            lineSet->AddMarker(end.x(), end.y(), end.z());
    }
}

TEveTrack *EventDisplay::makeMCTrack(TEveTrackPropagator* trkProp, unsigned id, McParticle *mc) {
    // get mother id
    int m_id = -999;
    auto p = mc->getParents();
    if (p) m_id = p->getId();

    TEveMCTrack rt;
    rt.SetPdgCode(mc->getPdg());
    rt.SetUniqueID(mc->getId());
    rt.SetMomentum(mc->getPx() * 1e-3, mc->getPy() * 1e-3, mc->getPz() * 1e-3, mc->getEnergy() * 1e-3);
    rt.SetProductionVertex(mc->getVertexX() / 10, mc->getVertexY() / 10, mc->getVertexZ() / 10, 0.);
    TVector3 endpoint(mc->getEndPointX() / 10, mc->getEndPointY() / 10, mc->getEndPointZ() / 10);

    auto EndPoint = new TEvePathMark(TEvePathMark::kDecay);
    EndPoint->fV.Set(endpoint);

    auto *track = new TEveTrack(&rt, trkProp);
    track->SetCharge( mc->getPdg()/abs(mc->getPdg()) );
    track->SetName(Form("Trk %d: PDG %d", rt.GetUniqueID(), rt.GetPdgCode()));
    track->SetPdg(mc->getPdg());
    track->SetLineColor(PDG_Color[mc->getPdg()]);
    // Line Style Setting
    if (mc->getERemain() > 0.) {
        track->SetLineWidth(2);
        track->SetLineStyle(10);
    } else {
        // highlighted Special Particle like muons, hadrons
        if (abs(mc->getPdg()) == 13) {
            track->SetLineWidth(5);
            track->SetLineStyle(9);
        } else if (abs(mc->getPdg()) > 1e2 || abs(mc->getPdg()) < 1e3) {
            track->SetLineWidth(4);
            track->SetLineStyle(6);
        } else if (abs(mc->getPdg()) > 1e3 || mc->getP() > 1e3) { // Require Momentum > 1 GeV
            track->SetLineWidth(5);
            track->SetLineStyle(8);
        }
        if (mc->getP() > 1e3) {
            track->SetLineWidth(track->GetLineWidth() * 2);
        }
    }
    track->AddPathMark(*EndPoint);
    track->SetTitle((Form("Index=%d, Pdg=%d\n"
                          "MotherID=%d\n"
                          "E=%.3f, Eremain=%.3f [MeV]\n"
                          "P=(%.3f, %.3f, %.3f) [MeV]\n"
                          "Vertex=(%.3f, %.3f, %.3f) [mm]\n"
                          "End=(%.3f, %.3f, %.3f) [mm]\n"
                          "CreateProcess: %s\n",
                          rt.GetUniqueID(), rt.GetPdgCode(),
                          m_id,
                          mc->getEnergy(), mc->getERemain(),
                          mc->getPx(), mc->getPy(), mc->getPz(),
                          mc->getVertexX(), mc->getVertexY(), mc->getVertexZ(),
                          mc->getEndPointX(), mc->getEndPointY(), mc->getEndPointZ(),
                          mc->getCreateProcess().data()
    )));

    return track;
}

TEveBox *EventDisplay::makeCaloBox(SimulatedHit *hit, double EMax) {
    auto cur_node = gGeoManager->FindNode(hit->getX() / 10, hit->getY() / 10, hit->getZ() / 10);
    auto *cur_shape = dynamic_cast<TGeoBBox *>(cur_node->GetVolume()->GetShape());
    double abs_pos[3] = {hit->getX() / 10, hit->getY() / 10, hit->getZ() / 10};
    double half_size[3] = {cur_shape->GetDX(), cur_shape->GetDY(), cur_shape->GetDZ()};

    auto *box = new TEveBox((Form("Cell %d", hit->getCellId())));
    double vertex[24] = {0.};
    // (a,b,-c)
    vertex[0] = half_size[0] + abs_pos[0];
    vertex[1] = half_size[1] + abs_pos[1];
    vertex[2] = -half_size[2] + abs_pos[2];
    // (a,-b,-c)
    vertex[3] = half_size[0] + abs_pos[0];
    vertex[4] = -half_size[1] + abs_pos[1];
    vertex[5] = -half_size[2] + abs_pos[2];
    // (-a,-b,-c)
    vertex[6] = -half_size[0] + abs_pos[0];
    vertex[7] = -half_size[1] + abs_pos[1];
    vertex[8] = -half_size[2] + abs_pos[2];
    // (-a,b,-c)
    vertex[9] = -half_size[0] + abs_pos[0];
    vertex[10] = half_size[1] + abs_pos[1];
    vertex[11] = -half_size[2] + abs_pos[2];
    // (a,b,c)
    vertex[12] = half_size[0] + abs_pos[0];
    vertex[13] = half_size[1] + abs_pos[1];
    vertex[14] = half_size[2] + abs_pos[2];
    // (a,-b,c)
    vertex[15] = half_size[0] + abs_pos[0];
    vertex[16] = -half_size[1] + abs_pos[1];
    vertex[17] = half_size[2] + abs_pos[2];
    // (-a,-b,c)
    vertex[18] = -half_size[0] + abs_pos[0];
    vertex[19] = -half_size[1] + abs_pos[1];
    vertex[20] = half_size[2] + abs_pos[2];
    // (-a,b,c)
    vertex[21] = -half_size[0] + abs_pos[0];
    vertex[22] = half_size[1] + abs_pos[1];
    vertex[23] = half_size[2] + abs_pos[2];

    for (int k = 0; k < 24; k += 3) box->SetVertex((k / 3), vertex[k], vertex[k + 1], vertex[k + 2]);

    auto color = FindColor(hit->getE(), EMax);
    box->SetLineColor(color);
    box->SetFillColor(color);
    box->SetMainAlpha(log(log(hit->getE() + 1) / log(EMax + 1) + 1));

    box->SetTitle(Form("CellID=%d, ID=(%d, %d, %d)\n"
                       "E=%.3f, E_EM=%.3f, E_Had= %.3f[MeV]\n"
                       "Center=(%.3f, %.3f, %.3f) [mm]\n",
                       hit->getCellId(), hit->getCellIdX(), hit->getCellIdY(), hit->getCellIdZ(),
                       hit->getE(), hit->getEdepEm(), hit->getEdepHad(),
                       hit->getX(), hit->getY(), hit->getZ()
    ));
    return box;
}

