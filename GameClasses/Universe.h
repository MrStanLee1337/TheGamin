#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Windows.h>

#include <vector>

#include <GameMusic.h>
#include <Object.h>


class Universe {
	private:
		gm::GameMusic sounds;
        std::vector<std::unique_ptr<Object>> tiles;
        std::vector<std::unique_ptr<Object>> pausemenu;
        sf::Cursor cursor;
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

        void initcursor(sf::RenderWindow& window) {
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
        
	public:
		Universe()  {}
        void playMusic() {
            sounds.playMusic(true);
            //sounds.playSound(TREE);
        }
        void playSound(int lpName) {
            sounds.playSound(lpName);
        }
        void init(sf::RenderWindow& window) {
            initmusic();
            inittiles();
            initcursor(window);
        }

        void pause() {
        
        }

        void draw(sf::RenderWindow& window) {
            //auto botptr = dynamic_cast<Bot*>(&*obj)
            
            for (auto& obj : tiles) //*obj.draw(window);
            {
                if(auto botptr = dynamic_cast<Object*>(&*obj))
                    if(botptr->isVisible())
                        botptr->draw(window);
            }
        }

};