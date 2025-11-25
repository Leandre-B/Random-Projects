#include <iostream>
#include <array>

/*
  3
4 0 1
  2
  5

*/
class RubiksCube{
    public:
        using Matrice = std::array<std::array<char,3>,3>;
        struct Face{
            Matrice data;
            char couleur;
        };

        using Cube = std::array<Face,6>;
        Cube cube;

        RubiksCube(){
            init(cube);
            for(int i=0; i<6; ++i)
                remplir(cube[i]);
        }

        void afficher(){
            for(int i=0; i<6; ++i){
                affFace(cube[i]);
                std::cout<<"\n";
            }
        }

        void move(std::string mv){
            if(mv=="R")
                move_R();
            else if(mv=="R\'")
                move_R_prime();
            else if(mv=="L")
                move_L();
            else if(mv=="L\'")
                move_L_prime();
            else if(mv=="U")
                move_U();
            else if(mv=="U\'")
                move_U_prime();
            else if(mv=="D")
                move_D();
            else if(mv=="D\'")
                move_D_prime();
            else if(mv=="F")
                move_F();
            else if(mv=="F\'")
                move_F_prime();
            else if(mv=="B")
                move_B();
            else if(mv=="B\'")
                move_B_prime();
            else
                std::cout<<"mouvement inconnue\n";
        }

        void parse(std::string moves){
            moves+=" ";
            std::string dpl="";
            for(uint i=0; i<moves.length(); ++i){
                if(moves[i]!=' ')
                    dpl+=moves[i];
                else{
                    char c = dpl.back();
                    if(int(c)>= 48 and int(c)<=57){
                        dpl.pop_back();
                        for(int j=0; j<int(c)-48; ++j)
                            move(dpl);
                    }else 
                        move(dpl);
                    dpl="";
                }
            }
        }

    private : 
        void init(Cube & cube){
            cube[0].couleur = 'v';
            cube[1].couleur = 'r';
            cube[2].couleur = 'j';
            cube[3].couleur = 'w';
            cube[4].couleur = 'o';
            cube[5].couleur = 'b';
        }
        void remplir(Face & face){
            for(int i=0; i<3;++i)
                for(int j=0; j<3; ++j)
                    face.data[i][j]=face.couleur;
        }

        void affFace(Face face){
            for(int i=0; i<3;++i){
                for(int j=0; j<3; ++j){
                    std::cout<<face.data[j][i];
                    std::cout<<" ";
                }
                std::cout<<"\n";
            }
        }

        void affVide(){
            for(int i=0; i<3;++i){
                for(int j=0; j<3; ++j){
                    std::cout<<" ";
                    std::cout<<" ";
                }
                std::cout<<"\n";
            }
        }
        void move_R(){
            Matrice aux;
            aux=cube[0].data;
            for(int i=0; i<3; ++i){
                cube[0].data[2][i]=cube[2].data[2][i];
            }
            for(int i=0; i<3; ++i){
                cube[2].data[2][i]=cube[5].data[2][i];
            }
            for(int i=0; i<3; ++i){
                cube[5].data[2][i]=cube[3].data[2][i];
            }
            for(int i=0; i<3; ++i){
                cube[3].data[2][i]=aux[2][i];
            }

            rotateClockWise(1);
        }

        void move_R_prime(){
            Matrice aux;
            aux=cube[0].data;
            for(int i=0; i<3; ++i){
                cube[0].data[2][i]=cube[3].data[2][i];
            }
            for(int i=0; i<3; ++i){
                cube[3].data[2][i]=cube[5].data[2][i];
            }
            for(int i=0; i<3; ++i){
                cube[5].data[2][i]=cube[2].data[2][i];
            }
            for(int i=0; i<3; ++i){
                cube[2].data[2][i]=aux[2][i];
            }

            rotateCounterClockWise(1);
        }

        void move_L(){
            Matrice aux;
            aux=cube[0].data;
            for(int i=0; i<3; ++i){
                cube[0].data[0][i]=cube[3].data[0][i];
            }
            for(int i=0; i<3; ++i){
                cube[3].data[0][i]=cube[5].data[0][i];
            }
            for(int i=0; i<3; ++i){
                cube[5].data[0][i]=cube[2].data[0][i];
            }
            for(int i=0; i<3; ++i){
                cube[2].data[0][i]=aux[0][i];
            }

            rotateClockWise(4);
            
        }

        void move_L_prime(){
            Matrice aux;
            aux=cube[0].data;
            for(int i=0; i<3; ++i){
                cube[0].data[0][i]=cube[2].data[0][i];
            }
            for(int i=0; i<3; ++i){
                cube[2].data[0][i]=cube[5].data[0][i];
            }
            for(int i=0; i<3; ++i){
                cube[5].data[0][i]=cube[3].data[0][i];
            }
            for(int i=0; i<3; ++i){
                cube[3].data[0][i]=aux[0][i];
            }

            rotateCounterClockWise(4);
        }

        void move_U(){
            Matrice aux;
            aux=cube[0].data;
            for(int i=0; i<3; ++i){
                cube[0].data[i][0]=cube[1].data[i][0];
            }
            for(int i=0; i<3; ++i){
                cube[1].data[i][0]=cube[5].data[2-i][2];
            }
            for(int i=0; i<3; ++i){
                cube[5].data[2-i][2]=cube[4].data[i][0];
            }
            for(int i=0; i<3; ++i){
                cube[4].data[i][0]=aux[i][0];
            }

            rotateClockWise(3);
        }

