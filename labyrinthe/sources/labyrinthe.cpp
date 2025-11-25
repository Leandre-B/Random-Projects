#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

struct Coord{
  int x;
  int y;
  char dir;
};

void init(bool**  & wall,int m , int n,bool is_wall){
  wall = new bool* [m+1];
  for (int i = 0;i<m;++i){
    wall[i]=new bool [n];

    //init boolean
    for(int k = 0;k<n;++k)
      wall[i][k]=is_wall;
  }
}

void affiche(bool** L,int m ,int n){
  for(int i = 0;i<m;++i){
    for(int j = 0;j<n;++j)
      std::cout<<L[i][j]<<" ";
    std::cout<<std::endl;
  }
}


void drawLabyConsole(bool** topWall,bool** leftWall,int m ,int n){
  //affiche murs du haut
  for(int i = 0;i<n;++i){
    if(topWall[0][i]==true) std::cout<<" -";
    else std::cout<<"  ";
  }
  std::cout<<std::endl;
  
  for(int i = 0;i<m;++i){
    for(int j = 0;j<n+1;++j){
      if(leftWall[i][j]==true) std::cout<<"| ";
      else std::cout<<"  ";
    }
    std::cout<<std::endl;
    
    for(int j = 0;j<n;++j){
      if(topWall[i+1][j]==true) std::cout<<" -";
      else std::cout<<"  ";
    }
    std::cout<<std::endl;
  }
      
}


int nbVoisineNonVisite(bool** L,int x,int y, int m,int n,Coord* & voisines){
  int p = -1;
  
  if(x-1>=0)
    if(L[x-1][y]==false){
     ++p;
     voisines[p].x=x-1;
     voisines[p].y=y;
     voisines[p].dir='h';
 }
  if(x+1<m)
    if(L[x+1][y]==false){
     ++p;
     voisines[p].x=x+1;
     voisines[p].y=y;
     voisines[p].dir='b';
 }
  if(y-1>=0)
    if(L[x][y-1]==false){
     ++p;
     voisines[p].x=x;
     voisines[p].y=y-1;
     voisines[p].dir='g';
 }
  if(y+1<n)
    if(L[x][y+1]==false){
     ++p;
     voisines[p].x=x;
     voisines[p].y=y+1;
     voisines[p].dir='d';
 }
  //retourn le NOMBRE de voisines non visitée
  return p+1;
}

void openWall(int x, int y,char dir,bool** & topWall,bool** & leftWall){
  switch (dir){
    case 'd' : 
      leftWall[x][y+1]=false;
      break;
    case 'g' :
      leftWall[x][y]=false;
      break;
    case 'h':
      topWall[x][y]=false;
      break;
    case 'b':
      topWall[x+1][y]=false;
      break;
    }
}


void makeLabyAux(bool** & L,bool** & topW,bool** & leftW,int x,int y,int m,int n){
  //on visite la case
  L[x][y]=true;
  //std::cout<<"visite L ["<<x<<"]["<<y<<"]"<<std::endl;

  //on regarde les voisines non visitée
  Coord* voisines = new Coord [4];
  int nbVoiNonVisisite=nbVoisineNonVisite(L,x,y,m,n,voisines);

  //on se déplace sur une voisines non visitée random
  while(nbVoiNonVisisite>0){
    int rdm_vois = rand()%nbVoiNonVisisite;
    //std::cout<<"nb voisines : "<<nbVoiNonVisisite<<"   "<<"rdm voisines : "<<rdm_vois<<std::endl;
    int v_x = voisines[rdm_vois].x;
    int v_y = voisines[rdm_vois].y;

    //std::cout<<"deplace vers "<<v_x<<" "<<v_y<<std::endl;
    
    openWall(x,y,voisines[rdm_vois].dir,topW,leftW);
    //drawLabyConsole(topW,leftW,m,n);
    makeLabyAux(L,topW,leftW,v_x,v_y,m,n);
    nbVoiNonVisisite=nbVoisineNonVisite(L,x,y,m,n,voisines);
  }
}
    
void makeLaby(bool** & L,bool** & topW,bool** & leftW, int m, int n){
  srand(time(nullptr));
  int seed = 313251408;
  std::cout<<"Seed : "<<seed<<std::endl;
  srand(seed);
  
  makeLabyAux(L,topW,leftW,rand()%m,rand()%n,m,n);
}

int main(){
  int m = 30; //lignes
  int n = 40; //

  bool** Laby;
  bool** topWall;
  bool** leftWall;
  
  Laby= new bool* [m];
  for (int i = 0;i<m;++i){
    Laby[i]=new bool [n];
  }
  init(Laby,m,n,false);
  init(topWall,m+1,n,true);
  init(leftWall,m,n+1,true);
  
  makeLaby(Laby,topWall,leftWall,m,n);
  //drawLabyConsole(topWall,leftWall,m,n);



  //entrée et sortie rdm
  topWall[0][rand()%n]=false;
  topWall[m][rand()%n]=false;


  // Création de la fenêtre
  sf::RenderWindow fenetre(sf::VideoMode({1000, 1000}), "Labyrinthe");
  // Fixe le nombre d'images par seconde
  // sur python : sleep(0.01)
  // cela équivaut à 100 images par seconde :
  //fenetre.setFramerateLimit(30);

  // Premier dessin de la fenêtre
  fenetre.clear(); // On efface tout
  fenetre.display(); // On affiche

  sf::Event event;

  sf::RectangleShape topWallDraw;
  sf::RectangleShape leftWallDraw;
  topWallDraw.setSize(sf::Vector2f(20,1));
  leftWallDraw.setSize(sf::Vector2f(1,20));
  topWallDraw.setFillColor(sf::Color::White);
  leftWallDraw.setFillColor(sf::Color::White);


  while(fenetre.isOpen()) {
    while(fenetre.pollEvent(event)) { // boucle des événements
          
          if(event.type == sf::Event::Closed) {
              fenetre.close();
          }
    }
      //affiche murs du haut
    for(int i = 0;i<n;++i){
      if(topWall[0][i]==true){
        topWallDraw.setPosition(sf::Vector2f(i*20,0));
        fenetre.draw(topWallDraw);
      }
    }

    for(int i = 0;i<m;++i){
      for(int j = 0;j<n+1;++j){
        if(leftWall[i][j]==true){
          leftWallDraw.setPosition(sf::Vector2f(j*20,i*20));
          fenetre.draw(leftWallDraw);
        }
      }

      
      for(int j = 0;j<n;++j){
        if(topWall[i+1][j]==true){
          topWallDraw.setPosition(sf::Vector2f((j)*20,(i+1)*20));
          fenetre.draw(topWallDraw);
        }
      }
    
    }

    
    fenetre.display(); // On affiche
  }
}
