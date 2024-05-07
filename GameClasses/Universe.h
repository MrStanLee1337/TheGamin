#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

#include <vector>

#include <GameMusic.h>
#include <Object.h>
#include <Character.h>
#include <MyMap.h>

class Universe {
	private:

        int WIDTH, HEIGHT;
		gm::GameMusic sounds;
        //std::vector<std::unique_ptr<Object>> tiles;
        std::vector<std::unique_ptr<Object>> pausemenu;
        sf::RenderWindow& window;
        
        sf::Cursor cursor;
        sf::Event event;

        DWORD ticks = 0;// тики для анимации

        Character Bunny;
        MyMap mymap;

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
        /*
        void inittiles(){
            try {
                tiles.push_back(std::make_unique<Object>("tiles\\backwater.png"));
            }
            catch (const std::exception& ex){
                std::cerr << ex.what() << '\n';
            }
        }
        */
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
                pausemenu.push_back(std::make_unique<Object>("tiles\\play.png", WIDTH / 2 - 110, HEIGHT / 6 * 3, "play"));
                pausemenu.push_back(std::make_unique<Object>("tiles\\exit.png", WIDTH / 2 - 110, HEIGHT / 6 * 5, "exit"));
                pausemenu.push_back(std::make_unique<Object>("tiles\\title.png", WIDTH / 2 - 260, HEIGHT / 12, "title"));
                pausemenu.push_back(std::make_unique<Object>("tiles\\newmap.png", WIDTH / 2 - 110, HEIGHT / 6 * 4, "newmap"));
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
                if (event.key.code == sf::Keyboard::Escape) switchPause(); 
                else
                if (now == GAME) {
                    if (event.key.code == sf::Keyboard::A) Bunny.moveleft(); 
                    else
                    if (event.key.code == sf::Keyboard::D) Bunny.moveright(); 
                    else
                    if (event.key.code == sf::Keyboard::W) Bunny.moveup(); 
                    else
                    if (event.key.code == sf::Keyboard::S) Bunny.movedown();
                }
            }
        }

        void pendingMouse() {
            if (event.type == sf::Event::MouseButtonPressed) {
                
                float x = sf::Mouse::getPosition(window).x;
                float y = sf::Mouse::getPosition(window).y;
                if (now == PAUSE) {
                    for (auto& obj : pausemenu) {
                        if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                            if (ptr->isClicked(x, y)) {
                                if (ptr->getType() == "exit") exit();
                                if (ptr->getType() == "play") game();
                                if (ptr->getType() == "newmap") mymap.generateMap();
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
            Bunny.initAnimation();
            mymap.initTiles();
            initmusic();
            //inittiles();
            initcursor();
            initpause();
        }

        void tickrate() {
            ++(ticks %= 600);//10 секунд
        }

        void animation() {
            if (!(ticks % 60)) {
                int offset = ticks < 200 ? 3 : ticks > 400 ? -3 : 0;
                if (now == PAUSE) {
                    for (auto& obj : pausemenu) {
                        if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                            ptr->setPosition(ptr->getPosition().x, ptr->getPosition().y + offset);
                        }
                    }
                }
            }
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
            /*
            for (auto& obj : tiles) {
                if(auto ptr = dynamic_cast<Object*>(&*obj))
                    if(ptr->isVisible())
                        ptr->draw(window);
            }
            */
            mymap.draw(window);

            if (!(ticks % 20)) Bunny.nextFrame();
            Bunny.draw(window);
            
            if (now == PAUSE) {
                for (auto& obj : pausemenu) {
                    if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                        ptr->draw(window);
                    }
                }
            }
            
        }

};