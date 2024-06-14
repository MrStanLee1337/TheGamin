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
        std::vector<std::unique_ptr<Object>> collections;//объекты коллекций сбора урожая

        //std::vector<std::unique_ptr<Object>> envelope;//письмо
        std::unique_ptr<Object> envelope;

        sf::RenderWindow& window;
        
        sf::Cursor cursor;
        sf::Event event;

        std::vector<std::vector<int>> theMap;
        int leftStart, upStart;
        DWORD ticks = 0;// тики для анимации

        Character Bunny;
        MyMap mymap;

        std::pair<int, int> heroTilePos; // место появление персонажа относительно тайлов
        int tileW = 16;
        int tileH = 16;
        int BunnyOffset = 10;//сдвиг по вертикали (чтобы не казалось что ГГ ходит по самому краю берега)

        DWORD keyPressed = 0;// одно нажатие - одно действие раз в X тиков

        enum GameState {
            PAUSE,
            GAME,
            EXIT
        } now = PAUSE;

        size_t countOfRes = 0;

		void initmusic() {
            try {
                sounds.SetMusic(BACK_MUSIC, MUSICFILE);
                //sounds.AddSound(TREE, MUSICFILE);
                //sounds.AddSound(SHSH, MUSICFILE);
                sounds.AddSound(AXE, MUSICFILE);
                sounds.AddSound(SHURSH, MUSICFILE);
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
            
            int x = heroTilePos.first + offx;
            int y = heroTilePos.second + offy;
            if (x < 0 || x >= theMap[0].size()) return;
            if (y < 0 || y >= theMap.size()) return;
            if (theMap[y][x] == 1 || theMap[y][x] == 2 || theMap[y][x] == 3 || theMap[y][x] == 7) {
                heroTilePos = std::make_pair(x, y);
                Bunny.setPosition(leftStart + tileW * x, upStart + tileH * y - BunnyOffset);
            }
        }

        void activeInteraction(std::string typeOfInteract) {
            if (typeOfInteract == "tree") {
                for (auto& obj : collections) {
                    if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                        if (ptr->getType() == "collecttree") {
                            ptr->nextFrame();
                            sounds.playSound(SHURSH);
                        }
                    }
                }
            } else if (typeOfInteract == "bush") {
                for (auto& obj : collections) {
                    if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                        if (ptr->getType() == "collectbush") {
                            ptr->nextFrame();
                            sounds.playSound(SHURSH);
                        }
                    }
                }
            } else if (typeOfInteract == "stump") {
                for (auto& obj : collections) {
                    if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                        if (ptr->getType() == "collectwood") {
                            ptr->nextFrame();
                            sounds.playSound(AXE);
                        }
                    }
                }
            } else if (typeOfInteract == "boat") {
                genMap();
            }
        }

        void interaction() {
            int dx[] = { -1, 1, 0, 0 };
            int dy[] = { 0, 0, -1, 1 };
            std::string typeOfInteract = "";
            for (int i = 0; i < std::size(dx); i++) {
                int newx = dx[i] + heroTilePos.first;
                int newy = dy[i] + heroTilePos.second;
                if (newx >= 0 && newx < theMap[0].size() && newy >= 0 && newy < theMap.size()) {
                    newx = leftStart + tileW * newx;
                    newy = upStart + tileH * newy;
                    typeOfInteract = mymap.typeOfInteraction(newx, newy);
                    if (typeOfInteract != "") break;
                    //_debug(countOfRes);
                }
            }

            activeInteraction(typeOfInteract);

        }

        void pendingKeyboard() {
            if (event.type == sf::Event::KeyPressed) {
                
                if (event.key.code == sf::Keyboard::Escape) switchPause(); 
                else
                if (now == GAME && keyPressed == 0) {
                    //keyPressed = 30;
                    if (event.key.code == sf::Keyboard::A) {
                        Bunny.moveLeft();
                        changeBunnyTilePos(-1, 0);
                        //--keyPressed;
                    } else  if (event.key.code == sf::Keyboard::D) {
                        Bunny.moveRight(); 
                        changeBunnyTilePos(1, 0);
                        //--keyPressed;
                    } else if (event.key.code == sf::Keyboard::W) {
                        Bunny.moveUp();
                        changeBunnyTilePos(0, -1);
                        //--keyPressed;
                    } else if (event.key.code == sf::Keyboard::S) {
                        Bunny.moveDown();
                        changeBunnyTilePos(0, 1);
                       // --keyPressed;
                    } else if (event.key.code == sf::Keyboard::F) {
                        interaction();
                        //--keyPressed;
                    }

                }
                ++keyPressed %= 5;
            }
            else if (event.type == sf::Event::KeyReleased) {
                keyPressed = 0;
            }
        }

        void genMap() {//функция генерации карты и получения основной информации о ней
            mymap.generateMap();
            theMap = mymap.getMap();
            leftStart = mymap.getStartIslandPointPxls().first;
            upStart = mymap.getStartIslandPointPxls().second;
            //Bunny.setPosition(mymap.getStartCharacterPxlsPoint().first, mymap.getStartCharacterPxlsPoint().second);

            heroTilePos = mymap.getStartCharacterTilePoint();
            //int offsety = 10;
            Bunny.setPosition(heroTilePos.first * 16 + leftStart, heroTilePos.second * 16 + upStart - BunnyOffset);
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
                if (now == GAME && envelope->isVisible() == true) {
                    if (envelope->isLastFrame()) envelope->setVisibility(false);
                    if (envelope->isClicked(x, y)) envelope->nextFrame();
                }
            }
        }

        void initHarvestCollections() {//счётчик сбора урожая
            void* filebuffer = nullptr;
            long filesize = 0;
            sf::Image image;
            
            if (!getDataFromImage("tiles\\harvestbush.png", filebuffer, filesize)) throw std::runtime_error("Can't open harvestbush.png file");
            
            image.loadFromMemory(filebuffer, filesize);
            std::vector<sf::Image> collectbush;
            for (int i = 0; i <= 5; i++) {
                sf::Image harvestBushImage;
                harvestBushImage.create(150, 35);
                harvestBushImage.copy(image, 0, 0, sf::IntRect(0, i * 35, 150, (i + 1) * 35));
                collectbush.push_back(harvestBushImage);
            }
            collections.push_back(std::make_unique<Object>(collectbush, WIDTH - 160, 30, "collectbush", false));

            if (!getDataFromImage("tiles\\harvesttree.png", filebuffer, filesize)) throw std::runtime_error("Can't open harvesttree.png file");

            image.loadFromMemory(filebuffer, filesize);
            std::vector<sf::Image> collecttree;
            for (int i = 0; i <= 3; i++) {
                sf::Image harvestTreeImage;
                harvestTreeImage.create(150, 35);
                harvestTreeImage.copy(image, 0, 0, sf::IntRect(0, i * 35, 150, (i + 1) * 35));
                collecttree.push_back(harvestTreeImage);
            }
            collections.push_back(std::make_unique<Object>(collecttree, WIDTH - 160, 70, "collecttree", false));

            if (!getDataFromImage("tiles\\harvestwood.png", filebuffer, filesize)) throw std::runtime_error("Can't open harvestwood.png file");

            image.loadFromMemory(filebuffer, filesize);
            std::vector<sf::Image> collectwood;
            for (int i = 0; i <= 3; i++) {
                sf::Image harvestWoodImage;
                harvestWoodImage.create(150,35);
                harvestWoodImage.copy(image, 0, 0, sf::IntRect(0, i * 35, 150, (i + 1) * 35));
                collectwood.push_back(harvestWoodImage);
            }
            collections.push_back(std::make_unique<Object>(collectwood, WIDTH - 160, 110, "collectwood", false));
        }

        void initEnvelope() {
            void* filebuffer = nullptr;
            long filesize = 0;
            if (!getDataFromImage("tiles\\envelope.png", filebuffer, filesize)) throw std::runtime_error("Can't open envelope.png file");
            sf::Image image;
            image.loadFromMemory(filebuffer, filesize);
            std::vector<sf::Image> collect;
            for (int i = 0; i < 3; i++) {
                sf::Image img;
                img.create(455, 539);
                img.copy(image, 0, 0, sf::IntRect(0, 540 * i, 455, (i + 1) * 540));
                collect.push_back(img);
            }
            //Object obj = Object();
            envelope = std::make_unique<Object>(collect, WIDTH / 4, HEIGHT / 6, "envelope", false);
            //envelope.push_back(std::make_unique<Object> (collect, WIDTH / 4, HEIGHT / 6, "envelope", false));
            //envelope = std::make_unique<Object>(collect, WIDTH / 4, HEIGHT / 6, "envelope", false);
        }

	public:
        
        Universe(sf::RenderWindow& window, int WIDTH, int HEIGHT) :window(window), WIDTH(WIDTH), HEIGHT(HEIGHT) {}

        void playMusic() {
            sounds.playMusic(true);
        }

        //void playSound(int lpName) {
        //    sounds.playSound(lpName);
        //}

        
        void init() {
            
            Bunny.initAnimation();
            mymap.initTiles();
            genMap();
            initmusic();
            initcursor();
            initpause();
            initHarvestCollections();
            initEnvelope();
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
            else if (now == GAME) {
                for (auto& obj : collections) {
                    if (auto ptr = dynamic_cast<Object*>(&*obj)) {
                        ptr->draw(window);
                    }
                }
                if (envelope->isVisible() == true) {
                    envelope->draw(window);
                }
            }
            
        }

};