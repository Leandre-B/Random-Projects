#include <iostream>
#include <SFML/Graphics.hpp>

#include "game.h"


enum Fenetre {menu, jeu};

int main()
{
    Fenetre fenetre  = jeu;
    auto window = sf::RenderWindow({1200u, 800u}, "CMake SFML Project"/*,sf::Style::Fullscreen*/);
    window.setFramerateLimit(144);

    sf::Clock clock;
    sf::View camera;
    camera.setSize(window.getSize().x, window.getSize().y);
    sf::Event event;

    while (window.isOpen())
    {
        sf::Time dt = clock.getElapsedTime();
        clock.restart();

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed or sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        switch(fenetre){
            case jeu:
                play_game(window);
                break;
            case menu:
                std::cout<<"menu\n";
                //play_menu();
        }
    }
}
