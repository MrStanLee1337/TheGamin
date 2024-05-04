#pragma once

#include <iostream>

class Character {

	private:
		//анимации ходьбы
		bool isMoving = true;
		Object down;
		Object up;
		Object left;
		Object right;
		sf::Texture texture;
		sf::Sprite sprite;
		Object afk;
	public:
		Character() {}
		~Character() {}
		void initAnimation(){
			try {
				down.setPosition(100,100);
				sf::Image movement;
				sf::Image part;
				part.create(16, 16);

				movement.loadFromFile("tiles\\movement.png");
				
				
				for (int i = 0; i < 4; i++) {
					part.copy(movement, 0, 0, sf::IntRect(i*16,i*16,(i+1)*16,(i+1)*16));
					afk.addPicture(part);
				}
			}
			catch (const std::exception& ex) {
				std::cerr << ex.what() << '\n';
			}
		}

		void nextFrame() {
			//if down
			std::cout << "nextFrame" << '\n';
			afk.nextFrame();
		}	

		void draw(sf::RenderWindow& window) {
			afk.draw(window);
		}
};