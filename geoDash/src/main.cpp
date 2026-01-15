#include <iostream>
#include <SFML/Graphics.hpp>

#include "selection.h"
#include "editeur.h"

enum Fenetre {menu, selection, editor};

int main()
{
    Fenetre fenetre  = menu;
    auto window = sf::RenderWindow({1200u, 800u}, "CMake SFML Project",sf::Style::Fullscreen);
    window.setFramerateLimit(144);

    sf::Font font;
    font.loadFromFile("../assets/MapleMono-Regular.ttf");
    sf::Text txt_menu("Menu", font, 40);

    sf::Event event;

    sf::View camera;
    camera.setSize(window.getSize().x, window.getSize().y);
    camera.setCenter(window.getSize().x/2, window.getSize().y/2);

    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed or (event.key.code == sf::Keyboard::Escape and event.type == sf::Event::KeyReleased))
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

        window.setView(camera);
        window.display();
    }

    return 0;
}
