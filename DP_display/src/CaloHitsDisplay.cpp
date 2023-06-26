//
// Created by Zhang Yulei on 12/21/20.
//

#include "CaloHitsDisplay.h"
#include "TGLCameraOverlay.h"
#include "TGListTree.h"

#include "TGLAxis.h"
#include "TAxis.h"

bool compE(CaloHit h1, CaloHit h2) {
    return (h1.E < h2.E);
}

CaloHitsDisplay::CaloHitsDisplay() {
    CaloHitsDisplayList = new TEveElementList("Calo Hits Plot");
    // LegoListTree = gEve->AddToListTree(CaloHitsDisplayList, kFALSE);
}

CaloHitsDisplay::~CaloHitsDisplay() {
    CaloHitsDisplayList->DestroyElements();
    grid_line->DestroyElements();
    CaloHitsList->DestroyElements();
    delete CaloHitsDisplayList;
    delete grid_line;
    delete CaloHitsList;
}

//note for staggered ECAL there is no well-defined projection!
void CaloHitsDisplay::makeLego(TEveViewer *v, TEveScene *s, ProjectionPlane plane) {

    s->DestroyElements();

    if (plane == dXY) {
        v->SetElementName("CaloHits Lego (X-Y)");
        grid_line = new TEveStraightLineSet("(X-Y) Grid");
        CaloHitsList = new TEveElementList("(X-Y) Calo Hits");
    } else if (plane == dXZ) {
        v->SetElementName("CaloHits Lego (X-Z)");
        grid_line = new TEveStraightLineSet("(X-Z) Grid");
        CaloHitsList = new TEveElementList("(X-Z) Calo Hits");
    } else if (plane == dYZ) {
        v->SetElementName("CaloHits Lego (Y-Z)");
        grid_line = new TEveStraightLineSet("(Y-Z) Grid");
        CaloHitsList = new TEveElementList("(Y-Z) Calo Hits");
    }

    CaloHitsDisplayList->AddElement(grid_line);
    CaloHitsDisplayList->AddElement(CaloHitsList);

    v->GetGLViewer()->SetGuideState(TGLUtil::kAxesEdge, true, false, nullptr);
    v->GetGLViewer()->DrawGuides();
    s->SetElementName("Scene - Calo Lego");

    // Make Grid First
    makeGrid(plane, kWhite);
    //s->AddElement(grid_line);

    if (!if_drawLego || calovec.empty()) return;

    vector<CaloHit> sumhit_table;
    vector<CaloHit> hit_table;
    // Loop first to find the Maximum Energy Value
    // EMax is to scale the total lego height
    for (const auto &calo : calovec) {
        for (auto hit : calo) {
            bool found = false;
            for (auto exist_hit : sumhit_table) {
                if (hit.inSamePlane(exist_hit, plane)) {
                    exist_hit.E += hit.E;
                    found = true;
                    break;
                }
            }
            if (!found) sumhit_table.push_back(hit);
        }
    }
    auto E_Max_sum = scale_factor * (*(max_element(sumhit_table.begin(), sumhit_table.end(), compE))).E;

    // Build Lego Layer by Layer
    for (const auto &calo : calovec) {
        // n layers
        // build boxes
        for (auto hit : calo) {
            double height = 0;
            double orig_E = hit.E;
            // Fill Reference Table
            bool found = false;
            hit.E = (if_log) ? 5 * log(hit.E / E_Max_sum + 1) : hit.E / E_Max_sum;
            for (auto exist_hit : hit_table) {
                if (hit.inSamePlane(exist_hit, plane)) {
                    height = exist_hit.E;
                    exist_hit.E += hit.E;
                    found = true;
                    break;
                }
            }
            if (!found)
                hit_table.push_back(hit);

            double abs_pos[3] = {hit.X, hit.Y, hit.Z};
            double half_size[3] = {delta_x / 2, delta_y / 2, delta_z / 2}; // height is the energy

            if (plane == dXY) {
                abs_pos[2] = height + hit.E / 2;
                half_size[2] = hit.E / 2;
            } else if (plane == dXZ) {
                abs_pos[1] = height + hit.E / 2;
                half_size[1] = hit.E / 2;
            } else if (plane == dYZ) {
                abs_pos[0] = height + hit.E / 2;
                half_size[0] = hit.E / 2;
            }

            auto box = DEventDisplay::makeBox(abs_pos, half_size);
            hit.Color = DEventDisplay::FindColor(orig_E, E_Max_sum / scale_factor);
            box->SetLineColor(hit.Color);
            box->SetFillColor(hit.Color);
            box->SetName(Form("Cell %d", hit.id));
            box->SetTitle(Form("CellID = %d, ID = (%d, %d, %d)\n"
                               "E = %.3f [MeV], E_height = %.3f\n"
                               "Center = (%.3f, %.3f, %.3f) [cm]\n",
                               hit.id, hit.id_x, hit.id_y, hit.id_z,
                               orig_E, hit.E,
                               hit.X, hit.Y, hit.Z
            ));
            CaloHitsList->AddElement(reinterpret_cast<TEveElement *>(box));
        }
    }

    s->AddElement(CaloHitsList);

}

