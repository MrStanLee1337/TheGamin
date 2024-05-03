#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

#include <vector>

#include <GameMusic.h>
#include <Object.h>


class Universe {
	private:

        int WIDTH, HEIGHT;
		gm::GameMusic sounds;
        std::vector<std::unique_ptr<Object>> tiles;
        std::vector<std::unique_ptr<Object>> pausemenu;
        sf::RenderWindow& window;
        
        sf::Cursor cursor;
        sf::Event event;
        

        enum GameState {
            PAUSE,
            GAME,
            EXIT
        } now = PAUSE;


		void initmusic() {
            try {
                sounds.SetMusic(BACK_MUSIC, MUSICFILE);
                sounds.AddSound(TREE, MUSICFILE);
            }
            catch (const std::exception& ex) {
                std::cerr << ex.what() << '\n';
            }
            //sounds.playMusic(true);
		}

        void inittiles(){
            try {
                tiles.push_back(std::make_unique<Object>("tiles\\backwater.png"));
            }
            catch (const std::exception& ex){
                std::cerr << ex.what() << '\n';
            }
        }

        void initcursor() {
            try {
                Object texture("tiles\\cursor.png");
                sf::Image image = texture.getTexture().copyToImage();
                int width = image.getSize().x; int height = image.getSize().y;
                cursor.loadFromPixels(image.getPixelsPtr(), sf::Vector2u(width,height), sf::Vector2u(0,0));
                window.setMouseCursor(cursor);
            }
            catch (const std::exception& ex) {
                std::cerr << ex.what();
            }
        }

        void initpause() {
            try {
                pausemenu.push_back(std::make_unique<Object>("tiles\\play.png", WIDTH / 2 - 110, HEIGHT / 2));
                pausemenu.push_back(std::make_unique<Object>("tiles\\exit.png", WIDTH / 2 - 110, HEIGHT / 3 * 2));
                pausemenu.push_back(std::make_unique<Object>("tiles\\title.png", WIDTH / 2 - 260, HEIGHT / 12));
            }   catch (const std::exception& ex) {
                std::cerr << ex.what() << '\n';
            }
        }
        
        void pause() {
            now = PAUSE;
        }
        void game() {
            now = GAME;
        }

        void switchPause() {
            now = now == PAUSE ? GAME : PAUSE;
        }

        void exit() {
            window.close();
        }

        void pendingKeyboard() {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    switchPause();
                }
            }
        }

        void pendingMouse() {
            if (event.type == sf::Event::MouseLeft) {
                float x = sf::Mouse::getPosition().x;
                float y = sf::Mouse::getPosition().y;
                if (now == PAUSE) {
                    for (auto& obj : pausemenu) {
                        if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                            if (ptr->isClicked(x, y)) {

                            }
                        }
                    }
                }
            }
        }

	public:

        Universe(sf::RenderWindow& window, int WIDTH, int HEIGHT) :window(window), WIDTH(WIDTH), HEIGHT(HEIGHT) {}

        void playMusic() {
            sounds.playMusic(true);
        }

        void playSound(int lpName) {
            sounds.playSound(lpName);
        }

        void init() {
            initmusic();
            inittiles();
            initcursor();
            initpause();
        }

        

        void pendingAction() {
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
                pendingKeyboard();
                pendingMouse();
                
            }
        }

        void state() {
            switch (now) {
                case PAUSE:
                    pause();
                    break;
                case GAME:
                    game();
                    break;
                case EXIT:
                    exit();
                    break;
            }
        }

        void draw() {
            for (auto& obj : tiles) {
                if(auto ptr = dynamic_cast<Object*>(&*obj))
                    if(ptr->isVisible())
                        ptr->draw(window);
            }
            if (now == PAUSE) {
                for (auto& obj : pausemenu) {
                    if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                        ptr->draw(window);
                    }
                }
            }
        }

};