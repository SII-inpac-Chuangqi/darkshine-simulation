//
// Created by Zhang Yulei on 9/18/20.
//

#include <iostream>

#include "Core/AlgoManager.h"
#include "Core/ControlManager.h"

#include "Utility/Config.h"

using namespace std;

int main (int argc,char** argv)
{
    auto control = new ControlManager();

    control->setFileName("dp_out.root");
    control->setEvtReader(new EventReader());

    auto algo = new AlgoManager();
    control->setAlgo( algo );
    control->setConfMgr(new ConfigManager("config.txt",algo));
    control->run();

    return 1;
}