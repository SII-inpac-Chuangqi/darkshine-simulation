//
// Created by Zhang Yulei on 12/20/20.
//

#include "DEventDisplay.h"

#include "TEveStraightLineSet.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "TEveTrackGL.h"
#include "TEveTrackEditor.h"
#include "TParticle.h"
#include "TEvePathMark.h"
#include "TEveProjectionAxes.h"
#include "TGLViewer.h"
#include "TEveCaloLegoOverlay.h"
#include "TEveLegoEventHandler.h"
#include "TEveBrowser.h"
#include "TGTab.h"

#include "TH2F.h"

#include "CaloHitsDisplay.h"

#include <iostream>

bool DEventDisplay::drawEvent(int id, bool resCam) {
    // Draw Events

    // Draw Geometry first
    if (_drawDetector) drawDetector();

    // read event
    evt = shared_ptr<AnaEvent>(new AnaEvent());
    EvtReader->setEvt(evt);

    EvtReader->ReadEntry(id);

    EvtReader->Convert();
    //evtHistory.emplace_back(evt);
    if (!evt) return false;
    std::cout << "[Event Display] ==> Plotting event: " << evt->getEventId() << std::endl;


    // Draw Event collection by collection
    /***************************/
    /*   Display of Raw Hits   */
    /***************************/
    if (_drawMCTracks) {
        drawInitialParticleStep();
        drawMCParticles();
    }
    auto *SimHitsList = new TEveElementList("Simulated Hits");
    gEve->AddElement(SimHitsList);
    // Tracker Hits
    if (_drawSimuTrkHits) {
        drawSimuTrkHits(SimHitsList);
    }
    if (_drawSimuCaloHits) {
        drawSimuCaloHits(SimHitsList);
    }

    // Display Simulated Calorimeter Hits LEGO
    CaloDisplay = new CaloHitsDisplay();
    auto CALCols = evt->getSimulatedHitCollection();
    makeCaloLego(CALCols, CaloDisplay, _drawSimuCaloLego);

    /************************************/
    /*   Reconstruction Visualization   */
    /************************************/
    RunAnaProcessors();

    /*********************/
    /*   Finalization   */
    /********************/
    gEve->Redraw3D(resCam);

    return true;
}

void DEventDisplay::drawInitialParticleStep() {
    // Step Collection
    auto StepCols = evt->getStepCollection();
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
}

void DEventDisplay::drawMCParticles() {
    // MC Collection ( e.g. Particle Track)
    // Initialize Track
    auto *gMCTrackList = new TEveTrackList("MC Tracks");
    gEve->AddElement(gMCTrackList);
    auto *trkProp = gMCTrackList->GetPropagator();
    trkProp->SetMagFieldObj(new DSMagneticField());

    auto MCCols = evt->getMcParticleCollection();
    for (const auto &MCCol : MCCols) {
        auto MCs = MCCol.second;
        std::cout << "[Event Display] ==> Draw Collection: " << MCCol.first;
        std::cout << ", with total particles: " << MCs->size() << std::endl;
        if (MCs->size() < 2) continue;
        for (unsigned i = 0; i < MCs->size(); ++i) {
            if (MCs->at(i)->getId() == 1) continue;
            if (MCs->at(i)->getEnergy() < MC_Emin) continue;
            if ( (MC_PDG) && (abs(MCs->at(i)->getPdg()) != MC_PDG)) continue;
            TEveTrack *track = makeMCTrack(trkProp, i, MCs->at(i));
            gMCTrackList->AddElement(track);
        }
    }
    gMCTrackList->MakeTracks();
}

void DEventDisplay::drawSimuTrkHits(TEveElementList *SimHitsList) {
    /***************/
    /*   Tracker   */
    /***************/
    // Ploting raw hits
    // basic idea is to draw a box on the hit point, but the box is scaled with a factor
    auto TrkCols = evt->getSimulatedHitCollection();
    for (const auto &TrkCol : TrkCols) {
        // only count hits in tracker
        if (!TString(TrkCol.first).Contains("Trk")) continue;
        if (TrkCol.second->empty()) continue;
        auto *SimTrkHits = new TEveElementList(TrkCol.first.data());
        SimHitsList->AddElement(SimTrkHits);
        auto Trks = TrkCol.second;
        std::cout << "[Event Display] ==> Draw Collection: " << TrkCol.first;
        std::cout << ", with total hits: " << Trks->size() << std::endl;
        if (Trks->empty()) continue;

        for (auto trk_hit : *Trks) {
            // Skip calo hits under threshold
            if (trk_hit->getE() < Trk_Emin && TString(TrkCol.first).Contains("Trk")) continue;
            auto *box = makeTrackerBox(trk_hit, _scale_factor_SimuTrkHits);
            SimTrkHits->AddElement(reinterpret_cast<TEveElement *>(box));
        }
    }
}

void DEventDisplay::drawSimuCaloHits(TEveElementList *SimHitsList) {
    /************/
    /*   CALO   */
    /************/
    // Ploting raw hits
    // basic idea is to draw a box on the hit point
    auto CALCols = evt->getSimulatedHitCollection();
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
            auto *box = makeSimuCaloBox(cal_hit, EMax);
            SimHits->AddElement(reinterpret_cast<TEveElement *>(box));
        }
    }
}

