//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_ANAMANAGER_H
#define DSIMU_ANAMANAGER_H

#include <map>
#include <vector>
#include <string>

enum STATUS_CODE {success, fail, other};
class AnaManager {
    /*
     *  Base Class of Algorithm
     */

public:
    AnaManager();
    virtual ~AnaManager() = 0;

    /*
     * Workflow of an algo:
     *      begin of run -> [loop of evt: process -> check  ] -> end of run
     */
    virtual void Begin() = 0;
    virtual void ProcessEvt() = 0;
    virtual void CheckEvt() = 0;
    virtual void End() = 0;

protected:
    //std::map<std::string, int>

};


#endif //DSIMU_ANAMANAGER_H
