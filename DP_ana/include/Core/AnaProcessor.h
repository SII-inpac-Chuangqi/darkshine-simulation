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

#include "Event/AnaEvent.h"
#include "Event/EventStoreAndWriter.h"
#include "Core/DataHandle.h"

using namespace std;

enum STATUS_CODE {
    success, fail, other
};

class AnaProcessor {
    /*
     *  Base Class of Algorithm
     */
private:
    using data_handles_t = std::map<std::string, DataHandleBase*>;

public:
    AnaProcessor() = default;

    explicit AnaProcessor(std::string name, std::shared_ptr<EventStoreAndWriter> evtwrt) : Name(std::move(name)) , EvtWrt(std::move(evtwrt)) {};

    virtual ~AnaProcessor() = default;

    /*
     * Workflow of an algo:
     *      begin of run -> [loop of evt: process -> check  ] -> end of run
     */

    virtual void Begin() = 0;

    virtual void InitEvt() = 0;

    virtual void ProcessEvt(AnaEvent *) = 0;

    virtual void CheckEvt(AnaEvent *) = 0;

    virtual void End() = 0;

    // Get Methods
    const std::string &getName() const {
        return Name;
    }

    const std::map<std::string, std::pair<std::string, int *>> &getIntParameters() const {
        return IntParameters;
    }

    const std::map<std::string, std::pair<std::string, double *>> &getDoubleParameters() const {
        return DoubleParameters;
    }

    const std::map<std::string, std::pair<std::string, std::string *>> &getStringParameters() const {
        return StringParameters;
    }

    int getVerbose() const {
        return Verbose;
    }

    virtual const std::string &getDescription() const {
        return Description;
    }

    // Set Methods
    void setName(const std::string &name) {
        Name = name;
    }

    void setVerbose(int verbose) {
        Verbose = verbose;
    }

    void setDescription(const std::string &description) {
        Description = description;
    }

    void setIntValue(const std::string &name, int value);

    void setDoubleValue(const std::string &name, double value);

    void setStringValue(const std::string &name, const std::string &value);


    // Register Parameters
    void RegisterIntParameter(const std::string &name, const std::string &description, int *address, int default_value);

    void RegisterUIntParameter(const std::string &name, const std::string &description, unsigned int *address, unsigned int default_value);

    void RegisterDoubleParameter(const std::string &name, const std::string &description, double *address,
                                 double default_value);

    void RegisterStringParameter(const std::string &name, const std::string &description, std::string *address,
                                 std::string default_value);

    // Operators
    friend std::ostream &operator<<(std::ostream &os, const AnaProcessor &processor);

    void setIsDisplay(bool v=true){is_display=v;};
    bool getIsDisplay(){return is_display;};

    void linkDataHandles(data_handles_t* data_handles) {data_handles_ = data_handles;}

    template<class data_t>
    void insertSharedData(std::string key, const data_t &data, std::string description = "")
    {
        if(!datahandlesDuplicatedCheck(key)) return;

        (*data_handles_)[key] = new DataHandle<data_t>(key, data, description);
    }

    template<class data_t>
    data_t* createSharedData(std::string key)
    {
        if(!datahandlesEmptyCheck(key)) return nullptr;

        return dynamic_cast<DataHandle<data_t>*>(data_handles_->at(key))->Get();
    }

    template<class data_t>
    data_t* retrieveSharedData(std::string key) const
    {
        if(!datahandlesEmptyCheck(key)) return nullptr;

        return dynamic_cast<DataHandle<data_t>*>(data_handles_->at(key))->Get();
    }

protected:
    bool datahandlesNullCheck() const
    {
        if(!data_handles_)
        {
            std::cerr << "[WARNING] ==> Empty shared data handler in processor " << Name << "." << std::endl;
            return false;
        }

        return true;
    }

    bool datahandlesDuplicatedCheck(std::string key) const
    {
        if(!datahandlesNullCheck()) return false;

        if(data_handles_->count(key) != 0)
        {
            std::cerr << "[WARNING] ==> Shared data added in processor " << Name << " already exists." << std::endl;
            return false;
        }

        return true;
    }

    bool datahandlesEmptyCheck(std::string key) const
    {
        if(!datahandlesNullCheck()) return false;

        if(data_handles_->count(key) == 0)
        {
            std::cerr << "[WARNING] ==> Shared data called in processor " << Name << " does not exist." << std::endl;
            return false;
        }

        return true;
    }

    bool is_display; 

    // Algo Name
    std::string Name;

    // Algo Description
    std::string Description = "None";

    // Verbosity
    int Verbose{0};

    // Parameter Name, Description, Value
    std::map<std::string, std::pair<std::string, int *> > IntParameters;
    std::map<std::string, std::pair<std::string, double *> > DoubleParameters;
    std::map<std::string, std::pair<std::string, std::string *> > StringParameters;

    // Event Writer and Data Storage
    std::shared_ptr<EventStoreAndWriter> EvtWrt;

    data_handles_t* data_handles_{nullptr};
};

#endif //DSIMU_ANAPROCESSOR_H