void DEventDisplay::makeLines(TEveStraightLineSet *lineSet, const TVector3 &start, const TVector3 &end,
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

TEveTrack *DEventDisplay::makeMCTrack(TEveTrackPropagator *trkProp, unsigned id, McParticle *mc) {
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
    track->SetCharge(mc->getPdg() / abs(mc->getPdg()));
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
            track->SetLineWidth(4);
            track->SetLineStyle(2);
        } else if (abs(mc->getPdg()) > 1e2 || abs(mc->getPdg()) < 1e3) {
            track->SetLineWidth(3);
            track->SetLineStyle(3);
        } else if (abs(mc->getPdg()) > 1e3 || mc->getP() > 1e3) { // Require Momentum > 1 GeV
            track->SetLineWidth(4);
            track->SetLineStyle(4);
        }
        if (mc->getP() > 1e3) { // Highlight
            track->SetLineWidth(track->GetLineWidth() * 1.5);
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


TEveBox *DEventDisplay::makeBox(const double *abs_pos, const double *half_size) {
    auto *box = new TEveBox();
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

    return box;
}

TEveBox *DEventDisplay::makeSimuCaloBox(SimulatedHit *hit, double EMax) const {
    auto cur_node = gGeoManager->FindNode(hit->getX() / 10, hit->getY() / 10, hit->getZ() / 10);
    auto *cur_shape = dynamic_cast<TGeoBBox *>(cur_node->GetVolume()->GetShape());
    double abs_pos[3] = {hit->getX() / 10, hit->getY() / 10, hit->getZ() / 10};
    double half_size[3] = {cur_shape->GetDX(), cur_shape->GetDY(), cur_shape->GetDZ()};

    if (_drawScaleSimuCaloBox) {
        double ratio =
                log(hit->getE() / EMax / _scale_factor_SimuCaloHits + 1) / log(1 / _scale_factor_SimuCaloHits + 1);
        half_size[0] *= ratio;
        half_size[1] *= ratio;
        half_size[2] *= ratio;
    }

    auto *box = makeBox(abs_pos, half_size);
    box->SetName((Form("Cell %d", hit->getCellId())));

    auto color = FindColor(hit->getE(), EMax);
    box->SetLineColor(color);
    box->SetFillColor(color);
    if (!_drawScaleSimuCaloBox)
        box->SetMainAlpha(log(log(hit->getE() + 1) / log(EMax + 1) + 1));
    else
        box->SetMainAlpha(0.65);

    box->SetTitle(Form("CellID=%d, ID=(%d, %d, %d)\n"
                       "E = %.3f , E_EM = %.3f, E_Had = %.3f [MeV]\n"
                       "Center=(%.3f, %.3f, %.3f) [mm]\n",
                       hit->getCellId(), hit->getCellIdX(), hit->getCellIdY(), hit->getCellIdZ(),
                       hit->getE(), hit->getEdepEm(), hit->getEdepHad(),
                       hit->getX(), hit->getY(), hit->getZ()
    ));
    return box;
}

TEveBox *DEventDisplay::makeRecCaloBox(CalorimeterHit *hit, double EMax) {
    auto cur_node = gGeoManager->FindNode(hit->getX() / 10, hit->getY() / 10, hit->getZ() / 10);
    auto *cur_shape = dynamic_cast<TGeoBBox *>(cur_node->GetVolume()->GetShape());
    double abs_pos[3] = {hit->getX() / 10, hit->getY() / 10, hit->getZ() / 10};
    double half_size[3] = {cur_shape->GetDX(), cur_shape->GetDY(), cur_shape->GetDZ()};

    auto *box = makeBox(abs_pos, half_size);
    box->SetName((Form("Cell %d", hit->getCellId())));

    //auto color = FindColor(hit->getE(), EMax);
    auto color = kYellow;
    box->SetLineColor(color);
    box->SetFillColor(color);
    //box->SetMainAlpha(log(log(hit->getE() + 1) / log(EMax + 1) + 1));

    box->SetTitle(Form("CellID=%d, ID=(%d, %d, %d)\n"
                       "E = %.3f [MeV]\n"
                       "Center=(%.3f, %.3f, %.3f) [mm]\n",
                       hit->getCellId(), hit->getCellIdX(), hit->getCellIdY(), hit->getCellIdZ(),
                       hit->getE(),
                       hit->getX(), hit->getY(), hit->getZ()
    ));
    return box;
}

template<class Hit>
TEveBox *DEventDisplay::makeTrackerBox(Hit *hit, double scale) {
    auto cur_node = gGeoManager->FindNode(hit->getX() / 10, hit->getY() / 10, hit->getZ() / 10 + 1e-6);
    auto *cur_shape = dynamic_cast<TGeoBBox *>(cur_node->GetVolume()->GetShape());
    double abs_pos[3] = {hit->getX() / 10, hit->getY() / 10, hit->getZ() / 10};
    double half_size[3] = {cur_shape->GetDX() * scale, cur_shape->GetDY() * scale, cur_shape->GetDZ() * 0.1};

    auto *box = makeBox(abs_pos, half_size);
    box->SetName((Form("Tracker Hit %d", hit->getCellId())));

    auto color = kMagenta;
    box->SetLineColor(color);
    box->SetFillColor(color);
    box->SetMainAlpha(1.0);

    box->SetTitle(Form("CellID=%d, ID=(%d, %d, %d)\n"
                       "E = %.3f [MeV], t = %.3f [ns]\n"
                       "Center=(%.3f, %.3f, %.3f) [mm]\n",
                       hit->getCellId(), hit->getCellIdX(), hit->getCellIdY(), hit->getCellIdZ(),
                       hit->getE(), hit->getT(),
                       hit->getX(), hit->getY(), hit->getZ()
    ));
    return box;
}

template<class CaloCol>
void DEventDisplay::makeCaloLego(CaloCol col, CaloHitsDisplay *calo_dis, bool if_draw_lego) {
    // Convert Calo Hits to Eve Calo Data
    // ECAL is split into several slices with respect to Z layer
    // HCAL merges together

    int ECAL_slice_number = ECALslice_calo ? (int) ECAL_Cell_Arr[2] : 1;
    std::cout << "[Calo Data Hist] ==> ECAL slice number: " << ECAL_slice_number << std::endl;

    std::vector<std::vector<CaloHit> > tmpCaloHits;

    for (const auto &CALCol : col) {
        // only count hits in calorimeter
        if (!TString(CALCol.first).Contains("CAL")) continue;
        if (CALCol.second->empty()) continue;
        auto CALs = CALCol.second;
        if (TString(CALCol.first).Contains("ECAL_Center") && _drawECAL_calo) {
            // Only Slice for ECAL Center
            for (int i = 0; i < ECAL_slice_number; ++i) {
                tmpCaloHits.emplace_back();
            }

            // Fill Calo hits in to hist 2F vector
            for (auto hits : *CALs) {
                int i = ECALslice_calo ? hits->getCellIdZ() - 1 : 0;
                CaloHit tmp;
                tmp.X = hits->getX() / 10; // cm
                tmp.Y = hits->getY() / 10; // cm
                tmp.Z = hits->getZ() / 10; // cm
                tmp.E = hits->getE();    // MeV
                tmp.Color = kGreen + i;
                tmp.id = hits->getCellId();
                tmp.id_x = hits->getCellIdX();
                tmp.id_y = hits->getCellIdY();
                tmp.id_z = hits->getCellIdZ();

                tmpCaloHits.at(i).push_back(tmp);
            }
        }
        if (TString(CALCol.first).Contains("HCAL") && _drawHCAL_calo) {
            // unused
        }
    }
    for (unsigned i = 0; i < tmpCaloHits.size(); ++i) {
        if (i < (unsigned) ECAL_slice_number) {
            auto name = "ECAL" + to_string(i);
            calo_dis->name.emplace_back(name);
            calo_dis->E_thre.push_back(ECAL_Emin * 1e-3);
            calo_dis->color.push_back(kBlue + i);
        } else {
            calo_dis->name.emplace_back("HCAL");
            calo_dis->E_thre.push_back(HCAL_Emin * 1e-3);
            calo_dis->color.push_back(kMagenta);
        }
    }
    calo_dis->xbin = ECAL_Cell_Arr[0];
    calo_dis->ybin = ECAL_Cell_Arr[1];
    calo_dis->zbin = ECAL_Cell_Arr[2];
    calo_dis->xmin = -ECAL_Size[0];
    calo_dis->xmax = ECAL_Size[0];
    calo_dis->ymin = -ECAL_Size[1];
    calo_dis->ymax = ECAL_Size[1];
    calo_dis->zmin = -ECAL_Size[2] + ECAL_Z_Move;
    calo_dis->zmax = ECAL_Size[2] + ECAL_Z_Move;

    calo_dis->if_drawLego = if_draw_lego;
    calo_dis->if_log = _drawLogSacle;
    calo_dis->scale_factor = _scale_factor_Lego;
    calo_dis->calovec = tmpCaloHits;
    calo_dis->makeLego(win_v.at(0), win_s.at(0), dXY);
    calo_dis->calovec = tmpCaloHits;
    calo_dis->makeLego(win_v.at(1), win_s.at(1), dXZ);
    calo_dis->calovec = tmpCaloHits;
    calo_dis->makeLego(win_v.at(2), win_s.at(2), dYZ);

}


void DEventDisplay::MakeViewerScene(TEveWindowSlot *slot, TEveViewer *&v, TEveScene *&s) {

    // Create a scene and a viewer in the given slot.

    v = new TEveViewer("Viewer");
    v->SpawnGLViewer(reinterpret_cast<TGedEditor *>(gEve->GetEditor()));
    slot->ReplaceWindow(v);
    gEve->GetViewers()->AddElement(v);
    s = gEve->SpawnNewScene("Scene");
    v->AddScene(s);
}
