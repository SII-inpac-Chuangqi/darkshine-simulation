//
// Created by Zhang Yulei on 9/19/20.
//

#include "ExampleProcessor.h"

void ExampleProcessor::Begin() {
    cout<<"Begin of Processor: "<<this->Name<<endl;
}

void ExampleProcessor::ProcessEvt(DEvent* evt) {
    cout<<"Process # "<<evt->getEventId()<<" event ..."<<endl;

}

void ExampleProcessor::CheckEvt(DEvent* evt) {
    cout<<"Check!"<<endl;

}

void ExampleProcessor::End() {
    cout<<"End!"<<endl;

}


