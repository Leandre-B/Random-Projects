#include "game.h"

bool spikeCollision(sf::FloatRect & hitboxCube, sf::Sprite & spike){
    sf::FloatRect hitbox(
        spike.getGlobalBounds().left + 27.f,
        spike.getGlobalBounds().top  + 30.f,
        10.f,
        30.f
    );

    return hitbox.intersects(hitboxCube);

}

bool spikeReverseCollision(sf::FloatRect & hitboxCube, sf::Sprite & spike){
    sf::FloatRect hitbox(
        spike.getGlobalBounds().left + 27.f,
        spike.getGlobalBounds().top  + 5.f,
        10.f,
        30.f
    );

    return hitbox.intersects(hitboxCube);

}

bool padCollision(sf::FloatRect & hitboxCube, sf::Sprite & pad){
    sf::FloatRect hitbox(
        pad.getGlobalBounds().left,
        pad.getGlobalBounds().top  + 55.f,
        64.f,
        6.f
    );

    return hitbox.intersects(hitboxCube);

}

bool padCollisionGraviteReverse(sf::FloatRect & hitboxCube, sf::Sprite & pad){
    sf::FloatRect hitbox(
        pad.getGlobalBounds().left,
                         pad.getGlobalBounds().top,
                         64.f,
                         6.f
    );

    return hitbox.intersects(hitboxCube);

}

