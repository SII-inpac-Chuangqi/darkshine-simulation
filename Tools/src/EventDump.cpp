#include <iostream>

#include "TKey.h"

#include "EventDump.h"

EventDump::EventDump(TString file_name, TString tree_name) : file_name_(file_name),
                                                             tree_name_(tree_name),
                                                             verbose_(1)
{
    this->CheckFile();
}

void EventDump::CheckFile()
{
    file_ = new TFile(file_name_, "read");
    if(!file_->IsOpen())
        exit(-1);

    tree_ = file_->Get<TTree>(tree_name_);
    if(!tree_)
    {
        std::cerr << "Error No tree " << tree_name_ << " found" << std::endl
                  << "Error Please check the file:" << std::endl;
        this->ListRecursive();
        exit(-1);
    }

    TString branch_name("DEvent"); 
    if(!tree_->FindBranch("DEvent"))
    {
        std::cout << "Error No branch named DEvent found" << std::endl;

        int n_branch = tree_->GetNbranches();
        auto branch_list = tree_->GetListOfBranches();

        for(int i = 0; i < n_branch; i++)
        {
            if((TString)branch_list->At(i)->ClassName() == (TString)"DEvent")
                std::cout << "      Found branch " << branch_list->At(i)->GetName() << "with leaf class DEvent" << std::endl;
        }

        exit(-1);
    }
}

void EventDump::ListRecursive()
{
    TFile *dir = file_;
    this->ListRecursiveTree(dir);
}

void EventDump::ListRecursiveTree(TDirectoryFile *file, unsigned int nindent)
{
    const std::string indent(nindent, ' ');
    for (auto &&keyAsObj : *file->GetListOfKeys())
    {
       auto key = (TKey*)keyAsObj;
       if((TString)key->GetClassName() == (TString)"TTree")
           std::cout << "	" << indent << "TTree: " << key->GetName() << std::endl;
 
       if (auto d = dynamic_cast<TDirectoryFile*>(key->ReadObj()))
           this->ListRecursiveTree(d, nindent + 2);
    }
}

void EventDump::Dump(long long skip_number, long long event_number)
{
    long long start = 0;
    long long  end = tree_->GetEntries();

    if(skip_number >= 0)
    {
        start = skip_number;
        if(event_number > 0)
            end = skip_number + event_number;
    }
    if(end > tree_->GetEntries())
    {
        std::cout << "Warning Request more than exceeds the total number of entries" << std::endl;
        end = tree_->GetEntries();
    }

    DEvent *event = nullptr;
    tree_->SetBranchAddress("DEvent", &event);
    TBranch *branch = tree_->GetBranch("DEvent");

    std::cout << "**************************************************************************************************************************" << std::endl
              << "* File: " << file_name_ << std::endl
              << "* Tree: " << tree_name_ << std::endl
              << "**************************************************************************************************************************" << std::endl
              << "* Dump from event " << start << " to " << end - 1 << std::endl
              << "**************************************************************************************************************************" << std::endl;

    for(long long i = start; i < end; i++)
    {
        branch->GetEntry(i);

        std::cout << "**************************************************************************************************************************"
                  << std::endl
                  << "* Event " << i << ": " << std::endl;

        event->PrintDetails();
    }

}
