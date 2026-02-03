#include "editeur.h"
#include "parse_level.h"

#include <fstream>
#include <nlohmann/json.hpp>

enum Deplacement {up, down, right, left, stay};
enum PickedItem {Spike, SpikeReverse, Gravity,GravityReverse, Block, Player, End};

void save(const Level & level) {
    json data;

    // On modifie/ajoute les données

    std::vector<std::vector<int>> blocks;
    std::vector<std::vector<int>> spikes;
    std::vector<std::vector<int>> spikes_reverse;
    std::vector<std::vector<int>> pad_gravite;
    std::vector<std::vector<int>> pad_gravite_reverse;
    for(int i=0; i<level.width; ++i){
        for(int j=0; j<level.height; ++j){
            if(level.game[i][j]=="b")
                blocks.push_back({i, j});
            else if(level.game[i][j]=="s")
                spikes.push_back({i, j});
            else if(level.game[i][j]=="s_r")
                spikes_reverse.push_back({i, j});
            else if(level.game[i][j]=="g")
                pad_gravite.push_back({i, j});
            else if(level.game[i][j]=="g_r")
                pad_gravite_reverse.push_back({i, j});
            else if(level.game[i][j]=="p")
                data["spawn"]  = {i, j};
            else if(level.game[i][j]=="end")
                data["end"]  = {i, j};
        }
    }

    data["height"] = level.height;
    data["width"]  = level.width;
    data["blocks"] = blocks;
    data["spikes"] = spikes;
    data["spikes_reverse"] = spikes_reverse;
    data["pad_gravite"] = pad_gravite;
    data["pad_gravite_reverse"] = pad_gravite_reverse;

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
    PickedItem item = Block;
    Deplacement deplacement = stay;

    Level level = foo();
    level.game[level.spawn_coord.first][level.spawn_coord.second] = 'p';
    level.width = 1000;
    level.height=50;

    uint GROUND = 64*level.height;

    sf::Font font;
    font.loadFromFile("../assets/MapleMono-Regular.ttf");
    sf::Text txt_menu("Editeur", font, 40);

    sf::Event event;
    sf::View camera;
    camera.setSize(window.getSize().x, window.getSize().y);
    camera.setCenter(level.spawn_coord.first*64, GROUND - level.spawn_coord.second*64);


    sf::Texture textureSpike;
    textureSpike.loadFromFile("../assets/spike.png");
    sf::Sprite spike(textureSpike);
    spike.setScale(2,2);

    sf::Texture textureSpikeReverse;
    textureSpikeReverse.loadFromFile("../assets/spike_reverse.png");
    sf::Sprite spike_reverse(textureSpikeReverse);
    spike_reverse.setScale(2,2);

    sf::Texture texturePlayer;
    texturePlayer.loadFromFile("../assets/cube.png");
    sf::Sprite player(texturePlayer);
    player.setScale(2,2);

    sf::Texture texturePadGravite;
    texturePadGravite.loadFromFile("../assets/pad_gravite.png");
    sf::Sprite pad_gravite(texturePadGravite);
    pad_gravite.setScale(2,2);

    sf::Texture texturePadGraviteReverse;
    texturePadGraviteReverse.loadFromFile("../assets/pad_gravite_reverse.png");
    sf::Sprite pad_gravite_reverse(texturePadGraviteReverse);
    pad_gravite_reverse.setScale(2,2);

    sf::Texture textureEnd;
    textureEnd.loadFromFile("../assets/end.png");
    sf::Sprite end(textureEnd);
    end.setScale(2,2);

    sf::RectangleShape block(sf::Vector2f(64, 64));
    block.setFillColor(sf::Color(255,150,255));

    sf::RectangleShape bgPalette(sf::Vector2f(camera.getSize().x, 100));
    bgPalette.setFillColor(sf::Color(255,255,150));

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
                deplacement = up;
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                deplacement = down;
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                deplacement = left;
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                deplacement = right;

            if (event.type == sf::Event::KeyReleased)
            {
                    deplacement = stay;
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                if(bgPalette.getGlobalBounds().contains(mousePos))
                {
                    std::cout<<"Clique palette\n";
                    if(spike.getGlobalBounds().contains(mousePos))
                        item = Spike;
                    if(spike_reverse.getGlobalBounds().contains(mousePos))
                        item = SpikeReverse;
                    else if(pad_gravite.getGlobalBounds().contains(mousePos))
                        item = Gravity;
                    else if (pad_gravite_reverse.getGlobalBounds().contains(mousePos))
                        item = GravityReverse;
                    else if(block.getGlobalBounds().contains(mousePos))
                        item =  Block;
                    else if(player.getGlobalBounds().contains(mousePos))
                        item =  Player;
                    else if(end.getGlobalBounds().contains(mousePos))
                        item =  End;
                }
                else
                {
                    int i = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/64;
                    int j = GROUND/64 - window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/64 +1;
                    std::cout<<i<<" "<<j<<std::endl;
                    if(i>=0 and j>=0 and i<level.width and j<level.height)
                    {
                        std::cout<<"place : "<<i<<" "<<j<<std::endl;
                        switch(item){
                            case Block :
                                level.game[i][j]="b";
                                std::cout<<"place block in "<<i<<" "<<j<<std::endl;
                                break;
                            case Spike :
                                level.game[i][j]="s";
                                break;
                            case SpikeReverse :
                                level.game[i][j]="s_r";
                                break;
                            case Gravity :
                                level.game[i][j]="g";
                                break;
                            case GravityReverse :
                                level.game[i][j]="g_r";
                                break;
                            case Player :
                                for (auto &ligne : level.game) {
                                    for (std::string &val : ligne) {
                                        if(val=="p")
                                            val="n";
                                    }
                                }
                                level.game[i][j]="p";
                                break;

                            case End :
                                level.game[i][j]="end";
                                break;

                            default :
                                break;
                        }
                    }
                }
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                int i = window.mapPixelToCoords(sf::Mouse::getPosition(window)).x/64;
                int j = GROUND/64 - window.mapPixelToCoords(sf::Mouse::getPosition(window)).y/64 +1;
                if(i>=0 and j>=0 and i<level.width and j<level.height)
                    level.game[i][j]="n";
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            {
                save(level);
            }
        }

        window.clear(sf::Color(255,150,150));

        switch(deplacement){
            case up :
                camera.move(0, -int(6000*dt.asSeconds()));
                break;
            case down :
                camera.move(0, int(6000*dt.asSeconds()));
                break;
            case right :
                camera.move(int(6000*dt.asSeconds()), 0);
                break;
            case left :
                camera.move(int(-6000*dt.asSeconds()), 0);
                break;
            default:
                //ne bouge pas
                break;
        }

        window.setView(camera);

        //===== DRAW LEVEL ==========
        for(int i=0; i<level.width; ++i){
            for(int j=0; j<level.height; ++j){

                if(level.game[i][j]=="s")
                {
                    spike.setPosition(i*64, GROUND - j*64);
                    window.draw(spike);
                }
                else if(level.game[i][j]=="s_r")
                {
                    spike_reverse.setPosition(i*64, GROUND - j*64);
                    window.draw(spike_reverse);
                }
                else if(level.game[i][j]=="b")
                {
                    block.setPosition(i*64, GROUND - j*64);
                    window.draw(block);
                }
                else if(level.game[i][j]=="g")
                {
                    pad_gravite.setPosition(i*64, GROUND - j*64);
                    window.draw(pad_gravite);
                }
                else if(level.game[i][j]=="g_r")
                {
                    pad_gravite_reverse.setPosition(i*64, GROUND - j*64);
                    window.draw(pad_gravite_reverse);
                }
                else if(level.game[i][j]=="p")
                {
                    player.setPosition(i*64, GROUND - j*64);
                    window.draw(player);
                }
                else if(level.game[i][j]=="end")
                {
                    end.setPosition(i*64, GROUND - j*64);
                    window.draw(end);
                }
            }
        }
        //=======================

        //===== DRAW PALLETTE ==========
        int leftPal = camera.getCenter().x - camera.getSize().x/2;
        int topPal = camera.getCenter().y + camera.getSize().y/2 - 100;

        bgPalette.setPosition(leftPal ,topPal);
        spike.setPosition(leftPal + 100, topPal+10);
        spike_reverse.setPosition(leftPal + 300, topPal+10);
        block.setPosition(leftPal + 500, topPal+10);
        player.setPosition(leftPal + 700, topPal+10);
        pad_gravite.setPosition(leftPal + 900, topPal);
        pad_gravite_reverse.setPosition(leftPal + 1100, topPal+10);
        end.setPosition(leftPal + 1200, topPal+10);

        window.draw(bgPalette);
        window.draw(player);
        window.draw(spike);
        window.draw(spike_reverse);
        window.draw(pad_gravite);
        window.draw(pad_gravite_reverse);
        window.draw(block);
        window.draw(end);
        //=======================


        window.display();
    }
}
