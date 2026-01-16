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

bool padCollision(sf::Sprite & cube, sf::Sprite & pad){
    sf::FloatRect hitbox(
        pad.getGlobalBounds().left,
        pad.getGlobalBounds().top  + 63.f,
        64.f,
        1.f
    );

    return hitbox.intersects(cube.getGlobalBounds());

}

void play_game(sf::RenderWindow & window)
{
    int GRAVITY = 7500;
    int YSPEED = 0;
    int XSPEED = 700;
    bool jump = false;
    bool onGround = false;
    uint GROUND = 1080;
    bool lost = false;
    bool hold_jump = false;
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

    sf::Texture texturePadGravite;
    texturePadGravite.loadFromFile("../assets/pad_gravite.png");
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
    int frame=15;
    while (!exit)
    {
        ++frame;
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
                if (event.key.code == sf::Keyboard::Space)
                    hold_jump = true;

                if (event.key.code == sf::Keyboard::R){
                    cube.setPosition(plt.spawn_coord.first*64 + 64 , GROUND - plt.spawn_coord.second*64);
                    cube.setRotation(0);
                    camera.setCenter(cube.getPosition().x + 400, GROUND - plt.spawn_coord.second*64);
                    GRAVITY = 7500;
                    YSPEED = 0;
                    lost=false;
                }
            }
            if (event.type == sf::Event::KeyReleased)
            {
                if (event.key.code == sf::Keyboard::Space)
                    hold_jump = false;
            }
        }
        if(hold_jump and onGround){
            if(GRAVITY>0)
                YSPEED = -1500;
            else
                YSPEED = 1500;
            onGround = false;
        }
        YSPEED +=GRAVITY*dt.asSeconds();
        if(YSPEED>900 and GRAVITY>0)
            YSPEED = 900;
        else if (YSPEED<-1400 and GRAVITY<0)
            YSPEED = -1400;
        if(!lost){
            cube.move(XSPEED*dt.asSeconds(), YSPEED*dt.asSeconds());
            if(!onGround)
                cube.rotate(300*dt.asSeconds());
            else{
                int rota = int (cube.getRotation())%360;
                if(rota>= 25 and rota<115)
                    cube.setRotation(90);
                else if(rota>= 115 and rota<205)
                    cube.setRotation(180);
                else if(rota>= 205 and rota<295)
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
                            (cube.getGlobalBounds().top +40) and GRAVITY>0)
                        {
                            cube.setPosition(cube.getPosition().x, rect.getGlobalBounds().top - 32);
                            YSPEED = 0;
                            onGround = true;

                        }
                        else if(rect.getGlobalBounds().top <=
                            (cube.getGlobalBounds().top +44) and GRAVITY<0)
                        {
                            cube.setPosition(cube.getPosition().x, rect.getGlobalBounds().top + 64 +32);
                            YSPEED = 0;
                            onGround = true;

                        }
                        else if(cube.getGlobalBounds().top != rect.getGlobalBounds().top + 64){
                            lost  = true;
                        }
                    }
                }
                else if(plt.game[i][j]=='g')
                {
                    sf::Sprite pad_gravite(texturePadGravite);
                    pad_gravite.setScale(2,2);
                    pad_gravite.setPosition(i*64, GROUND - j*64);
                    window.draw(pad_gravite);
                    if(frame>10 and padCollision(pad_gravite,cube)){
                        GRAVITY *= -1;
                        if(GRAVITY<0)
                            cube.move(0, -20);
                        else
                            cube.move(0, 20);
                        frame = 0;
                    }
                }
            }
        }
        if(GROUND/64 - cube.getGlobalBounds().top/64 >0){
            if(GRAVITY>=0){
                if((plt.game[cube.getGlobalBounds().left/64][GROUND/64 - cube.getGlobalBounds().top/64]=='b') or (plt.game[(cube.getGlobalBounds().left/64) +1][GROUND/64 - cube.getGlobalBounds().top/64]=='b'))
                    onGround = true;
                else
                    onGround = false;
            }else{
                if((plt.game[cube.getGlobalBounds().left/64][GROUND/64 - cube.getGlobalBounds().top/64 +2]=='b') or (plt.game[(cube.getGlobalBounds().left/64) +1][GROUND/64 - cube.getGlobalBounds().top/64 +2]=='b'))
                    onGround = true;
                else
                    onGround = false;
            }
        }else
            lost = true;

        if(cube.getPosition().y < camera.getCenter().y -150)
        {
            camera.setCenter(cube.getPosition().x + 400, camera.getCenter().y);
            camera.move(0, -500*dt.asSeconds());
        }
        else if(cube.getPosition().y > camera.getCenter().y + 150 and !onGround)
        {
            camera.setCenter(cube.getPosition().x + 400, camera.getCenter().y);
            camera.move(0, 700*dt.asSeconds());
        }
        else
            camera.setCenter(cube.getPosition().x + 400, camera.getCenter().y);

        window.setView(camera);

        window.draw(cube);
        window.display();
    }
}