void play_game(sf::RenderWindow & window)
{
    int GRAVITY = 7040;
    int YSPEED = 0;
    int XSPEED = 704;
    bool jump = false;
    bool onGround = false;
    bool lost = false;
    bool hold_jump = false;
    Level plt = foo();
    uint GROUND = 64*plt.height;

    //== TEXTURES ==
    sf::Texture textureCube;
    textureCube.loadFromFile("../assets/cube.png");

    sf::Texture textureSpike;
    textureSpike.loadFromFile("../assets/spike.png");


    sf::Texture textureSpikeReverse;
    textureSpikeReverse.loadFromFile("../assets/spike_reverse.png");

    // if (!textureSpike.loadFromFile("../assets/spike.png"))
    //     return EXIT_FAILURE;

    sf::Texture texturePadGravite;
    texturePadGravite.loadFromFile("../assets/pad_gravite.png");

    sf::Texture texturePadGraviteReverse;
    texturePadGraviteReverse.loadFromFile("../assets/pad_gravite_reverse.png");
    //===

    sf::Sprite cube(textureCube);
    cube.setOrigin(16.f, 16.f);
    cube.setScale(2,2);
    cube.setPosition(plt.spawn_coord.first*64 + 64 , GROUND - plt.spawn_coord.second*64);

    sf::FloatRect hitboxCube (cube.getPosition(), sf::Vector2f(64, 64));
    hitboxCube.left = cube.getPosition().x -32;
    hitboxCube.top  = cube.getPosition().y -32;

    sf::Clock clock;
    sf::View camera;
    camera.setSize(window.getSize().x, window.getSize().y);
    camera.setCenter(cube.getPosition().x + 400, GROUND - plt.spawn_coord.second*64);

    sf::Event event;
    bool exit = false;
    int frame=15;

    float time=0;

    sf::Font font;
    font.loadFromFile("../assets/MapleMono-Regular.ttf");
    sf::Text txt_fps("0", font, 40);
    while (!exit)
    {
        if(lost){
            cube.setPosition(plt.spawn_coord.first*64 + 64 , GROUND - plt.spawn_coord.second*64);
            cube.setRotation(0);
            camera.setCenter(cube.getPosition().x + 400, GROUND - plt.spawn_coord.second*64);
            GRAVITY = 7500;
            YSPEED = 0;
            hitboxCube.left = cube.getPosition().x -32;
            hitboxCube.top  = cube.getPosition().y -32;
            lost=false;
            time=0;
        }
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
                    hitboxCube.left = cube.getPosition().x -32;
                    hitboxCube.top  = cube.getPosition().y -32;
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
        if(YSPEED>1000 and GRAVITY>0)
            YSPEED = 1000;
        else if (YSPEED<-1400 and GRAVITY<0)
            YSPEED = -1400;
        if(!lost){
            cube.move(int(XSPEED*dt.asSeconds()), int(YSPEED*dt.asSeconds()));
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
        hitboxCube.left = cube.getPosition().x -32;
        hitboxCube.top  = cube.getPosition().y -32;

        if (hitboxCube.left>38*plt.width)
            window.clear(sf::Color(150,10,75));
        else if(hitboxCube.left>24.5*plt.width)
            window.clear(sf::Color(0,0,0));
        else
            window.clear(sf::Color(255,150,150));


         for(int i=0; i<plt.width; ++i){
            for(int j=0; j<plt.height; ++j){
                if(plt.game[i][j]=="s")
                {

                    sf::Sprite spike(textureSpike);
                    spike.setScale(2,2);
                    spike.setPosition(i*64, GROUND - j*64);
                    window.draw(spike);

                    if(spikeCollision(hitboxCube, spike)){
                        lost = true;

                    }
                }
                else if(plt.game[i][j]=="s_r")
                {
                    sf::Sprite spikeReverse(textureSpikeReverse);
                    spikeReverse.setScale(2,2);
                    spikeReverse.setPosition(i*64, GROUND - j*64);
                    window.draw(spikeReverse);

                    if(spikeReverseCollision(hitboxCube, spikeReverse)){
                        lost = true;

                    }
                }
                else if(plt.game[i][j]=="b")
                {
                    sf::RectangleShape rect(sf::Vector2f(64, 64));
                    rect.setPosition(i*64, GROUND - j*64);
                    rect.setFillColor(sf::Color(255,150,255));
                    window.draw(rect);

                    //collision avec bloc
                    if(rect.getGlobalBounds().intersects(hitboxCube)){
                        //si le joueurs (cube) est au dessus du bloc
                        if(rect.getGlobalBounds().top >=
                            (hitboxCube.top +40 ) and GRAVITY>0)
                        {
                            cube.setPosition(int(cube.getPosition().x), int(rect.getGlobalBounds().top - 32));
                            YSPEED = 0;
                            onGround = true;

                        }
                        else if((rect.getGlobalBounds().top +64) <=
                            (hitboxCube.top +44) and GRAVITY<0)
                        {
                            cube.setPosition(int(cube.getPosition().x), int(rect.getGlobalBounds().top + 64 +32));
                            YSPEED = 0;
                            onGround = true;

                        }
                        else if(frame>5 and hitboxCube.top != rect.getGlobalBounds().top + 64){
                            lost  = true;
                        }
                    }
                }
                else if(plt.game[i][j]=="g")
                {
                    sf::Sprite pad_gravite(texturePadGravite);
                    pad_gravite.setScale(2,2);
                    pad_gravite.setPosition(i*64, GROUND - j*64);
                    window.draw(pad_gravite);
                    if(frame>15 and padCollision(hitboxCube, pad_gravite)){
                        GRAVITY *= -1;
                        YSPEED  *= -1;
                        frame = 0;
                    }
                }
                else if(plt.game[i][j]=="g_r")
                {
                    sf::Sprite pad_gravite(texturePadGraviteReverse);
                    pad_gravite.setScale(2,2);
                    pad_gravite.setPosition(i*64, GROUND - j*64);
                    window.draw(pad_gravite);
                    if(frame>15 and padCollisionGraviteReverse(hitboxCube, pad_gravite)){
                        GRAVITY *= -1;
                        YSPEED  *= -1;
                        frame = 0;
                    }
                }
            }
        }
        if(GROUND/64 - hitboxCube.top/64 >0){
            if(GRAVITY>=0){
                if((plt.game[hitboxCube.left/64][GROUND/64 - hitboxCube.top/64]=="b") or (plt.game[(hitboxCube.left/64) +1][GROUND/64 - hitboxCube.top/64]=="b"))
                    onGround = true;
                else
                    onGround = false;
            }else{
                if((plt.game[hitboxCube.left/64][GROUND/64 - hitboxCube.top/64 +1]=="b") or (plt.game[(hitboxCube.left/64) +1][GROUND/64 - hitboxCube.top/64 +1]=="b"))
                    onGround = true;
                else
                    onGround = false;
            }
        }else
            lost = true;

        camera.setCenter((cube.getPosition().x + 400), (camera.getCenter().y));
        if(cube.getPosition().y < camera.getCenter().y -200){
            camera.move(0, -(850*dt.asSeconds()));
        }
        if(cube.getPosition().y > camera.getCenter().y + 200){
            camera.move(0, (850*dt.asSeconds()));
        }
        window.setView(camera);



        // sf::RectangleShape hitboxDebug;
        // hitboxDebug.setPosition(hitboxCube.left, hitboxCube.top);
        // hitboxDebug.setSize({hitboxCube.width, hitboxCube.height});
        // hitboxDebug.setFillColor(sf::Color::Transparent);
        // hitboxDebug.setOutlineColor(sf::Color::Blue);
        // hitboxDebug.setOutlineThickness(2.f);
        // window.draw(hitboxDebug);

        window.draw(cube);
        time+=dt.asSeconds();
        txt_fps.setString(std::to_string(1/dt.asSeconds()));
        txt_fps.setPosition(camera.getCenter().x -700, camera.getCenter().y-500);
        window.draw(txt_fps);

        window.display();
    }
}
