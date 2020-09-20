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
    cout<<"Test program for DAna"<<endl;

    auto control = new ControlManager();

    /*
    int port;
    std::string ipAddress;
    std::string username;
    std::string password;
    const char ConfigFile[]= "config.txt";
    Config configSettings(ConfigFile);

    port = configSettings.Read("port", 0);
    ipAddress = configSettings.Read("ipAddress", ipAddress);
    username = configSettings.Read("username", username);
    password = configSettings.Read("password", password);
    std::cout<<"port:"<<port<<std::endl;
    std::cout<<"ipAddress:"<<ipAddress<<std::endl;
    std::cout<<"username:"<<username<<std::endl;
    std::cout<<"password:"<<password<<std::endl;
     */

    control->setFileName("dp_out.root");
    control->setEvtReader(new EventReader());

    auto algo = new AlgoManager();
    control->setAlgo( algo );
    control->setConfMgr(new ConfigManager("config.txt",algo));
    control->run();

    return 1;
}