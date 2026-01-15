#pragma once

#include <SFML/Graphics.hpp>
#include <fstream>
#include <nlohmann/json.hpp>
#include <iostream>


using json = nlohmann::ordered_json;


void editeur(sf::RenderWindow & window);