//
// Created by Zhang Yulei on 12/21/20.
//

#ifndef DSIMU_CALOHITSDISPLAY_H
#define DSIMU_CALOHITSDISPLAY_H


#include "EventDisplay.h"
#include "TGLViewer.h"


#include <algorithm>

class EventDisplay;

enum ProjectionPlane {dXY, dXZ, dYZ};

struct CaloHit {
    double X = 0;
    double Y = 0;
    double Z = 0;
    double E = 0;
    Color_t Color = kBlue;

    int id_x = 0;
    int id_y = 0;
    int id_z = 0;
    int id = 0;

    [[nodiscard]] bool inSamePlane(CaloHit h2, ProjectionPlane plane) const {
        if (plane == dXY)
            return (X == h2.X) && (Y == h2.Y);
        if (plane == dXZ)
            return (X == h2.X) && (Z == h2.Z);
        if (plane == dYZ)
            return (Z == h2.Z) && (Y == h2.Y);
    }
};

class CaloHitsDisplay : public TObject {
    // Class to display lego plot
public:
    CaloHitsDisplay();

    ~CaloHitsDisplay() override;

    std::vector<std::vector<CaloHit> > calovec;
    vector<TString> name;
    vector<float> E_thre;
    vector<Color_t> color;

    unsigned xbin = 0;
    float xmin = 0;
    float xmax = 0;
    unsigned ybin = 0;
    float ymin = 0;
    float ymax = 0;
    unsigned zbin = 0;
    float zmin = 0;
    float zmax = 0;
    float delta_x = 0;
    float delta_y = 0;
    float delta_z = 0;

    double scale_factor = 0.1;
    bool if_log = false;

    TEveElementList *CaloHitsDisplayList{nullptr};
    TEveElementList *CaloHitsList{nullptr};

    TEveStraightLineSet *grid_line{nullptr};

    void makeGrid(ProjectionPlane plane, Color_t grid_color = kWhite, float grid_alpha = 1.0);

    void makeLego(TEveViewer* v, TEveScene* s, ProjectionPlane plane);

ClassDefOverride(CaloHitsDisplay, 0);
};


#endif //DSIMU_CALOHITSDISPLAY_H
