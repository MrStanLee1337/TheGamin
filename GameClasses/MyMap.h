#pragma once
//#include <iostream>
//#include <vector>
//#include <SFML/Graphics.hpp>
#include <map>
class MyMap {
private:
	std::vector<sf::Sprite> sprites;// спрайты которые мы будем отрисовывать, в том числе и повтор€ющиес€

	enum TileType {//названи€ типа текстур
		BACKGROUND,
		FULL,
		EDGE,
		CORNER,
		FULLONE,
		FULLTWO,
		FULLTHREE,
		FULLFOUR,
		FULLDIAG
	};

	size_t maxcountx, maxcounty;
	
	std::vector<std::vector<int>> intmap;

	
	std::unordered_map<TileType, sf::Texture> textures;//единичные экземпл€ры текстур дл€ спрайта


	void initMap() {
		intmap.resize(maxcounty);
		for (int i = 0; i < maxcounty; i++) intmap[i].resize(maxcountx, 0);
		for (int i = 0; i < maxcounty; i++) for (int j = 0; j < maxcountx; j++) intmap[i][j] = rand() % 2 + 1;
		for (int i = 0; i < maxcounty; i++) for (int j = 0; j < maxcountx; j++) std::cout << intmap[i][j];
	}

	void defMap() {
		for (int j = 0; j < maxcounty; j++) {
			for (int i = 0; i < maxcountx; i++) {
				sf::Sprite sprite;
				sprite.setPosition(8*16 + 16*i, 8*16 + 16*j);
				switch (intmap[j][i]) {
				case 1:
					sprite.setTexture(textures[FULL]);
					break;
				case 2:
					sprite.setTexture(textures[EDGE]);
					break;
				}
				sprites.push_back(sprite);
			}
		}
	}

	void generateMap(int maxx = 48, int maxy = 32) {
		maxcountx = maxx;
		maxcounty = maxy;
		initMap();
		defMap();
	}	

	void DRY(sf::Image& im, TileType type, sf::IntRect rect, int sizex = 16, int sizey = 16) {// чтобы не повтор€тьс€ в инициализации тайлов
		sf::Image part;
		part.create(sizex,sizey);
		sf::Texture texture;
		part.copy(im, 0, 0, rect);
		texture.loadFromImage(part);
		textures[type] = texture;
	}
	

	void initTextures() throw() {
		void* filebuffer = nullptr;
		long filesize = 0;
		if (!getDataFromImage("tiles\\backwater.png", filebuffer, filesize)) throw std::runtime_error("Can't open backwater.png file.");
		sf::Image backwater;
		backwater.loadFromMemory(filebuffer, filesize);
		DRY(backwater, BACKGROUND, sf::IntRect(0,0,1028,768), 1028, 768);
		sprites.push_back(sf::Sprite(textures[BACKGROUND]));

		filebuffer = nullptr;
		filesize = 0;
		if (!getDataFromImage("tiles\\simpletiles.png", filebuffer, filesize)) throw std::runtime_error("Can't open simpletiles.png file.");
		sf::Image image;
		
		image.loadFromMemory(filebuffer, filesize);

		DRY(image, FULL, sf::IntRect(0,0,16,16));
		DRY(image, EDGE, sf::IntRect(16, 0, 32, 16));
		DRY(image, CORNER, sf::IntRect(32, 0, 48, 16));
		DRY(image, FULLONE, sf::IntRect(0, 16, 16, 32));
		DRY(image, FULLTWO, sf::IntRect(16, 16, 32, 32));
		DRY(image, FULLTHREE, sf::IntRect(32, 16, 48, 32));
		DRY(image, FULLFOUR, sf::IntRect(48, 16, 64, 32));
		DRY(image, FULLDIAG, sf::IntRect(64, 16, 80, 32));

	}


	public:
		MyMap() {}

		void initTiles() {
			try {
				initTextures();
				generateMap();
			} catch (const std::exception& ex) {
				std::cout << ex.what() << '\n';
			}
		}

		void draw(sf::RenderWindow& window) {
			for (auto& obj : sprites) window.draw(obj);
		}
		~MyMap() {}
};