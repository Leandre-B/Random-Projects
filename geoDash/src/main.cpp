#include <iostream>
#include <SFML/Graphics.hpp>

#include "selection.h"
#include "editeur.h"

enum Fenetre {menu, selection, editor};

int main()
{
    Fenetre fenetre  = menu;
    auto window = sf::RenderWindow({1200u, 800u}, "CMake SFML Project",sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    sf::Font font;
    font.loadFromFile("../assets/MapleMono-Regular.ttf");
    sf::Text txt_menu("Menu", font, 40);

    sf::Event event;
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed or sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                std::cout<<"quitting\n";
                window.close();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            {
                fenetre = selection;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                fenetre = editor;
            }
        }

        window.clear();

        switch(fenetre){
            case selection:
                open_selection(window);
                fenetre = menu;
                break;
            case menu:
                window.draw(txt_menu);
                break;

            case editor:
                editeur(window);
                fenetre = menu;
                break;

            default:
                break;
        }

        window.display();
    }

    return 0;
}
