#include "draw.h"

#include <SFML/Graphics.hpp>

void draw_laby(bool** topWall, bool** leftWall, int m, int n){
  
  // Création de la fenêtre
  sf::RenderWindow fenetre(sf::VideoMode({1000,1000}), "Labyrinthe");
  sf::Event event;

  // Premier dessin de la fenêtre
  fenetre.clear(); // On efface tout
  fenetre.display(); // On affiche


  //definie la forme des murs du labyrinthe
  sf::RectangleShape topWallDraw;
  sf::RectangleShape leftWallDraw;
  topWallDraw.setSize(sf::Vector2f(20,1));
  leftWallDraw.setSize(sf::Vector2f(1,20));
  topWallDraw.setFillColor(sf::Color::White);
  leftWallDraw.setFillColor(sf::Color::White);


  while(fenetre.isOpen()) {
    while(fenetre.pollEvent(event)) { // boucle des événements

      //event pour pouvoir fermer la fenetre
      if(event.type == sf::Event::Closed) {
        fenetre.close();
      }
    }
    
    //dessine les topWall
    for(int i = 0;i<m+1;++i){
      for(int j = 0;j<n;++j){
        if(topWall[i][j]==true){
          topWallDraw.setPosition(sf::Vector2f(50+j*20,50+i*20));
          fenetre.draw(topWallDraw);
        }
      }
    }
    
    //dessine les leftWall
    for(int i = 0;i<m;++i){
      for(int j = 0;j<n+1;++j){
        if(leftWall[i][j]==true){
          leftWallDraw.setPosition(sf::Vector2f(50+j*20,50+i*20));
          fenetre.draw(leftWallDraw);
        }
      }
    }
    
    fenetre.display(); // On affiche
  }
}
