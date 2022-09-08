//
// Created by zhuxu on 2021/7/29.
//

#include <iostream>
#include "Core/AnaData.h"

#ifdef RM_UNIT
#define CUNIT 1
#else
#define CUNIT 10
#endif

// Required by Singleton
AnaData *dAnaData = nullptr;

// Get Instance Class
AnaData *AnaData::CreateInstance() {
    if (dAnaData == nullptr)
        dAnaData = new AnaData();
    return dAnaData;
}

AnaData::AnaData() {
    processMap["__max"]=0;
    rev_processMap[0]="Unknown";
}

void AnaData::ReadMagField() {
    if (! root_file->IsOpen() ) {
        std::cerr << "[READFILE ERROR] ==> InputGeoFile does not exist." << std::endl;
        exit(1);
    }

    if (! root_file->Get("magnet0") ) {
        std::cerr << "[READFILE WARNING] ==> Magnet x not found" << std::endl;
        return;
    }
    if (! root_file->Get("magnet1") ) {
        std::cerr << "[READFILE WARNING] ==> Magnet y not found" << std::endl;
        return;
    }
    if (! root_file->Get("magnet2") ) {
        std::cerr << "[READFILE WARNING] ==> Magnet z not found" << std::endl;
        return;
    }

    mag_field_vec = std::vector<DMagnet*>({dynamic_cast<DMagnet*>(root_file->Get("magnet0")),
                                           dynamic_cast<DMagnet*>(root_file->Get("magnet1")),
                                           dynamic_cast<DMagnet*>(root_file->Get("magnet2"))});
}

void AnaData::setConstMagnetField(const vector<double> &const_value)
{
    if_const_field_ = true;

    if(const_value.size() != 3)
    {
        std::cerr << "[WARNING] ==> Input const field size not compatible, "
                  << "input size: " << const_value.size() << ", "
                  << "required size: 3" << std::endl;
        return;
    }

    const_mag_field_vec.reserve(3);
    const_mag_field_vec.push_back(const_value.at(0));
    const_mag_field_vec.push_back(const_value.at(1));
    const_mag_field_vec.push_back(const_value.at(2));
}

const vector<double> AnaData::getMagnetFieldAt(const vector<double> &pos) const
{
    if(if_const_field_) return const_mag_field_vec;

    vector<double> field = {RETURN, RETURN, RETURN};
    field.at(0) = mag_field_vec.at(0)->GetField(pos.at(0), pos.at(1), pos.at(2));
    field.at(1) = mag_field_vec.at(1)->GetField(pos.at(0), pos.at(1), pos.at(2));
    field.at(2) = mag_field_vec.at(2)->GetField(pos.at(0), pos.at(1), pos.at(2));

    return field;
}

