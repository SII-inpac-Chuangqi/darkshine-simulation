//
// Created by Zhang Yulei on 9/18/20.
//

#ifndef DSIMU_DEVENT_H
#define DSIMU_DEVENT_H
#include <cstddef>
#include<cstdlib>

class DEvent
{
public:
    // Constructor
    DEvent() = default;
    virtual ~DEvent() = default;

    // Operators

    // Get Methods

    // Set Methods


private:
    // run number
    int RunID;

    // event number
    int EventID;


};

// Some inline

#endif //DSIMU_DEVENT_H

