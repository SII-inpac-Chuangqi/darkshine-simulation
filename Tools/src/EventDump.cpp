#include <iostream>

#include "TKey.h"

#include "EventDump.h"

EventDump::EventDump(TString file_name, TString tree_name) : file_name_(file_name),
                                                             tree_name_(tree_name),
                                                             verbose_(1) {
    this->CheckFile();
}

void EventDump::CheckFile() {
    file_ = new TFile(file_name_, "read");
    if (!file_->IsOpen())
        exit(-1);

    tree_ = file_->Get<TTree>(tree_name_);
    if (!tree_) {
        std::cerr << "Error No tree " << tree_name_ << " found" << std::endl
                  << "Error Please check the file:" << std::endl;
        this->ListRecursive();
        exit(-1);
    }

    TString branch_name("DEvent");
    if (!tree_->FindBranch("DEvent")) {
        std::cout << "Error No branch named DEvent found" << std::endl;

        int n_branch = tree_->GetNbranches();
        auto branch_list = tree_->GetListOfBranches();

        for (int i = 0; i < n_branch; i++) {
            if ((TString) branch_list->At(i)->ClassName() == (TString) "DEvent")
                std::cout << "      Found branch " << branch_list->At(i)->GetName() << "with leaf class DEvent"
                          << std::endl;
        }

        exit(-1);
    }
}

void EventDump::ListRecursive() {
    TFile *dir = file_;
    this->ListRecursiveTree(dir);
}

void EventDump::ListRecursiveTree(TDirectoryFile *file, unsigned int nindent) {
    const std::string indent(nindent, ' ');
    for (auto &&keyAsObj : *file->GetListOfKeys()) {
        auto key = (TKey *) keyAsObj;
        if ((TString) key->GetClassName() == (TString) "TTree")
            std::cout << "	" << indent << "TTree: " << key->GetName() << std::endl;

        if (auto d = dynamic_cast<TDirectoryFile *>(key->ReadObj()))
            this->ListRecursiveTree(d, nindent + 2);
    }
}

void EventDump::Dump(long long skip_number, long long event_number) {
    long long start = 0;
    long long end = tree_->GetEntries();

    if (skip_number >= 0)
        start = skip_number;
    if (event_number >= 0)
        end = start + event_number;
    if (end > tree_->GetEntries()) {
        std::cout << "[Warning] ==> Requested events exceed the total number of entries" << std::endl;
        end = tree_->GetEntries();
    }

    if (end < start) {
        std::cout << "[Warning] ==> Requested events from " << start << " to " << end << std::endl;
        return;
    }

    if (end - start >= 100) {
        TString confirm;
        std::cout << "[Warning] ==> Requested more than 100 events, are you sure? [Y/y] ";
        std::cin  >> confirm;
        std::cout << " " << std::endl;
        confirm.ToLower();
        if (confirm != "y" && confirm != "yes") return;
    }

    DEvent *event = nullptr;
    tree_->SetBranchAddress("DEvent", &event);
    TBranch *branch = tree_->GetBranch("DEvent");

    std::cout
            << "***********************************************************************************************************************"
            << std::endl
            << "* File: " << file_name_ << std::endl
            << "* Tree: " << tree_name_ << std::endl
            << "***********************************************************************************************************************"
            << std::endl
            << (event_number == 1 ? TString::Format("* Dump event %lld", start) : TString::Format("* Dump from event %lld to %lld", start, end - 1)) << std::endl
            << "***********************************************************************************************************************"
            << std::endl;

    for (long long i = start; i < end; i++) {
        branch->GetEntry(i);

        std::cout
                << "***********************************************************************************************************************"
                << std::endl
                << "* Event " << i << ": " << std::endl;

        event->PrintDetails();
    }

}

void EventDump::Help() {
    std::cout << "Dark Shine event dump:" << std::endl
              << "DDump -h: help" << std::endl
              << "DDump -f [file name] -t [tree name] -j [skip number(optional)] -e [event number(optional)]" << std::endl
              << "    file name: file to dump" << std::endl
              << "    tree name: tree in file which contains a branch of DEvent" << std::endl
              << "    skip number(optional): not set or < 0, dump all events; >= 0, skip events from 0 to skip number"
              << std::endl
              << "    event number(optional): when skip number >= 0, not set or < 0, dump all events unskipped; >= 0, dump events from skip number to skip number + event number"
              << std::endl;
}
