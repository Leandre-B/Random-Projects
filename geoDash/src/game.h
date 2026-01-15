#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>

#include "parse_level.h"



using Plateau = std::array<std::array<char,20>, 1000>;
void play_game(sf::RenderWindow & window);
