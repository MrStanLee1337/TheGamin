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

		void initmusic() throw() {
            try {
                sounds.SetMusic(BACK_MUSIC, MUSICFILE);
                sounds.AddSound(TREE, MUSICFILE);
            }
            catch (const std::exception& ex) {
                std::cerr << ex.what() << '\n';
            }
            //sounds.playMusic(true);
		}

        void inittiles() throw(){
            try {
                tiles.push_back(std::make_unique<Object>("tiles\\backwater.png"));
            }
            catch (const std::exception& ex){
                std::cerr << ex.what() << '\n';
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
        void init() {
            initmusic();
            inittiles();
        }
        void draw(sf::RenderWindow& window) {
            //auto botptr = dynamic_cast<Bot*>(&*obj)
            
            for (auto& obj : tiles) //*obj.draw(window);
            {
                if(auto botptr = dynamic_cast<Object*>(&*obj))
                    botptr->draw(window);
            }
        }

};