//
// Created by Zhang Yulei on 12/21/20.
//

#ifndef DSIMU_CALOHITSDISPLAY_H
#define DSIMU_CALOHITSDISPLAY_H

#include "DEventDisplay.h"
#include "TGLViewer.h"

#include <algorithm>

class DEventDisplay;

/// \brief define Lego Plane, only support 3 types
enum ProjectionPlane {
    dXY, dXZ, dYZ
};

/// \brief in order to display different data types,
///        create a specific type for LEGO use
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

    /// \brief Check if two hits are in the same plane
    [[nodiscard]] bool inSamePlane(CaloHit h2, ProjectionPlane plane) const {
        if (plane == dXY)
            return (X == h2.X) && (Y == h2.Y);
        if (plane == dXZ)
            return (X == h2.X) && (Z == h2.Z);
        if (plane == dYZ)
            return (Z == h2.Z) && (Y == h2.Y);
        return false;
    }
};

/// \brief The control class to display CaloHits Lego
/// \Initialization Initialize Class with TEveElementList to store all the display elements
/// \Usage Explicitly assign the 3D CaloHits(data) to the class and then makeLego()
///        with corresponding Viewer, Scener and its projection plane
class CaloHitsDisplay : public TObject {
public:
    CaloHitsDisplay();

    ~CaloHitsDisplay() override;

    // data vector to store all the input hits
    std::vector<std::vector<CaloHit> > calovec;

    // Not Implemented yet
    vector<TString> name;
    vector<float> E_thre;
    vector<Color_t> color;

    // Explicitly Assign grid value for the following variables
    unsigned xbin = 0;
    double xmin = 0;
    double xmax = 0;
    unsigned ybin = 0;
    double ymin = 0;
    double ymax = 0;
    unsigned zbin = 0;
    double zmin = 0;
    double zmax = 0;

    // makeGrid will recalculate these steps
    // which is the size of lego box
    double delta_x = 0;
    double delta_y = 0;
    double delta_z = 0;

    double scale_factor = 0.1; // scale_factor of the height of Lego box, the smaller the higher
    bool if_log = false; // if to use log scale to display lego box
    bool if_drawLego = true;

    TEveElementList *CaloHitsDisplayList{nullptr};
    TEveElementList *CaloHitsList{nullptr};
    TGListTreeItem *LegoListTree{nullptr};

    // Line Set to contain grids
    TEveStraightLineSet *grid_line{nullptr};

    /// \brief base function to create grid lines given the projection plane
    void makeGrid(ProjectionPlane plane, Color_t grid_color = kWhite);

    /// \brief Core function to display Lego
    /// \arguments v and s are the user-defined TEve elements, which have to be explicitly defined.
    void makeLego(TEveViewer *v, TEveScene *s, ProjectionPlane plane);

    // Clear TListTree
    TEveElementList *getCaloHitsDisplayList() const {
        return CaloHitsDisplayList;
    }

    TGListTreeItem *getLegoListTree() const {
        return LegoListTree;
    }

ClassDefOverride(CaloHitsDisplay, 0);
};


#endif //DSIMU_CALOHITSDISPLAY_H
