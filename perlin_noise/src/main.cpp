#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <cmath>


const unsigned int HAUTEUR_FENETRE = 800;
const unsigned int LARGEUR_FENETRE = 800;



int main(){

	sf::Texture texture;
	texture.create(500,500);

	std::vector<sf::Uint8> pixels(500 * 500 * 4);

	for(int i=0; i<500*500; ++i){
		pixels[i*4 + 0]= 0;
		pixels[i*4 + 1]= 0;
		pixels[i*4 + 2]= 0;
		pixels[i*4 + 3]= 255;
	}

	texture.update(pixels.data());

	sf::RenderWindow window(sf::VideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE), "Particules");
	window.setFramerateLimit(120);
	
	sf::Sprite sprite(texture);
    sprite.setPosition(150, 150);
	
	window.clear(sf::Color::White);
	window.draw(sprite);
	window.display();
	
	sf::Clock clock;
	
	int i=0;
	while (window.isOpen()) {
		sf::Event event;
		sf::Time dt  = clock.getElapsedTime();
		clock.restart();
		
		while (window.pollEvent(event)) {
			
			if (event.type == sf::Event::Closed)
				window.close();
		}

		for(int t = 0; t<dt.asMilliseconds(); ++t){
			int k=0;
			for(int j=0; j<500; ++j){
				pixels[i*500*4 + j*4+2]=(pixels[i*500*4 + j*4+2]+dt.asMilliseconds()*100)%256;
				pixels[i*4 + j*100*k*4+0]=(pixels[i*4 + j*100*k*4+0]+dt.asMilliseconds()*10)%256;
				++k;
				if(k==6) k=1;
			}
			++i;
		}
		
		texture.update(pixels.data());
		sf::Sprite sprite(texture);
		sprite.setPosition(150, 150);
		
		window.clear(sf::Color::White);
		window.draw(sprite);
		window.display();
		
		if(i>500)
			i=0;
	}
	
	return 0;
}