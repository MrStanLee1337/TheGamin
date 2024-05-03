#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <cstdio>

class Object {
	private:
        bool visibility = true;
        sf::Sprite sprite;
        sf::Texture texture;
        int name = 0;
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

                texture.loadFromMemory(memoryBuffer, fileSize);
                sprite.setTexture(texture);

                free(fileBuffer);
                free(memoryBuffer);
            }
            return true;
        }
	public:
        Object() {}
		Object(const char* filename) throw(){
            setPicture(filename); 
        }
        Object(const char* filename, int x, int y) {
            setPicture(filename);
            sprite.setPosition((float)x, (float)y);
        }
        void setPosition(int x, int y) {
            sprite.setPosition(float(x), float(y));
        }

        void setPicture(const char* filename) throw() {
            if (!savePngToMemory(filename)) throw std::runtime_error("Can't open png file.\n");
        }

		~Object() {}

        void draw(sf::RenderWindow& window) {
            window.draw(sprite);
        }

        bool isVisible() { return visibility; }
        bool setVisibility(bool b) { visibility = b; }
        sf::Texture getTexture() { return texture; }

};