void AnaData::readGeometryDetails() {
    world_ = dynamic_cast<TGeoNode*>(gGeoManager->GetListOfNodes()->At(0));
    if(!world_) {
        std::cerr << "[WARNING] ==> No world node ..." << std::endl;
        return;
    }

    layer_width_tag.clear();
    layer_length_tag.clear();
    strip_no_tag.clear();
    angles_tag.clear();

    layer_width_rec.clear();
    layer_length_rec.clear();
    strip_no_rec.clear();
    angles_rec.clear();

    ECal_cell_length_x.clear();
    ECal_cell_length_y.clear();
    ECal_cell_length_z.clear();

    N_ECal_cell_x = 0;
    N_ECal_cell_y = 0;
    N_ECal_cell_z = 0;
    double last_pos[3] = {-INFINITY, -INFINITY, -INFINITY};

    for (int i = 0; i < world_->GetNdaughters(); ++i) {
        auto *detector = dynamic_cast<TGeoNode*>(world_->GetDaughter(i));
        auto detector_name = TString(detector->GetVolume()->GetName());

        if(detector_name.Contains("Trk")) { // TAGTrk or RECTrk
            for(int j = 0; j < detector->GetNdaughters(); j++) {
                auto *layer = dynamic_cast<TGeoNode*>(detector->GetDaughter(j));
                auto *layer_shape = dynamic_cast<TGeoBBox*>(layer->GetVolume()->GetShape());
                auto layer_name = TString(layer->GetVolume()->GetName());
                auto rotation = layer->GetMatrix()->GetRotationMatrix();

                if(layer_name.Contains("Tag")) { // TagTrk1_LV or TagTRk2_LV
                    layer_width_tag.push_back(2.*CUNIT*layer_shape->GetDX());
                    layer_length_tag.push_back(2.*CUNIT*layer_shape->GetDY());
                    auto *block0 = dynamic_cast<TGeoNode*>(layer->GetDaughter(0));
                    strip_no_tag.push_back(layer->GetNdaughters() * block0->GetNdaughters());
                    angles_tag.push_back(std::asin(rotation[1]));
                }
                else if(layer_name.Contains("Rec")) {
                    layer_width_rec.push_back(2.*CUNIT*layer_shape->GetDX());
                    layer_length_rec.push_back(2.*CUNIT*layer_shape->GetDY());
                    auto *block0 = dynamic_cast<TGeoNode*>(layer->GetDaughter(0));
                    strip_no_rec.push_back(layer->GetNdaughters() * block0->GetNdaughters());
                    angles_rec.push_back(std::asin(rotation[1]));
                }
            }
        }

        if(detector_name.Contains("ECAL")) {
            auto *detector_shape = dynamic_cast<TGeoBBox*>(detector->GetVolume()->GetShape());
            ECAL_center_x = CUNIT*detector->GetMatrix()->GetTranslation()[0];
            ECAL_center_y = CUNIT*detector->GetMatrix()->GetTranslation()[1];
            ECAL_center_z = CUNIT*detector->GetMatrix()->GetTranslation()[2];
            ECAL_length_x = CUNIT*2*detector_shape->GetDX();
            ECAL_length_y = CUNIT*2*detector_shape->GetDY();
            ECAL_length_z = CUNIT*2*detector_shape->GetDZ();
            double subdetector_pos[3];

            // Setting EECal_cell_xyz
            for (int j = 0; j < detector->GetNdaughters(); ++j) {
                auto *block = dynamic_cast<TGeoNode*>(detector->GetDaughter(j));
                auto block_name = TString(block->GetVolume()->GetName());
                for (int k = 0; k < block->GetNdaughters(); ++k ) {
                    auto *subdetector = dynamic_cast<TGeoNode*>(block->GetDaughter(k));
                    auto subdetector_name = TString(subdetector->GetVolume()->GetName());

                    if (subdetector_name.Contains("LVW")) {
                        auto *crystal = dynamic_cast<TGeoNode*>(subdetector->GetDaughter(0));
                        auto *crystal_shape = dynamic_cast<TGeoBBox*>(crystal->GetVolume()->GetShape());
                        ECal_cell_length_x.push_back(CUNIT*2*crystal_shape->GetDX());
                        ECal_cell_length_y.push_back(CUNIT*2*crystal_shape->GetDY());
                        ECal_cell_length_z.push_back(CUNIT*2*crystal_shape->GetDZ());

                        for (int l = 0; l < 3; l++)
                            subdetector_pos[l] = subdetector->GetMatrix()->GetTranslation()[l] + block->GetMatrix()->GetTranslation()[l];
                        if ( !ECAL_pos0 ) ECAL_pos0 = new TVector3(subdetector_pos[0],subdetector_pos[1],subdetector_pos[2]);

                        if (subdetector_pos[2] > last_pos[2]) {
                            if (ECAL_cell_dz == 0 && N_ECal_cell_z == 1) ECAL_cell_dz = fabs(subdetector_pos[2] - last_pos[2]);
                            last_pos[2] = subdetector_pos[2];
                            N_ECal_cell_z++;
                        }
                        if (subdetector_pos[1] > last_pos[1]) {
                            if (ECAL_cell_dy == 0 && N_ECal_cell_y == 1) ECAL_cell_dy = fabs(subdetector_pos[1] - last_pos[1]);
                            last_pos[1] = subdetector_pos[1];
                            N_ECal_cell_y++;
                        }
                        if (subdetector_pos[0] > last_pos[0]) {
                            if (ECAL_cell_dx == 0 && N_ECal_cell_x == 1) ECAL_cell_dx = fabs(subdetector_pos[0] - last_pos[0]);
                            last_pos[0] = subdetector_pos[0];
                            N_ECal_cell_x++;
                        }
                    }
                }
            }

            int ecal_n = 0;
            // Calculate ECAL_posmap
            for (int zi = 0; zi < N_ECal_cell_z; zi++) {
                for (int yi = 0; yi < N_ECal_cell_y; yi++) {
                    for (int xi = 0; xi < N_ECal_cell_x; xi++) {
                        ECAL_posmap[ecal_n] = TVector3( ECAL_pos0->x() + xi * ECAL_cell_dx,
                                                    ECAL_pos0->y() + yi * ECAL_cell_dy,
                                                    ECAL_pos0->z() + zi * ECAL_cell_dz);
                        ecal_n++;
                    }
                }
            }
        }
    }
}

