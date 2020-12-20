//
// Created by Zhang Yulei on 12/19/20.
//

#include "MultiView.h"

#include "TEveManager.h"
#include "TEveBrowser.h"
#include "TEveWindow.h"
#include "TEveWindowManager.h"
#include "TGLViewer.h"
#include "TEveProjections.h"
#include "TEveProjectionManager.h"
#include "TEveProjectionAxes.h"

MultiView::MultiView() {

    // Scenes
    //========

    fRPhiGeomScene  = gEve->SpawnNewScene("RPhi Geometry",
                                          "Scene holding projected geometry for the RPhi view.");
    fRhoZGeomScene  = gEve->SpawnNewScene("RhoZ Geometry",
                                          "Scene holding projected geometry for the RhoZ view.");
    fRPhiEventScene = gEve->SpawnNewScene("RPhi Event Data",
                                          "Scene holding projected event-data for the RPhi view.");
    fRhoZEventScene = gEve->SpawnNewScene("RhoZ Event Data",
                                          "Scene holding projected event-data for the RhoZ view.");


    // Projection managers
    //=====================

    fRPhiMgr = new TEveProjectionManager(TEveProjection::kPT_RPhi);
    gEve->AddToListTree(fRPhiMgr, kFALSE);
    {
        auto* a = new TEveProjectionAxes(fRPhiMgr);
        a->SetMainColor(kWhite);
        a->SetTitle("R-Phi");
        a->SetTitleSize(0.05);
        a->SetTitleFont(102);
        a->SetLabelSize(0.025);
        a->SetLabelFont(102);
        fRPhiGeomScene->AddElement(a);
    }

    fRhoZMgr = new TEveProjectionManager(TEveProjection::kPT_RhoZ);
    gEve->AddToListTree(fRhoZMgr, kFALSE);
    {
        auto* a = new TEveProjectionAxes(fRhoZMgr);
        a->SetMainColor(kWhite);
        a->SetTitle("Rho-Z");
        a->SetTitleSize(0.05);
        a->SetTitleFont(102);
        a->SetLabelSize(0.025);
        a->SetLabelFont(102);
        fRhoZGeomScene->AddElement(a);
    }

    // Viewers
    //=========

    TEveWindowSlot *slot = nullptr;
    TEveWindowPack *pack = nullptr;

    slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
    pack = slot->MakePack();
    pack->SetElementName("Multi View");
    pack->SetHorizontal();
    pack->SetShowTitleBar(kFALSE);
    pack->NewSlot()->MakeCurrent();
    f3DView = gEve->SpawnNewViewer("3D View", "");
    f3DView->AddScene(gEve->GetGlobalScene());
    f3DView->AddScene(gEve->GetEventScene());

    pack = pack->NewSlot()->MakePack();
    pack->SetShowTitleBar(kFALSE);
    pack->NewSlot()->MakeCurrent();
    fRPhiView = gEve->SpawnNewViewer("RPhi View", "");
    fRPhiView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
    fRPhiView->AddScene(fRPhiGeomScene);
    fRPhiView->AddScene(fRPhiEventScene);

    pack->NewSlot()->MakeCurrent();
    fRhoZView = gEve->SpawnNewViewer("RhoZ View", "");
    fRhoZView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
    fRhoZView->AddScene(fRhoZGeomScene);
    fRhoZView->AddScene(fRhoZEventScene);
}
