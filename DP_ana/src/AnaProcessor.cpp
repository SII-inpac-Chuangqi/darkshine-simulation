//
// Created by Zhang Yulei on 9/19/20.
//
#include "Core/AnaProcessor.h"

#include <utility>

void AnaProcessor::RegisterIntParameter(const std::string &name,
                                        const std::string &description,
                                        int *address,
                                        int default_value) {
    if (IntParameters.count(name) != 0)
        std::cerr << "[WARNING] ==> Parameter name already exists." << std::endl;
    else {
        *address = default_value;
        std::pair<std::string, int *> tmp(description, address);
        IntParameters.insert(std::pair<std::string, std::pair<std::string, int *> >(name, tmp));

        if (Verbose > 1) {
            std::cout << "[Parameter REGISTER] ==> Parameter " + name + " has been successfully added." << std::endl;
        }
    }
}

void AnaProcessor::RegisterDoubleParameter(const string &name,
                                           const string &description,
                                           double *address,
                                           double default_value) {
    if (DoubleParameters.count(name) != 0)
        std::cerr << "[WARNING] ==> Parameter name already exists." << std::endl;
    else {
        *address = default_value;
        std::pair<std::string, double *> tmp(description, address);
        DoubleParameters.insert(std::pair<std::string, std::pair<std::string, double *> >(name, tmp));

        if (Verbose > 1) {
            std::cout << "[Parameter REGISTER] ==> Parameter " + name + " has been successfully added." << std::endl;
        }
    }

}

void AnaProcessor::RegisterStringParameter(const string &name,
                                           const string &description,
                                           string *address,
                                           std::string default_value) {
    if (StringParameters.count(name) != 0)
        std::cerr << "[WARNING] ==> Parameter name already exists." << std::endl;
    else {
        *address = std::move(default_value);
        std::pair<std::string, std::string *> tmp(description, address);
        StringParameters.insert(std::pair<std::string, std::pair<std::string, std::string *> >(name, tmp));

        if (Verbose > 1) {
            std::cout << "[Parameter REGISTER] ==> Parameter " + name + " has been successfully added." << std::endl;
        }
    }
}

void AnaProcessor::setIntValue(const std::string &name, int value) {

    if (IntParameters.count(name) == 0)
        std::cerr << "[WARNING] ==> Parameter name does not exist." << std::endl;
    else
        *(IntParameters.at(name).second) = value;
}

void AnaProcessor::setDoubleValue(const std::string &name, double value) {
    if (DoubleParameters.count(name) == 0)
        std::cerr << "[WARNING] ==> Parameter name does not exist." << std::endl;
    else
        *(DoubleParameters.at(name).second) = value;
}

void AnaProcessor::setStringValue(const std::string &name, const std::string &value) {
    if (StringParameters.count(name) == 0)
        std::cerr << "[WARNING] ==> Parameter name does not exist." << std::endl;
    else
        *(StringParameters.at(name).second) = value;
}

ostream &operator<<(ostream &os, const AnaProcessor &processor) {
    os << "Name: " << processor.Name;
    return os;
}





