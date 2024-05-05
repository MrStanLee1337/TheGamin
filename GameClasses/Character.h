#pragma once

#include <iostream>

class Character {

	private:
		//�������� ������
		Object down;// ������ ������ ������ ������ ������� ��� �������� ������
		Object up;
		Object left;
		Object right;
		Object afk;
		int posx, posy;
		int velocity;

		enum Direction {
			DOWN,
			UP,
			LEFT,
			RIGHT,
			AFK
		} direction = AFK;
	public:
		Character() {}
		~Character() {}
		void initAnimation(){
			posx = 500, posy = 500, velocity = 10;
			try {

				//down.setPosition(100,100);
				sf::Image movement;
				sf::Image part;
				part.create(16, 16);

				movement.loadFromFile("tiles\\movement.png");
				
				for (int i = 0; i < 4; i++) { 
					part.copy(movement, 0, 0, sf::IntRect(i * 16,  0, (i + 1) * 16,  16));
					down.addPicture(part);
					part.copy(movement, 0, 0, sf::IntRect(i * 16, 16, (i + 1) * 16, 32));
					up.addPicture(part);
					part.copy(movement, 0, 0, sf::IntRect(i * 16, 32, (i + 1) * 16, 48));
					left.addPicture(part);
					part.copy(movement, 0, 0, sf::IntRect(i * 16, 48, (i + 1) * 16, 64));
					right.addPicture(part);
				}

				part.copy(movement, 0, 0, sf::IntRect(0, 0, 16, 16));
				afk.addPicture(part);
				part.copy(movement, 0, 0, sf::IntRect(16, 0, 32, 16));
				afk.addPicture(part);

			} catch (const std::exception& ex) {
				std::cerr << ex.what() << '\n';
			}
		}

		void moveleft() {
			direction = LEFT;
			posx -= velocity;
		}

		void moveright() {
			direction = RIGHT;
			posx += velocity;
		}
		
		void moveup() {
			direction = UP;
			posy -= velocity;
		}

		void movedown() {
			direction = DOWN;
			posy += velocity;
		}

		void nextFrame() {
			//if down
			std::cout << "nextFrame" << '\n';
			switch (direction) {
				case DOWN:
					down.nextFrame();
					break;
				case UP:
					up.nextFrame();
					break;
				case LEFT:
					left.nextFrame();
					break;
				case RIGHT:
					right.nextFrame();
					break;
				case AFK:
					afk.nextFrame();
					break;
			}
		}	

		void draw(sf::RenderWindow& window) {
			switch (direction) {
				case DOWN:
					down.draw(window, posx, posy);
					break;
				case UP:
					up.draw(window, posx, posy);
					break;
				case LEFT:
					left.draw(window, posx, posy);
					break;
				case RIGHT:
					right.draw(window, posx, posy);
					break;
				case AFK:
					afk.draw(window, posx, posy);
					break;
				}
		}
};