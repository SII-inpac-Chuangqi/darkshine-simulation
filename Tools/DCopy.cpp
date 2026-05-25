#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <charconv>

#include "TDirectory.h"
#include "TKey.h"
#include "TFile.h"
#include "TTree.h"

#include "Object/DEvent.h"
#include "Utility/parser.h"

std::vector<int> GetEventIds(const std::string &txt_name)
{
    std::ifstream file(txt_name);
    if (!file) {}

    std::vector<int> numbers;

    std::string line;
    std::string token;

    int number;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        
        while (std::getline(ss, token, ','))
        {
            auto [ptr, ec] = std::from_chars(token.data(), 
                                             token.data() + token.size(), 
                                             number);
            if (ec == std::errc() && ptr == token.data() + token.size())
                numbers.push_back(number);
        }
    }
    
    file.close();

    return numbers; 
}

void RemoveKeys(TDirectory* dir, const std::set<std::string> &keys_to_save)
{
    std::map<std::string, int> count;

    for(auto k_obj : *dir->GetListOfKeys())
    {
        auto *key = dynamic_cast<TKey*>(k_obj);
        std::string name = key->GetName();
        int cycle = key->GetCycle();

        if (keys_to_save.find(name) == keys_to_save.end())
            continue;

        if (count.find(name) == count.end()) count[name] = cycle;
        else if (count[name] < cycle)        count[name] = cycle;
    }

    for(auto k_obj : *dir->GetListOfKeys()) 
    {
        auto *key = dynamic_cast<TKey*>(k_obj);

        std::string name = key->GetName();
        int cycle = key->GetCycle();
        if (!(count.find(name) != count.end() && count[name] == cycle))
        {
            printf("%10s  %20s  %2d   delete\n", key->GetClassName(), name.c_str(), cycle);
            gDirectory->Delete((name + ";" + std::to_string(cycle)).c_str());
        }
        else
        {
            printf("%10s  %20s  %2d   keep  \n", key->GetClassName(), name.c_str(), cycle);
        }
    }
}

int main(int argc, char **argv) {
    std::string file_name = "dp_simu.root";
    std::string tree_name = "Dark_Photon";
    std::string txt_name = "events.txt";
    std::string out_name = "copied_simu.root";
    int max_events = -1;
    int auto_save = 4;

    arg_parser::Parser parser;
    parser.Add("f,file", file_name, "dp_simu.root", "name of the file to copy");
    parser.Add("t,tree", tree_name, "Dark_Photon", "name of the tree storing DEvents");
    parser.Add("x,txt", txt_name, "events.txt", "name of the txt file containing event ids");
    parser.Add("o,output", out_name, "copied_simu.root", "name of the file copied");
    parser.Add("e,event", max_events, -1, "max events to be checked");
    parser.Add("a,auto_save", auto_save, 4, "number of events to be auto-saved");

    parser.Parse(argc, argv);

    auto events = GetEventIds(txt_name);
    std::sort(events.begin(), events.end());
   
//    for (int event : events)
//        std::cout << event << " ";
//    std::cout << std::endl;

    auto input_file = new TFile(file_name.c_str(), "READ");
    auto input_tree = dynamic_cast<TTree*>(input_file->Get(tree_name.c_str()));
    input_tree->Print();
    input_tree->SetBranchStatus("*", true);

    auto output_file = new TFile(out_name.c_str(), "RECREATE");
    output_file->cd();

    auto output_tree = input_tree->CloneTree(0);

    auto matched_dis = 0u;
    auto matched_it = events.begin();
    auto n_entries = (max_events < 0 or max_events > input_tree->GetEntries()) ? input_tree->GetEntries()
                                                                               : max_events;
    int processed_ratio = -1;
    int processing_ratio = 0;
    for (auto i_entry = 0; i_entry < n_entries; i_entry++)
    {
        if (i_entry > events.back()) break;

        processing_ratio = (i_entry == n_entries - 1) ? 100 : 100.*i_entry/n_entries;
        if (processing_ratio % 10 == 0 and processing_ratio != processed_ratio)
        {
            processed_ratio = processing_ratio;
            std::cout << "[INFO] ==> " << processing_ratio << "% events checked" << std::endl; 
        }

        input_tree->GetEntry(i_entry);

        matched_it = std::find(events.begin() + matched_dis, events.end(), i_entry);
        if (matched_it != events.end())
        {
            matched_dis = matched_it - events.begin();

            output_tree->Fill();

            if (matched_dis % auto_save == 1) output_tree->AutoSave("SaveSelf");
        }
    }

    output_tree->Write(tree_name.c_str());
    RemoveKeys(output_file, {tree_name.c_str()});
    output_file->Close();

    input_file->Close();

    return 0;
}
