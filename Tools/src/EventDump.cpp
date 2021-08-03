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
        std::cerr << "ERROR	No tree " << tree_name_ << " found" << std::endl
                  << "ERROR	Please check the file:" << std::endl;
        this->ListRecursive();
        exit(-1);
    }
}

void EventDump::ListRecursive()
{
    TFile* dir = file_;
    this->ListRecursiveTree(dir);
}

void EventDump::ListRecursiveTree(TDirectoryFile *file, unsigned int nindent)
{
   const std::string indent(nindent, ' ');
   for (auto &&keyAsObj : *file->GetListOfKeys())
   {
      auto key = (TKey *)keyAsObj;
      if(key->GetClassName() == (TString)"TTree")
          std::cout << "	" << indent << "TTree: " << key->GetName() << std::endl;

      if (auto d = dynamic_cast<TDirectoryFile*>(key->ReadObj()))
          this->ListRecursiveTree(d, nindent + 2);
   }
}

void EventDump::Dump()
{
}

void EventDump::DumpEvent()
{
}