void CaloHitsDisplay::makeGrid(ProjectionPlane plane, Color_t grid_color) {
    // Draw Grid Line
    if (xbin * ybin * zbin <= 0) return;
    if (xmax - xmin < 0 || ymax - ymin < 0 || zmax - zmin < 0) return;

    delta_x = (xmax - xmin) / (float) xbin;
    delta_y = (ymax - ymin) / (float) ybin;
    delta_z = (zmax - zmin) / (float) zbin;

    // make x grid
    unsigned bin1 = 0, bin2 = 0;
    if (plane == dXY) {
        bin1 = xbin;
        bin2 = ybin;
    } else if (plane == dXZ) {
        bin1 = xbin;
        bin2 = zbin;
    } else if (plane == dYZ) {
        bin1 = ybin;
        bin2 = zbin;
    }

    for (unsigned i = 0; i < bin1 + 1; ++i) {
        TVector3 start(0, 0, 0.);
        TVector3 end(0, 0, 0.);
        if (plane == dXY) {
            start = TVector3((xmin + (float) i * delta_x), ymin, 0.);
            end = TVector3((xmin + (float) i * delta_x), ymax, 0.);
        } else if (plane == dXZ) {
            start = TVector3((xmin + (float) i * delta_x), 0., zmin);
            end = TVector3((xmin + (float) i * delta_x), 0., zmax);
        } else if (plane == dYZ) {
            start = TVector3(0., (ymin + (float) i * delta_y), zmin);
            end = TVector3(0., (ymin + (float) i * delta_y), zmax);
        }

        DEventDisplay::makeLines(grid_line, start, end, grid_color, 1, false, 1.0, 0);
    }

    // make y grid
    for (unsigned i = 0; i < bin2 + 1; ++i) {
        TVector3 start(0, 0, 0.);
        TVector3 end(0, 0, 0.);
        if (plane == dXY) {
            start = TVector3(xmin, (ymin + (float) i * delta_y), -1e-6);
            end = TVector3(xmax, (ymin + (float) i * delta_y), -1e-6);
        } else if (plane == dXZ) {
            start = TVector3(xmin, -1e-6, (zmin + (float) i * delta_z));
            end = TVector3(xmax, -1e-6, (zmin + (float) i * delta_z));
        } else if (plane == dYZ) {
            start = TVector3(-1e-6, ymin, (zmin + (float) i * delta_z));
            end = TVector3(-1e-6, ymax, (zmin + (float) i * delta_z));
        }

        DEventDisplay::makeLines(grid_line, start, end, grid_color, 1, false, 0.5, 0);
    }
    grid_line->SetMainAlpha(0.5);
}

