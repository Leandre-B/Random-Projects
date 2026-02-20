# Rubik's Cube in OpenGL
Implémentation simple d'un rubik's cube 3D avec OpenGL.
Créé en suivant le tuto de learnopengl.com/

## Dependances :  
OpenGL et glfw3 3.4

## Build :  
``` console
mkdir build  
cd build  
cmake ..  
make  
./rubiks  
```

## Utilisation
On peut se mouvoir dans l'espace avec zsqd, espace pour monter et shift pour descendre  

En appuyant sur 't', on peut taper dans la console les mouvements que l'on veut faire (suit la notation officielle).  
Exemple :   
R R' D2 B'2  

**Remarque :** M, M', x, y et z ne sont pas supporté