#include <iostream>
#include <SFML/Graphics.hpp>
#include <array>
#include <cmath>

#include "parse_level.h"

uint GRAVITY = 6000;
int YSPEED = 0;
int XSPEED = 80000;
bool jump = false;
bool onGround = false;
uint GROUND = 1080;

using Plateau = std::array<std::array<char,20>, 1000>;

bool spikeCollision(sf::Sprite & cube, sf::Sprite & spike){
    sf::FloatRect hitbox(
        spike.getGlobalBounds().left + 10.f,
        spike.getGlobalBounds().top  + 10.f,
        30.f,
        30.f
    );

    return hitbox.intersects(cube.getGlobalBounds());

}
int main()
{

    Level plt = foo();
    for (const auto &ligne : plt.game) {
        for (char val : ligne) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
    auto window = sf::RenderWindow({1200u, 800u}, "CMake SFML Project"/*,sf::Style::Fullscreen*/);
    window.setFramerateLimit(144);



    //== TEXTURES ==
    sf::Texture textureCube;
    if (!textureCube.loadFromFile("../assets/cube.png"))
        return EXIT_FAILURE;

    sf::Texture textureSpike;
    if (!textureSpike.loadFromFile("../assets/spike.png"))
        return EXIT_FAILURE;
    //===

    sf::Sprite cube(textureCube);
    cube.setScale(2,2);
    cube.setPosition(200, 1016);

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
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Space){
                    if(onGround){
                        YSPEED = -1600;
                        onGround = false;
                    }
                }
            }
        }

        YSPEED +=GRAVITY*dt.asSeconds();
        cube.move(XSPEED*dt.asSeconds()*dt.asSeconds(), YSPEED*dt.asSeconds());

        window.clear(sf::Color(255,150,150));


         for(int i=0; i<plt.width; ++i){
            for(int j=0; j<plt.height; ++j){
                if(plt.game[i][j]=='s')
                {
                    sf::Sprite spike(textureSpike);
                    spike.setScale(2,2);
                    spike.setPosition(i*64, GROUND - j*64);
                    window.draw(spike);
                    if(spikeCollision(spike,cube)){
                        XSPEED  = 0;
                        YSPEED  = 0;
                        GRAVITY = 0;

                    }
                }
                else if(plt.game[i][j]=='b')
                {
                    sf::RectangleShape rect(sf::Vector2f(64, 64));
                    rect.setPosition(i*64, GROUND - j*64);
                    rect.setFillColor(sf::Color(255,150,255));
                    window.draw(rect);

                    //collision avec bloc
                    if(rect.getGlobalBounds().intersects(cube.getGlobalBounds())){
                        //si le joueurs (cube) est au dessus du bloc
                        if(rect.getGlobalBounds().top >=
                            (cube.getGlobalBounds().top +50))
                        {
                            cube.setPosition(cube.getPosition().x, rect.getGlobalBounds().top - 64);
                            YSPEED = 0;
                            onGround = true;

                        //sinon perdu
                        }else if(cube.getGlobalBounds().top != rect.getGlobalBounds().top + 64){
                            XSPEED  = 0;
                            YSPEED  = 0;
                            GRAVITY = 0;
                            onGround = false;
                        }
                    }
                }

            }
        }

        if((plt.game[cube.getGlobalBounds().left/64][GROUND/64 - cube.getGlobalBounds().top/64]=='b') or (plt.game[(cube.getGlobalBounds().left/64) +1][GROUND/64 - cube.getGlobalBounds().top/64]=='b'))
            onGround = true;
        else
            onGround = false;
        //std::cout<<onGround<<"\n";

        camera.setCenter(cube.getPosition().x + 600, 800);
        window.setView(camera);

        window.draw(cube);
        window.display();
    }
}
