#include "editeur.h"

void editeur(sf::RenderWindow & window)
{

    sf::Font font;
    font.loadFromFile("../assets/MapleMono-Regular.ttf");
    sf::Text txt_menu("Menu", font, 40);

    sf::Event event;
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed or sf::Keyboard::isKeyReleased(sf::Keyboard::Escape))
            {
                return;
            }
        }

        window.clear();
        window.draw(txt_menu);
        window.display();
    }
}
