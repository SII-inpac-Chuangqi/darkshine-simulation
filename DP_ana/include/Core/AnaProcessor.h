//
// Created by Zhang Yulei on 9/19/20.
//

#ifndef DSIMU_ANAPROCESSOR_H
#define DSIMU_ANAPROCESSOR_H

#include <map>
#include <utility>
#include <vector>
#include <string>
#include <iostream>

#include "Object/DEvent.h"

using namespace std;

enum STATUS_CODE {
    success, fail, other
};

class AnaProcessor {
    /*
     *  Base Class of Algorithm
     */

public:
    AnaProcessor() = default;
    explicit AnaProcessor(string name) : Name(std::move(name)) {};
    virtual ~AnaProcessor() = default;

    /*
     * Workflow of an algo:
     *      begin of run -> [loop of evt: process -> check  ] -> end of run
     */

    virtual void Begin() = 0;

    virtual void ProcessEvt(DEvent*) = 0;

    virtual void CheckEvt(DEvent*) = 0;

    virtual void End() = 0;

    // Get Methods
    const std::string &getName() const {
        return Name;
    }

    const std::map<std::string, std::pair<std::string, int*>> &getIntParameters() const {
        return IntParameters;
    }

    const std::map<std::string, std::pair<std::string, double*>> &getDoubleParameters() const {
        return DoubleParameters;
    }

    const std::map<std::string, std::pair<std::string, std::string*>> &getStringParameters() const {
        return StringParameters;
    }

    // Set Methods
    void setName(const string &name) {
        Name = name;
    }

    void setIntValue(const std::string &name, int value);

    void setDoubleValue(const std::string &name, double value);

    void setStringValue(const std::string &name, const std::string &value);


    // Register Parameters
    void RegisterIntParameter(const std::string &name, const std::string &description, int *address, int default_value);

    void RegisterDoubleParameter(const std::string &name, const std::string &description, double *address,
                                 double default_value);

    void RegisterStringParameter(const std::string &name, const std::string &description, std::string *address,
                                 std::string default_value);

protected:
    // Algo Name
    string Name;

    // Parameter Name, Description, Value
    std::map<std::string, std::pair<std::string, int*> > IntParameters;
    std::map<std::string, std::pair<std::string, double*> > DoubleParameters;
    std::map<std::string, std::pair<std::string, std::string*> > StringParameters;

};

#endif //DSIMU_ANAPROCESSOR_H
