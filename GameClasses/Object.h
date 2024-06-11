#pragma once
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <cstdio>

bool getDataFromImage(const char* filename, void*& buffer, long& size) {
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
        
        buffer = memoryBuffer;
        size = fileSize;
        
        /*
        sf::Texture texture;
        texture.loadFromMemory(memoryBuffer, fileSize);
        animation.push_back(texture);
        sprite.setTexture(animation.back());
        */
        free(fileBuffer);
        
    }
    return true;
}

class Object {
	private:
        bool visibility = true;
        sf::Sprite sprite;
        std::vector<sf::Texture> animation;
        size_t frame = 0;
        const char* type = ""; // хранит тип объекта (кнопки, объекта) для определения с чем мы взимодействуем
        bool isCyclic = true;
        
	public:
        Object() {}
        ~Object() {  }
        Object(const char* filename, const char* type = nullptr) throw() : type(type) {
            addPicture(filename); 
        }

        Object(sf::Image image, const char* type = nullptr) : type(type) {
            addPicture(image);
        }

        Object(sf::Image image, int x, int y, const char* type = nullptr) : type(type){
            addPicture(image);
            sprite.setPosition(float(x), float(y));
        }
        Object(const char* filename, int x, int y, const char* type = nullptr) throw() : type(type) {
            addPicture(filename);
            sprite.setPosition((float)x, (float)y);
        }
        Object(std::vector<sf::Image> images, int x, int y, const char* type = nullptr, bool cycle = true) {
            for (auto img : images) {
                addPicture(img);
            }
            sprite.setPosition((float)x, (float)y);
            frame = 0;
            isCyclic = cycle;
        }
        

        void addPicture(const char* filename) throw() {// download pic to sprite
            void* filebuffer = nullptr;
            long filesize = 0;
            if(!getDataFromImage(filename, filebuffer, filesize)) throw std::runtime_error("Can't open png file.\n");
            else {
                sf::Texture texture;
                texture.loadFromMemory(filebuffer, filesize);
                animation.push_back(texture);
                sprite.setTexture(animation.front());
            }
            free(filebuffer);
        }

        void addPicture(sf::Image image) {
            sf::Texture texture;
            texture.loadFromImage(image);
            animation.push_back(texture);
            sprite.setTexture(animation.front());
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
            if (frame >= animation.size() && isCyclic) frame = 0;
            else if (frame >= animation.size() && !isCyclic) return;
            sprite.setTexture(animation[frame]);
        }

        void setCyclicity(bool makeCyclic) {
            isCyclic = makeCyclic;
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

