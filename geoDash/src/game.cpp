#include "game.h"

bool spikeCollision(sf::Sprite & cube, sf::Sprite & spike){
    sf::FloatRect hitbox(
        spike.getGlobalBounds().left + 10.f,
        spike.getGlobalBounds().top  + 30.f,
        30.f,
        30.f
    );

    return hitbox.intersects(cube.getGlobalBounds());

}
void play_game(sf::RenderWindow & window)
{
    uint GRAVITY = 6000;
    int YSPEED = 0;
    int XSPEED = 800;
    bool jump = false;
    bool onGround = false;
    uint GROUND = 1080;
    bool lost = false;

    Level plt = foo();


    //== TEXTURES ==
    sf::Texture textureCube;
    textureCube.loadFromFile("../assets/cube.png");
    // if (!textureCube.loadFromFile("../assets/cube.png"))
    //     return EXIT_FAILURE;

    sf::Texture textureSpike;
    textureSpike.loadFromFile("../assets/spike.png");
    // if (!textureSpike.loadFromFile("../assets/spike.png"))
    //     return EXIT_FAILURE;
    //===

    sf::Sprite cube(textureCube);
    cube.setScale(2,2);
    cube.setPosition(plt.spawn_coord.first*64 , GROUND - plt.spawn_coord.second*64);

    sf::Clock clock;
    sf::View camera;
    camera.setSize(window.getSize().x, window.getSize().y);
    sf::Event event;
    bool exit = false;
    while (!exit)
    {
        sf::Time dt = clock.getElapsedTime();
        clock.restart();

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed or (event.key.code == sf::Keyboard::Escape and event.type == sf::Event::KeyReleased))
            {
                return;
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
        if(YSPEED>1200)
            YSPEED = 1200;
        if(!lost)
            cube.move(XSPEED*dt.asSeconds(), YSPEED*dt.asSeconds());

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
                        lost = true;

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
                            (cube.getGlobalBounds().top +40))
                        {
                            cube.setPosition(cube.getPosition().x, rect.getGlobalBounds().top - 64);
                            YSPEED = 0;
                            onGround = true;

                        //sinon perdu
                        }else if(cube.getGlobalBounds().top != rect.getGlobalBounds().top + 64){
                            lost  = true;
                        }
                    }
                }
            }
        }

        if((plt.game[cube.getGlobalBounds().left/64][GROUND/64 - cube.getGlobalBounds().top/64]=='b') or (plt.game[(cube.getGlobalBounds().left/64) +1][GROUND/64 - cube.getGlobalBounds().top/64]=='b'))
            onGround = true;
        else
            onGround = false;

        camera.setCenter(cube.getPosition().x + 400, cube.getPosition().y);
        window.setView(camera);

        window.draw(cube);
        window.display();
    }
}
