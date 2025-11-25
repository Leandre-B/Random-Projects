#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <cmath>
#include <array>

const unsigned int HAUTEUR_FENETRE = 800;
const unsigned int LARGEUR_FENETRE = 800;


float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}


int main(){

	sf::Texture texture;
	texture.create(500,500);

	std::vector<sf::Uint8> pixels(500 * 500 * 4);
	
	
	srand(time(nullptr));
	std::array<std::array<std::pair<float, float>, 11>, 11> vecteurs;
	
	for(int i=0;i<11;++i){
		for(int j=0; j<11; ++j){
			vecteurs[i][j].first = 2.0f * rand()/RAND_MAX -1.0f;
			vecteurs[i][j].second = 2.0f * rand()/RAND_MAX -1.0f;
		}
	}
	
	for(int i=0;i<11;++i){
		for(int j=0; j<11; ++j){
			//std::cout<<vecteurs[i][j].first<<" "<<vecteurs[i][j].second<<std::endl;
		}
	}
	

	for (int i = 0; i < 500 * 500; ++i) {
		int pixelX = i % 500;
		int pixelY = i / 500;

		int cellSize = 50;
		int cellX = pixelX / cellSize;
		int cellY = pixelY / cellSize;

		float tx = (pixelX % cellSize) / float(cellSize);
		float ty = (pixelY % cellSize) / float(cellSize);

		float fadeX = fade(tx);
		float fadeY = fade(ty);

		float gX00 = vecteurs[cellX][cellY].first;
		float gY00 = vecteurs[cellX][cellY].second;
		float dX00 = tx;
		float dY00 = ty;
		float sX00 = gX00 * dX00 + gY00 * dY00;

		float gX10 = vecteurs[cellX + 1][cellY].first;
		float gY10 = vecteurs[cellX + 1][cellY].second;
		float dX10 = tx - 1.0f;
		float dY10 = ty;
		float sX10 = gX10 * dX10 + gY10 * dY10;

		float gX01 = vecteurs[cellX][cellY + 1].first;
		float gY01 = vecteurs[cellX][cellY + 1].second;
		float dX01 = tx;
		float dY01 = ty - 1.0f;
		float sX01 = gX01 * dX01 + gY01 * dY01;

		float gX11 = vecteurs[cellX + 1][cellY + 1].first;
		float gY11 = vecteurs[cellX + 1][cellY + 1].second;
		float dX11 = tx - 1.0f;
		float dY11 = ty - 1.0f;
		float sX11 = gX11 * dX11 + gY11 * dY11;

		float ix0 = lerp(sX00, sX10, fadeX);
		float ix1 = lerp(sX01, sX11, fadeX);
		float value = lerp(ix0, ix1, fadeY);

		// Normaliser value entre 0 et 1
		float effect = (value + 1.0f) / 2.0f;
		if (effect < 0) effect = 0;
		if (effect > 1) effect = 1;

		pixels[i * 4 + 0] = 255;
		pixels[i * 4 + 1] = static_cast<unsigned char>(255 * effect); 
		pixels[i * 4 + 2] = static_cast<unsigned char>(255 * effect); 
		pixels[i * 4 + 3] = 255;
}


	texture.update(pixels.data());

	sf::RenderWindow window(sf::VideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE), "Perlin");
	window.setFramerateLimit(120);
	
	sf::Sprite sprite(texture);
    sprite.setPosition(150, 150);
	
	window.clear(sf::Color::White);
	window.draw(sprite);
	window.display();
	
	while (window.isOpen()) {
		sf::Event event;
		
		while (window.pollEvent(event)) {
			
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		texture.update(pixels.data());
		sf::Sprite sprite(texture);
		sprite.setPosition(150, 150);
		
		window.clear(sf::Color::White);
		window.draw(sprite);
		window.display();
	}
	
	return 0;
}