        void move_U_prime(){
            Matrice aux;
            aux=cube[0].data;
            for(int i=0; i<3; ++i){
                cube[0].data[i][0]=cube[4].data[i][0];
            }
            for(int i=0; i<3; ++i){
                cube[4].data[i][0]=cube[5].data[2-i][2];
            }
            for(int i=0; i<3; ++i){
                cube[5].data[2-i][2]=cube[1].data[i][0];
            }
            for(int i=0; i<3; ++i){
                cube[1].data[i][0]=aux[i][0];
            }

            rotateCounterClockWise(3);
        }

        void move_D(){
            Matrice aux;
            aux=cube[0].data;
            for(int i=0; i<3; ++i){
                cube[0].data[i][2]=cube[4].data[i][2];
            }
            for(int i=0; i<3; ++i){
                cube[4].data[i][2]=cube[5].data[2-i][0];
            }
            for(int i=0; i<3; ++i){
                cube[5].data[2-i][0]=cube[1].data[i][2];
            }
            for(int i=0; i<3; ++i){
                cube[1].data[i][2]=aux[i][2];
            }

            rotateClockWise(2);
        }

        void move_D_prime(){
            Matrice aux;
            aux=cube[0].data;
            for(int i=0; i<3; ++i){
                cube[0].data[i][2]=cube[1].data[i][2];
            }
            for(int i=0; i<3; ++i){
                cube[1].data[i][2]=cube[5].data[2-i][0];
            }
            for(int i=0; i<3; ++i){
                cube[5].data[2-i][0]=cube[4].data[i][2];
            }
            for(int i=0; i<3; ++i){
                cube[4].data[i][2]=aux[i][2];
            }

            rotateCounterClockWise(2);
        }

        void move_F(){
            Matrice aux;
            aux=cube[3].data;
            for(int i=0; i<3; ++i){
                cube[3].data[i][2]=cube[4].data[2][2-i];
            }
            for(int i=0; i<3; ++i){
                cube[4].data[2][i]=cube[2].data[i][0];
            }
            for(int i=0; i<3; ++i){
                cube[2].data[i][0]=cube[1].data[0][2-i];
            }
            for(int i=0; i<3; ++i){
                cube[1].data[0][i]=aux[i][2];
            }

            rotateClockWise(0);
        }

        void move_F_prime(){
            Matrice aux;
            aux=cube[3].data;
            for(int i=0; i<3; ++i){
                cube[3].data[i][2]=cube[1].data[0][i];
            }
            for(int i=0; i<3; ++i){
                cube[1].data[0][i]=cube[2].data[2-i][0];
            }
            for(int i=0; i<3; ++i){
                cube[2].data[i][0]=cube[4].data[2][i];
            }
            for(int i=0; i<3; ++i){
                cube[4].data[2][i]=aux[2-i][2];
            }

            rotateCounterClockWise(0);
        }

        void move_B(){
            Matrice aux;
            aux=cube[3].data;
            for(int i=0; i<3; ++i){
                cube[3].data[i][0]=cube[1].data[2][i];
            }
            for(int i=0; i<3; ++i){
                cube[1].data[2][i]=cube[2].data[2-i][2];
            }
            for(int i=0; i<3; ++i){
                cube[2].data[i][2]=cube[4].data[0][i];
            }
            for(int i=0; i<3; ++i){
                cube[4].data[0][i]=aux[2-i][0];
            }

            rotateClockWise(5);
        }

        void move_B_prime(){
            Matrice aux;
            aux=cube[3].data;
            for(int i=0; i<3; ++i){
                cube[3].data[i][0]=cube[4].data[0][2-i];
            }
            for(int i=0; i<3; ++i){
                cube[4].data[0][i]=cube[2].data[i][2];
            }
            for(int i=0; i<3; ++i){
                cube[2].data[i][2]=cube[1].data[2][2-i];
            }
            for(int i=0; i<3; ++i){
                cube[1].data[2][i]=aux[i][0];
            }

            rotateCounterClockWise(5);
        }


        void rotateClockWise(uint f){
            //coin
            Matrice aux = cube[f].data;
            cube[f].data[0][0] = cube[f].data[0][2];
            cube[f].data[0][2] = cube[f].data[2][2];
            cube[f].data[2][2] = cube[f].data[2][0];
            cube[f].data[2][0] = aux[0][0];

            //edge
            aux = cube[f].data;
            cube[f].data[0][1] = cube[f].data[1][2];
            cube[f].data[1][2] = cube[f].data[2][1];
            cube[f].data[2][1] = cube[f].data[1][0];
            cube[f].data[1][0] = aux[0][1];
        }
        void rotateCounterClockWise(uint f){
            //coin
            Matrice aux = cube[f].data;
            cube[f].data[0][0] = cube[f].data[2][0];
            cube[f].data[2][0] = cube[f].data[2][2];
            cube[f].data[2][2] = cube[f].data[0][2];
            cube[f].data[0][2] = aux[0][0];

            //edge
            aux = cube[f].data;
            cube[f].data[0][1] = cube[f].data[1][0];
            cube[f].data[1][0] = cube[f].data[2][1];
            cube[f].data[2][1] = cube[f].data[1][2];
            cube[f].data[1][2] = aux[0][1];
        }
};