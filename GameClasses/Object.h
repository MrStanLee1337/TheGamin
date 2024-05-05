#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <cstdio>

class Object {
	private:
        bool visibility = true;
        sf::Sprite sprite;
        std::vector<sf::Texture> animation;
        int frame = 0;
        const char* type = ""; // хранит тип объекта (кнопки, объекта) для определения с чем мы взимодействуем
        bool savePngToMemory(const char* filename) {
            FILE* file;
            if (fopen_s(&file, filename, "rb"))  throw std::runtime_error("Can't open png file.\n");
            if (file) {
                fseek(file, 0, SEEK_END);
                long fileSize = ftell(file);
                fseek(file, 0, SEEK_SET);

                char* fileBuffer = (char*)malloc(fileSize);
                fread(fileBuffer, 1, fileSize, file);
                fclose(file);

                void* memoryBuffer = malloc(fileSize);
                memcpy(memoryBuffer, fileBuffer, fileSize);

                sf::Texture texture;
                texture.loadFromMemory(memoryBuffer, fileSize);
                animation.push_back(texture);
                sprite.setTexture(animation[animation.size() - 1]);

                free(fileBuffer);
                free(memoryBuffer);
            }
            return true;
        }
	public:
        Object() {}
        ~Object() {}
        Object(const char* filename, const char* type = nullptr) throw() : type(type) {
            addPicture(filename); 
        }
        Object(const char* filename, int x, int y, const char* type = nullptr) throw() : type(type) {
            addPicture(filename);
            sprite.setPosition((float)x, (float)y);
        }
        

        void addPicture(const char* filename) throw() {// download pic to sprite
            if (!savePngToMemory(filename)) throw std::runtime_error("Can't open png file.\n");
        }

        void addPicture(sf::Image image) {
            sf::Texture texture;
            texture.loadFromImage(image);
            animation.push_back(texture);
            sprite.setTexture(texture);   
        }
        
		

        void setPosition(int x, int y) {
            sprite.setPosition(float(x), float(y));
        }

        void setPosition(sf::Vector2f xy) {
            sprite.setPosition(xy);
        }

        sf::Vector2f getPosition() {
            return sprite.getPosition();
        }

        void draw(sf::RenderWindow& window, int x = -1, int y = -1) {
            if (x != -1 && y != -1) sprite.setPosition(x, y);
            window.draw(sprite);
        }

        void nextFrame() {
            ++frame;
            if (frame == animation.size()) frame = 0;
            sprite.setTexture(animation[frame]);
        }


        bool isVisible() { return visibility; }
        bool setVisibility(bool b) { visibility = b; }
        sf::Texture getTexture() { return animation[frame]; }

        bool isClicked(int x, int y) {
            return sprite.getGlobalBounds().contains(x, y);
        }

        const char* getType() {
            return type;
        }
};

