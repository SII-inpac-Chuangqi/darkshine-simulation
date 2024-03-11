#ifndef EVENT_DUMP_H
#define EVENT_DUMP_H

#include "TString.h"
#include "TFile.h"
#include "TTree.h"

#include "Object/DEvent.h"

class EventDump {
public:

    EventDump() = delete;

    EventDump(TString file_name, TString tree_name);

    ~EventDump() = default;

    void Dump(long long skip_number = -1, long long event_number = -1);

    static void Help();

private:

    void CheckFile();

    void ListRecursive();

    void ListRecursiveTree(TDirectoryFile *file, unsigned int nindent = 0);

    TFile *file_;
    TTree *tree_;

    TString file_name_;
    TString tree_name_;
    int verbose_{};
};

#endif
