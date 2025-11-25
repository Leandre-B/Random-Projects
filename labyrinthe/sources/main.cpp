#include "gen_laby.h"
#include "draw.h"



int main(){
  int m = 40, n=30;
  bool** Laby;
  bool** topWall;
  bool** leftWall;
  
  genere_laby(Laby,topWall,leftWall,m,n);
  draw_laby(topWall,leftWall,m,n);



  return 0;
}
