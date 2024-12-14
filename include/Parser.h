#pragma once
#include "WareHouse.h"
#include <string>

class Parser {
    public:
        static void parseConfigurationFile(const std::string& configFilePath, WareHouse& wareHouse);
};