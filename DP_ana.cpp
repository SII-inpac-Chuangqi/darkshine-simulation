//
// Created by Zhang Yulei on 9/18/20.
//

#include <iostream>

#include "Object/DEvent.h"
using namespace std;

int main (int argc,char** argv)
{
    cout<<"Test program for DAna"<<endl;

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

    return 1;
}