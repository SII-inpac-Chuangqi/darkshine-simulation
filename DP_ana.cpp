//
// Created by Zhang Yulei on 9/18/20.
//

#include <iostream>

#include "Object/SimulatedHit.h"
using namespace std;

int main (int argc,char** argv)
{
    cout<<"Test program for DAna"<<endl;

    auto a = new CalorimeterHit();
    a->setCellId(123);
    auto b = *a;
    cout<<b.getCellId()<<endl;

    return 1;
}