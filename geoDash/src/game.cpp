#include "game.h"

bool spikeCollision(sf::Sprite & cube, sf::Sprite & spike){
    sf::FloatRect hitbox(
        spike.getGlobalBounds().left + 27.f,
        spike.getGlobalBounds().top  + 30.f,
        10.f,
        30.f
    );

    return hitbox.intersects(cube.getGlobalBounds());

}
void play_game(sf::RenderWindow & window)
{
    uint GRAVITY = 7500;
    int YSPEED = 0;
    int XSPEED = 700;
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
    cube.setOrigin(16.f, 16.f);
    cube.setScale(2,2);
    cube.setPosition(plt.spawn_coord.first*64 + 64 , GROUND - plt.spawn_coord.second*64);

    sf::Clock clock;
    sf::View camera;
    camera.setSize(window.getSize().x, window.getSize().y);
    camera.setCenter(cube.getPosition().x + 400, GROUND - plt.spawn_coord.second*64);

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
                        YSPEED = -1650;
                        onGround = false;
                    }
                }
                else if (event.key.code == sf::Keyboard::R){
                    cube.setPosition(plt.spawn_coord.first*64 + 64 , GROUND - plt.spawn_coord.second*64);
                    cube.setRotation(0);
                    camera.setCenter(cube.getPosition().x + 400, GROUND - plt.spawn_coord.second*64);
                    lost=false;
                }
            }
        }

        YSPEED +=GRAVITY*dt.asSeconds();
        if(YSPEED>900)
            YSPEED = 900;
        if(!lost){
            cube.move(XSPEED*dt.asSeconds(), YSPEED*dt.asSeconds());
            if(!onGround)
                cube.rotate(300*dt.asSeconds());
            else{
                int rota = int (cube.getRotation())%360;
                if(rota>= 45 and rota<135)
                    cube.setRotation(90);
                else if(rota>= 135 and rota<225)
                    cube.setRotation(180);
                else if(rota>= 225 and rota<315)
                    cube.setRotation(270);
                else
                    cube.setRotation(0);
            }
        }

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
                            cube.setPosition(cube.getPosition().x, rect.getGlobalBounds().top - 32);
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
        if(GROUND/64 - cube.getGlobalBounds().top/64 >0)
            if((plt.game[cube.getGlobalBounds().left/64][GROUND/64 - cube.getGlobalBounds().top/64]=='b') or (plt.game[(cube.getGlobalBounds().left/64) +1][GROUND/64 - cube.getGlobalBounds().top/64]=='b'))
                onGround = true;
            else
                onGround = false;
        else
            lost = true;

        if(cube.getPosition().y < camera.getCenter().y -150)
        {
            camera.setCenter(cube.getPosition().x + 400, camera.getCenter().y);
            camera.move(0, -250*dt.asSeconds());
        }
        else if(cube.getPosition().y > camera.getCenter().y + 150 and !onGround)
        {
            camera.setCenter(cube.getPosition().x + 400, camera.getCenter().y);
            camera.move(0, 500*dt.asSeconds());
        }
        else
            camera.setCenter(cube.getPosition().x + 400, camera.getCenter().y);

        window.setView(camera);

        window.draw(cube);
        window.display();
    }
}
