#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>


using json = nlohmann::json;

struct Level{
    std::vector<std::vector<char>> game;
    uint width = 0;
    uint height = 0;
};

Level foo();
