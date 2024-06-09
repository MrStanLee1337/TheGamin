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

        std::vector<std::vector<int>> theMap;
        int leftStart, upStart;
        DWORD ticks = 0;// тики для анимации

        Character Bunny;
        MyMap mymap;

        std::pair<int, int> tilePos; // место появление персонажа относительно тайлов
        int tileW = 16;
        int tileH = 16;
        int BunnyOffset = 10;//сдвиг по вертикали (чтобы не казалось что ГГ ходит по самому краю берега)
        bool keyReleased = true;// одно нажатие - одно действие

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
            
		}
       
        void initcursor() {
            try {
                //Object texture("tiles\\cursor.png");
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
                pausemenu.push_back(std::make_unique<Object>("tiles\\author.png", WIDTH - 150, 0.9f * HEIGHT));
                pausemenu.push_back(std::make_unique<Object>("tiles\\assets.png", WIDTH - 150, 0.8f * HEIGHT));
                
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

        void changeBunnyTilePos(int offx, int offy) {
            
            int x = tilePos.first + offx;
            int y = tilePos.second + offy;
            if (x < 0 || x >= theMap[0].size()) return;
            if (y < 0 || y >= theMap.size()) return;
            if (theMap[y][x] == 1 || theMap[y][x] == 2 || theMap[y][x] == 3 || theMap[y][x] == 7) {
                tilePos = std::make_pair(x, y);
                Bunny.setPosition(leftStart + tileW * x, upStart + tileH * y - BunnyOffset);
            }
        }

        void pendingKeyboard() {
            if (event.type == sf::Event::KeyPressed) {
                
                if (event.key.code == sf::Keyboard::Escape) switchPause(); 
                else
                if (now == GAME && keyReleased) {
                    if (event.key.code == sf::Keyboard::A) {
                        Bunny.moveLeft();
                        changeBunnyTilePos(-1, 0);
                    } else  if (event.key.code == sf::Keyboard::D) {
                        Bunny.moveRight(); 
                        changeBunnyTilePos(1, 0);
                    } else if (event.key.code == sf::Keyboard::W) {
                        Bunny.moveUp();
                        changeBunnyTilePos(0, -1);
                    } else if (event.key.code == sf::Keyboard::S) {
                        Bunny.moveDown();
                        changeBunnyTilePos(0, 1);
                    }

                    keyReleased = false;
                }
               
            }
            else if (event.type == sf::Event::KeyReleased) {
                keyReleased = true;
            }
        }

        void genMap() {//функция генерации карты и получения основной информации о ней
            mymap.generateMap();
            theMap = mymap.getMap();
            leftStart = mymap.getStartIslandPointPxls().first;
            upStart = mymap.getStartIslandPointPxls().second;
            //Bunny.setPosition(mymap.getStartCharacterPxlsPoint().first, mymap.getStartCharacterPxlsPoint().second);

            tilePos = mymap.getStartCharacterTilePoint();
            int offsety = 10;
            Bunny.setPosition(tilePos.first * 16 + leftStart, tilePos.second * 16 + upStart - offsety);
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
                                if (ptr->getType() == "newmap") genMap();
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
            genMap();
            initmusic();
            //inittiles();
            initcursor();
            initpause();
        }

        void tickrate() {
            ++(ticks %= 600);//10 секунд с фреймрейтом 60
        }

        void animation() {
            if (!(ticks % 60)) {
                int offset = ticks < 200 ? -3 : ticks > 400 ?  3 : 0;
                if (now == PAUSE) {
                    for (auto& obj : pausemenu) {
                        if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                            ptr->setPosition(ptr->getPosition().x + offset, ptr->getPosition().y + offset);
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