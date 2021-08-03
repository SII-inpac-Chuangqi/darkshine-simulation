#ifndef EVENT_DUMP_H
#define EVENT_DUMP_H

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "Object/DEvent.h"

class EventDump
{
public:

    EventDump(TString file_name, TString tree_name);
    ~EventDump() {}

    EventDump() = delete;

private:

    void CheckFile();
    void ListRecursive();
    void ListRecursiveTree(TDirectoryFile *file, unsigned int nindent = 0);

    void Dump();
    void DumpEvent();

    TFile *file_;
    TTree *tree_;

    TString file_name_;
    TString tree_name_;
    int verbose_;
};

#endif
