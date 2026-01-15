#include "selection.h"

enum Fenetre {selection, jeu};



struct Niveau{
    std::string chemin;
    sf::Text sfml_text;
    sf::FloatRect rect_slct;
};

void get_niveaux(std::vector<Niveau> & niveaux, sf::RenderWindow & window){

    //on initialise la police d'écriture
    sf::Font font;
    font.loadFromFile("../assets/MapleMono-Regular.ttf");

    std::string path = "../levels";

    int i=0;
    for (const std::filesystem::directory_entry & entry : std::filesystem::directory_iterator(path)) {
        Niveau niv;
        //on récupere le chemin du plateau
        niv.chemin=entry.path().string();

        std::string nom = niv.chemin;
        nom.erase(nom.size()-5,5); //retire les 5 derniers (.json)
        nom.erase(0,10); //retire le path
            //=====Initialisation sfml_text========
            niv.sfml_text.setString(nom);
            niv.sfml_text.setFont(font);
            niv.sfml_text.setCharacterSize (40);


            niv.rect_slct = niv.sfml_text.getGlobalBounds(); //pour centrer le texte
            niv.sfml_text.setPosition(100,100 + i*64);
            //==================

            //le texte a bougé, on doit re-recuperer les bounds
            niv.rect_slct = niv.sfml_text.getGlobalBounds();

            niveaux.push_back(niv);
            window.draw(niv.sfml_text);
            ++i;
    }
}

void open_selection(sf::RenderWindow & window)
{
    Fenetre fenetre  = selection;
    std::vector<Niveau> niveaux;

    sf::Clock clock;
    sf::View camera;
    camera.setSize(window.getSize().x, window.getSize().y);
    camera.setCenter(window.getSize().x/2, window.getSize().y/2);

    sf::Event event;

    sf::Font font;
    font.loadFromFile("../assets/MapleMono-Regular.ttf");
    sf::Text txt_selection("Selection de niveau", font, 40);

    while (window.isOpen())
    {
        sf::Time dt = clock.getElapsedTime();
        clock.restart();

        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed or (event.key.code == sf::Keyboard::Escape and event.type == sf::Event::KeyReleased))
            {
                std::cout<<"quitting selection menu\n";
                return;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
            {
                fenetre = jeu;
            }
        }

        window.clear();
        switch(fenetre){
            case jeu:
                play_game(window);
                fenetre = selection;
                break;

            case selection:
                window.draw(txt_selection);
                get_niveaux(niveaux,window);
                break;

            default:
                break;
        }

        window.setView(camera);
        window.display();
    }
}