void AnaData::printGeometryDetails() const {
    std::cerr << "[INFO] ==> Geometry details:" << std::endl;
    if(strip_no_tag.size() && layer_width_tag.size() && layer_length_tag.size() &&
       strip_no_rec.size() && layer_width_rec.size() && layer_length_rec.size()   ) {
        std::cerr << "           Tag tracker: strip No.    ";
        for(size_t i = 0; i < strip_no_tag.size();     i += 2) std::cerr << strip_no_tag.at(i) << ", ";
        std::cerr << std::endl;
        std::cerr << "                        layer width  ";
        for(size_t i = 0; i < layer_width_tag.size();  i += 2) std::cerr << layer_width_tag.at(i)  << " mm, ";
        std::cerr << std::endl;
        std::cerr << "                        layer length ";
        for(size_t i = 0; i < layer_length_tag.size(); i += 2) std::cerr << layer_length_tag.at(i) << " mm, ";
        std::cerr << std::endl;

        std::cerr << "           Rec tracker: strip No.    ";
        for(size_t i = 0; i < strip_no_rec.size();     i += 2) std::cerr << strip_no_rec.at(i) << ", ";
        std::cerr << std::endl;
        std::cerr << "                        layer width  ";
        for(size_t i = 0; i < layer_width_rec.size();  i += 2) std::cerr << layer_width_rec.at(i)  << " mm, ";
        std::cerr << std::endl;
        std::cerr << "                        layer length ";
        for(size_t i = 0; i < layer_length_rec.size(); i += 2) std::cerr << layer_length_rec.at(i) << " mm, ";
        std::cerr << std::endl;
    }
    if(ECal_cell_length_x.size() && ECal_cell_length_y.size() && ECal_cell_length_z.size())
        std::cerr << "           ECal:        center x at  " << ECAL_center_x            << " mm" << std::endl
                  << "                        center y at  " << ECAL_center_y            << " mm" << std::endl
                  << "                        center z at  " << ECAL_center_z            << " mm" << std::endl
                  << "                        length x     " << ECAL_length_x            << " mm" << std::endl
                  << "                        length y     " << ECAL_length_y            << " mm" << std::endl
                  << "                        length z     " << ECAL_length_z            << " mm" << std::endl
                  << "                        cell size x  " << ECal_cell_length_x.at(0) << " mm" << std::endl
                  << "                        cell size y  " << ECal_cell_length_y.at(0) << " mm" << std::endl
                  << "                        cell size z  " << ECal_cell_length_z.at(0) << " mm" << std::endl
                  << "                        cell[0] pos x  "  << ECAL_posmap.at(0).X()    << " mm" << std::endl
                  << "                        cell[0] pos y  "  << ECAL_posmap.at(0).Y()    << " mm" << std::endl
                  << "                        cell[0] pos z  "  << ECAL_posmap.at(0).Z()    << " mm" << std::endl
                  << "                        cell No. x   " << N_ECal_cell_x            << std::endl
                  << "                        cell No. y   " << N_ECal_cell_y            << std::endl
                  << "                        cell No. z   " << N_ECal_cell_z            << std::endl;
}

int AnaData::getProcessId(const std::string& n){
    //find the processName corresponding id or add a new one
    if(processMap.count(n)==0){
        processMap[n]=processMap["__max"]+1;
        processMap["__max"]+=1;
        rev_processMap[processMap["__max"]]=n;
        // std::cerr << "[DEBUG] ==> process added "<<n<<std::endl;
    }
    // return processMap[n]; //use arb. id
    return static_cast<int>(std::hash<std::string>{}(n)); //now use hash
}

