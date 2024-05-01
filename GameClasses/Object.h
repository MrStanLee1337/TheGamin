#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <cstdio>

class Object {
	private:
        sf::Sprite sprite;
        sf::Texture texture;
        void savePngToMemory(const char* filename) {
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
        }
	public:
		Object(const char* filename) {
            try {
                savePngToMemory(filename);
            } catch (const std::exception ex) {
                std::cerr << ex.what() << '\n';
            }
        }
		~Object() {}
        void draw(sf::RenderWindow& window) {
            window.draw(sprite);
        }

};

