//
// Created by Zhang Yulei on 12/19/20.
//

#ifndef DSIMU_MULTIVIEW_H
#define DSIMU_MULTIVIEW_H

#include "TEveManager.h"
#include "TGeoManager.h"
#include "TEveProjectionManager.h"
#include "TApplication.h"
#include "TString.h"
#include "TNamed.h"

#include "TEveViewer.h"
#include "TEveScene.h"

class MultiView {
public:
    MultiView();

    virtual ~MultiView() = default;

    //---------------------------------------------------------------------------

    void ImportGeomRPhi(TEveElement *el) {
        fRPhiMgr->ImportElements(el, fRPhiGeomScene);
    }

    void ImportGeomRhoZ(TEveElement *el) {
        fRhoZMgr->ImportElements(el, fRhoZGeomScene);
    }

    void ImportEventRPhi(TEveElement *el) {
        fRPhiMgr->ImportElements(el, fRPhiEventScene);
    }

    void ImportEventRhoZ(TEveElement *el) {
        fRhoZMgr->ImportElements(el, fRhoZEventScene);
    }

    //---------------------------------------------------------------------------

    void DestroyEventRPhi() {
        fRPhiEventScene->DestroyElements();
    }

    void DestroyEventRhoZ() {
        fRhoZEventScene->DestroyElements();
    }

private:
    /**********/
    /* viewer */
    /**********/
    TEveViewer *f3DView{nullptr};
    TEveViewer *fRPhiView{nullptr};
    TEveViewer *fRhoZView{nullptr};

    TEveScene *fRPhiGeomScene{nullptr};
    TEveScene *fRhoZGeomScene{nullptr};
    TEveScene *fRPhiEventScene{nullptr};
    TEveScene *fRhoZEventScene{nullptr};

    TEveProjectionManager *fRPhiMgr{nullptr};
    TEveProjectionManager *fRhoZMgr{nullptr};
};


#endif //DSIMU_MULTIVIEW_H
