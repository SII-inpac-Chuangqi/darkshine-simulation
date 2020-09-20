//
// Created by Zhang Yulei on 9/18/20.
//

#include <iostream>

#include "Object/DEvent.h"
#include "Core/AlgoManager.h"
#include "Core/ControlManager.h"
#include "Utility/Config.h"

#include "ExampleProcessor.h"

using namespace std;

int main (int argc,char** argv)
{
    cout<<"Test program for DAna"<<endl;

    auto control = new ControlManager();

    auto a = new DEvent();
    a->RegisterMCParticleCollection("test1");
    a->RegisterMCParticleCollection("test2");
    a->RegisterRecParticleCollection("test3");

    a->DeleteCollection("test1");

    for ( const auto& t : *a->ListAllCollections() )
        std::cout<<t<<std::endl;

    a->Initialization();

    for ( const auto& t : *a->ListAllCollections() )
        std::cout<<t<<std::endl;

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


    auto c = new ExampleProcessor(std::string("std"));
    c->RegisterIntParameter("p1","p1 description",port,port);
    c->RegisterStringParameter("p2","p2 description",ipAddress,ipAddress);
    c->setStringValue("p2","change");

    auto d = new AlgoManager();
    d->RegisterAnaProcessor(c);

    control->setFileName("dp_out.root");
    control->setEvtReader(new EventReader);
    control->setAlgo(d);
    control->setEventNumber(-1);
    control->run();

    return 1;
}