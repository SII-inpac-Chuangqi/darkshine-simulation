//
// Created by Zhang Yulei on 12/22/20.
//

//-----------
// Processor

//void DEventDisplay::RunAnaProcessors() {
//    auto *AnaList = new TEveElementList("AnaProcessor Lists");
//    gEve->AddElement(AnaList);
//
//    // Initialize the Processors
//    auto recECAL = shared_ptr<RecECAL>(new RecECAL("RecECAL", nullptr));
//
//    *((recECAL->getIntParameters()).at("Verbose").second) = 1;
//    *((recECAL->getDoubleParameters()).at("W0").second) = RecECAL_W0;
//    *((recECAL->getDoubleParameters()).at("d_cut").second) = 0.05;
//    *((recECAL->getDoubleParameters()).at("r_cut").second) = RecECAL_r_cut;
//
//    // begin() & ProcessEvt
//    if (RecECAL_) {
//        recECAL->Begin();
//        recECAL->ProcessEvt(evt.get());
//
//        /************************************/
//        /*    Visualization Out Collection  */
//        /************************************/
//        auto CaloCols = evt->getCalorimeterHitCollection();
//        for (const auto &CALCol : CaloCols) {
//            // only count hits in calorimeter
//            if (!TString(CALCol.first).Contains("Cluster_Center")) continue;
//            if (CALCol.second->empty()) continue;
//            auto *SimHits = new TEveElementList(CALCol.first.data());
//            AnaList->AddElement(SimHits);
//            auto CALs = CALCol.second;
//            std::cout << "[Event Display] ==> Draw Collection: " << CALCol.first;
//            std::cout << ", with total hits: " << CALs->size() << std::endl;
//            if (CALs->empty()) continue;
//            double EMax = 0.;
//            for (auto cal_hit : *CALs)
//                EMax = (cal_hit->getE() > EMax) ? cal_hit->getE() : EMax;
//
//            for (auto cal_hit : *CALs) {
//                //auto test = new CalorimeterHit;
//                auto *box = makeRecCaloBox(cal_hit, EMax);
//                SimHits->AddElement(reinterpret_cast<TEveElement *>(box));
//            }
//        }
//    }
//}
//
//void DEventDisplay::makeGUIProcessor(DEventDisplay *fh) {
//    TEveBrowser *browser = gEve->GetBrowser();
//
//    TGLabel *lbl = nullptr;
//    TGTextButton *tb = nullptr;
//
//    browser->StartEmbedding(TRootBrowser::kLeft);
//    auto *frmMain1 = new TGMainFrame(gClient->GetRoot(), 1200, 800);
//    frmMain1->SetWindowName("XX GUI");
//    frmMain1->SetCleanup(kDeepCleanup);
//    TGHorizontalFrame *hf = nullptr;
//    // RecECAL
//    {
//        hf = new TGHorizontalFrame(frmMain1);
//        {
//            lbl = new TGLabel(hf, "RecECAL");
//            hf->AddFrame(lbl);
//        }
//        frmMain1->AddFrame(hf);
//        hf = new TGHorizontalFrame(frmMain1);
//        {
//            guiRecECAL = new TGCheckButton(hf, "Run RecECAL");
//            //if (guiRecECAL) guiRecECAL->Toggle();
//            hf->AddFrame(guiRecECAL);
//            guiRecECAL->Connect("Toggled(Bool_t)", "DEventDisplay", fh, "guiOptionsAna()");
//        }
//        frmMain1->AddFrame(hf);
//        hf = new TGHorizontalFrame(frmMain1);
//        {
//            // evt number entry
//            guiRecECAL_W0 = new TGNumberEntry(hf, RecECAL_W0, 9, 999, TGNumberFormat::kNESReal,
//                                              TGNumberFormat::kNEAAnyNumber,
//                                              TGNumberFormat::kNELLimitMinMax,
//                                              -100., 100.);
//            hf->AddFrame(guiRecECAL_W0);
//            guiRecECAL_W0->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
//            lbl = new TGLabel(hf, " W0");
//            hf->AddFrame(lbl);
//        }
//        frmMain1->AddFrame(hf);
//        hf = new TGHorizontalFrame(frmMain1);
//        {
//            // evt number entry
//            guiRecECAL_r_cut = new TGNumberEntry(hf, RecECAL_r_cut, 9, 999, TGNumberFormat::kNESReal,
//                                                 TGNumberFormat::kNEAAnyNumber,
//                                                 TGNumberFormat::kNELLimitMinMax,
//                                                 -1., 1.);
//            hf->AddFrame(guiRecECAL_r_cut);
//            guiRecECAL_r_cut->Connect("ValueSet(Long_t)", "DEventDisplay", fh, "guiOptionsAna()");
//            lbl = new TGLabel(hf, " r_cut");
//            hf->AddFrame(lbl);
//        }
//        frmMain1->AddFrame(hf);
//    }// end of RecECAL
//
//    frmMain1->MapSubwindows();
//    frmMain1->Resize();
//    frmMain1->MapWindow();
//
//    browser->StopEmbedding();
//    browser->SetTabTitle("Ana Processors", 0);
//}
//
//[[maybe_unused]] void DEventDisplay::guiOptionsAna() {
//    // Ana Processors
//    RecECAL_ = guiRecECAL->IsOn();
//    RecECAL_W0 = guiRecECAL_W0->GetNumberEntry()->GetNumber();
//    RecECAL_r_cut = guiRecECAL_r_cut->GetNumberEntry()->GetNumber();
//
//    gotoEvent(_eventID);
//}
