#include "editeur.h"
#include "parse_level.h"

#include <fstream>
#include <nlohmann/json.hpp>

void save(const Level & level) {
    json data;

    // On modifie/ajoute les données

    std::vector<std::vector<int>> blocks;
    std::vector<std::vector<int>> spikes;
    std::vector<std::vector<int>> pad_gravite;
    for(int i=0; i<level.width; ++i){
        for(int j=0; j<level.height; ++j){
            if(level.game[i][j]=='b')
                blocks.push_back({i, j});
            else if(level.game[i][j]=='s')
                spikes.push_back({i, j});
            else if(level.game[i][j]=='g')
                pad_gravite.push_back({i, j});
        }
    }

    data["height"] = level.height;
    data["width"]  = level.width;
    data["spawn"]  = {0, 1};
    data["blocks"] = blocks;
    data["spikes"] = spikes;
    data["pad_gravite"] = pad_gravite;

    // On écrit dans le fichier
    std::ofstream out("../levels/test.json");
    if (!out.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier pour écriture");
    }
    out << data.dump(-1);
    out.close();
    std::cout<<"Niveau enregistré\n";
}


void editeur(sf::RenderWindow & window)
{
    uint GROUND = 1080;
    Level level = foo();
    level.width = 1000;
    level.height=100;

    sf::Font font;
    font.loadFromFile("../assets/MapleMono-Regular.ttf");
    sf::Text txt_menu("Editeur", font, 40);

    sf::Event event;
    sf::View camera;
    camera.setSize(window.getSize().x, window.getSize().y);
    camera.setCenter(10, 1000);


    sf::Texture textureSpike;
    textureSpike.loadFromFile("../assets/spike.png");

    sf::Texture texturePadGravite;
    texturePadGravite.loadFromFile("../assets/pad_gravite.png");

    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Time dt = clock.getElapsedTime();
        clock.restart();

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed or (event.key.code == sf::Keyboard::Escape and event.type == sf::Event::KeyReleased))
            {
                return;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
                camera.move(0, -500*dt.asSeconds());
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                camera.move(0, 500*dt.asSeconds());
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                camera.move(-500*dt.asSeconds(), 0);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                camera.move(500*dt.asSeconds(), 0);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                int i = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/64;
                int j = GROUND/64 - window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/64 +2;
                std::cout<<i<<" "<<j<<"\n";
                if(i>=0 and j>=0 and i<level.width and j<level.height)
                    level.game[i][j]='b';
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                int i = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/64;
                int j = GROUND/64 - window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/64 +2;
                if(i>=0 and j>=0 and i<level.width and j<level.height)
                    level.game[i][j]='s';
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            {
                int i = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/64;
                int j = GROUND/64 - window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/64 +2;
                if(i>=0 and j>=0 and i<level.width and j<level.height)
                    level.game[i][j]='n';
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
            {
                int i = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/64;
                int j = GROUND/64 - window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/64 +2;
                if(i>=0 and j>=0 and i<level.width and j<level.height)
                    level.game[i][j]='g';
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            {
                save(level);
            }
        }

        window.clear(sf::Color(255,150,150));

        for(int i=0; i<level.width; ++i){
            for(int j=0; j<level.height; ++j){

                if(level.game[i][j]=='s')
                {
                    sf::Sprite spike(textureSpike);
                    spike.setScale(2,2);
                    spike.setPosition(i*64, GROUND - j*64);
                    window.draw(spike);
                }
                else if(level.game[i][j]=='b')
                {
                    sf::RectangleShape rect(sf::Vector2f(64, 64));
                    rect.setPosition(i*64, GROUND - j*64);
                    rect.setFillColor(sf::Color(255,150,255));
                    window.draw(rect);
                }
                else if(level.game[i][j]=='g')
                {
                    sf::Sprite pad_gravite(texturePadGravite);
                    pad_gravite.setScale(2,2);
                    pad_gravite.setPosition(i*64, GROUND - j*64);
                    window.draw(pad_gravite);
                }
                // else{
                //     sf::RectangleShape rect(sf::Vector2f(64, 64));
                //     rect.setPosition(i*64, GROUND - j*64);
                //     if(i%2==0 == j%2==0)
                //         rect.setFillColor(sf::Color(100,100,100));
                //     else
                //         rect.setFillColor(sf::Color(150,150,150));
                //     window.draw(rect);
                // }
            }
        }


        window.draw(txt_menu);
        window.setView(camera);

        window.display();
    }
}