void AnaData::printProcessMap(){
    std::cerr << "[INFO] ==> ProcessMap details: " << processMap["__max"] << std::endl;
    for(int i=1;i<=processMap["__max"];i++)
        std::cerr <<i<<" : "<<rev_processMap[i]<<" -> "<< static_cast<int>(std::hash<std::string>{}(rev_processMap[i])) << std::endl;
}

void AnaData::LoadTruthInfo(DTruth *truth)
{
    if(!truth) {std::cerr << "[WARNING] ==> No truth info found in event" << std::endl; truth_ = nullptr;}
    else       truth_ = truth;
}

void AnaData::PrintTruthInfo() const
{
    if(!truth_) std::cerr << "[WARNING] ==> No truth info found in event" << std::endl;
    else        truth_->printTruthTopology();
    //else        truth_->printTruthTracks();
}

std::vector<std::pair<const DTruthState*, int>> AnaData::getTruthTracksAtECalFront() const
{
    auto tracker_keys = truth_->getTracksKey(DTruth::DTruthDetPV::All);
    auto ECal_states  = truth_->getStatesInECAL();

    std::vector<std::pair<const DTruthState*, int>> truth_tracks_at_ECal_front;
    for(auto key : tracker_keys)
    {
        auto if_in_ECal = ECal_states.find(key);
        if(if_in_ECal != ECal_states.end())
            truth_tracks_at_ECal_front.push_back(std::make_pair(if_in_ECal->second.first,(int)key.second));
    }

    return truth_tracks_at_ECal_front;
}

std::vector<std::pair<const DTruthParticle*, const DTruthState*>> AnaData::getTruthsAtECalFront() const {
    std::vector<std::pair<const DTruthParticle*, const DTruthState*>> v_truthStateParticle;
    // m_states is map<{trackID, PDG}, {prev_state, post_state}>
    map<pair<int, int>, pair<DTruthState *, DTruthState *>> m_states = truth_->getStatesInECAL();
    // scan DTruthParticles
    for (auto tp : truth_->getTruthParticles()) {
        auto state_key = std::make_pair(tp->id, tp->pdg);
        auto itr_states = m_states.find(state_key);
        if (itr_states != m_states.end()) {
            v_truthStateParticle.emplace_back(std::make_pair(tp, itr_states->second.first));
            //std::cout << "[AnaData] ==> Truths at " << " StateZ: " << itr_states->second.first->vertex[2] << " id: " << tp->id << " pdg: " << tp->pdg << std::endl;
        }
    }
    return v_truthStateParticle;
}

unsigned int AnaData::getNTruthTracks(DTruth::DTruthDetPV DetPV, double min_energy, int min_hits) const
{
    unsigned int n_track(0);

    if(truth_) n_track = truth_->getTracksInRegion(DetPV, min_energy, min_hits).size();
    return n_track;
}

const DTruth* AnaData::getInitialElectron() const
{
    return nullptr;
}

/*
void AnaData::LoadTruthMcPHelper(const MCPHelperMap &helper_collection)
{
    if(!helper_collection.size() || helper_collection.find("MCPHelper") == helper_collection.end())
        std::cerr << "[WARNING] ==> No McPHelper to load" << std::endl;
    else
        helper = helper_collection.at("MCPHelper");
}

void AnaData::PrintTruthMcPHelper() const
{
    if(!helper) std::cerr << "[WARNING] ==> No McPHelper to print" << std::endl;
    else
    {
        std::cout << "| ID    | PDG    |  Px[MeV]   Py[MeV]   Pz[MeV] |   E[MeV]  Mass[MeV] |    x[mm]     y[mm]     z[mm] | PVID   CellID | MCPID  |"
                  << std::endl;
        for(const auto &particle : *helper) std::cout << *particle << std::endl;
        std::cout << "*******************************************************************************************************************************"
                  << std::endl;
    }
}

const McPHelper* AnaData::getInitialElectron() const
{
    if(!helper)
    {
        std::cerr << "[WARNING] ==> No McPHelper to get" << std::endl;
        return nullptr;
    }

    int n_particle = -1;
    double energy = -INFINITY;
    for(auto particle = helper->begin(); particle != helper->end(); ++particle)
    {
        if((*particle)->getId() == 1 && (*particle)->getE() > energy)
        {
            energy = (*particle)->getE();
            n_particle = particle - helper->begin();
        }
    }

    if(n_particle >= 0) return helper->at(n_particle);

    return nullptr;
}
*/
