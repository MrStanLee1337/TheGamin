#pragma once
//#include <iostream>
//#include <vector>
#include <SFML/Graphics.hpp>
#include <queue>
#include <map>
#include <algorithm>
class MyMap {
private:
	std::vector<sf::Sprite> sprites;// спрайты которые мы будем отрисовывать, в том числе и повторяющиеся

	enum TileType {//названия типа текстур
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
	std::unordered_map<TileType, sf::Texture> textures;//единичные экземпляры текстур для спрайта
	std::vector<std::pair<int, int>> mp;

	
	/*
	int dx[4] = { 1, 0, -1, 0 };
	int dy[4] = { 0, 1, 0, -1 };
	int diagx[4] = { 1, 1, -1, -1 };
	int diagy[4] = { 1, -1, 1, -1 };
	
	
	std::pair<int,int> func(int k, int x, int y) {
		std::cout << x << ' ' << y << '\n';
		if (k == 0) {// look up
			if (intmap[y + 1][x - 1].first == 1) {// down left is full
				intmap[y][x - 1].first = 2;//left = edge
				used[y][x - 1] = true;
				return { y, x - 1 };
			}
			else if (intmap[y + 1][x - 1].first == 2) {//down left is edge
				intmap[y][x - 1].first = 3;//left = corner
				intmap[y][x - 1].second = 0;
				used[y][x - 1] = true;
				return { -1, 0 };// if -1 then !q.push, second value is angle
			}
			if (intmap[y + 1][x + 1].first == 1) {//down right is full
				intmap[y][x + 1].first = 2;
				used[y][x + 1] = true;
				return { y, x + 1 };
			}
			else if (intmap[y + 1][x + 1].first == 2) {//down right is edge
				intmap[y][x + 1].first = 3;
				used[y][x + 1] = true;
				return { -1, 0 };
			}
			
		}
	}
	*/

	bool psb(int p) {
		return rand() % 100 < p;
	}
	/*
	int setPoint(int x, int y) {
		int neighbor[4];
		neighbor[0] = intmap[y][x - 1].first;
		neighbor[1] = intmap[y][x + 1].first;
		neighbor[2] = intmap[y - 1][x].first;
		neighbor[3] = intmap[y + 1][x].first;
		std::sort(neighbor, neighbor + 4);
		
		if (neighbor[0] > 0) return 1;//вокруг все занято возвращаем full
		if (neighbor[1] > 0) return 1;
		if (neighbor[2] == 1 && neighbor[3] == 1) return 1;
		if (neighbor[2] == 1 && neighbor[3] == 2) return psb(50) ? 2 : 4;
		if (neighbor[2] == 2 && neighbor[3] == 2) return 3;
		if (neighbor[3] == 1) return psb(90) ? 1 : 2;
		return 0;
	}*/

	/*
	void randMap() {
		intmap.resize(maxcounty);
		for (int i = 0; i < maxcounty; i++) intmap[i].resize(maxcountx, std::make_pair(0, 0));
		used.resize(maxcounty);
		for (int i = 0; i < maxcounty; i++) used[i].resize(maxcountx, false);

		int startx = rand() % maxcountx;
		int starty = rand() % maxcounty;

		//bfs
		std::queue<std::pair<int, int>> q;
		q.push(std::make_pair(startx, starty));
		intmap[starty][startx].first = 1;
		used[starty][startx] = true;

		while (!q.empty()) {
			auto now = q.front();
			q.pop();
			for (int i = 0; i < 4; i++) {
				int mx = now.first + dx[i];
				int my = now.second + dy[i];
				if (mx > 0 && mx < maxcountx - 1 && my > 0 && my < maxcounty - 1 && used[my][mx] == false) {
					used[my][mx] = true;
					if (auto k = setPoint(mx, my)) {
						intmap[my][mx].first = k;
						q.push(std::make_pair(mx,my));
					}

				}
			}
		}
		/*
		while (!q.empty()) {
			auto now = q.front();
			
			q.pop();
			for (int i = 0; i < 4; i++) {
				int mx = now.first + dx[i];
				int my = now.second + dy[i];
				if(mx >= 0 && mx < maxcountx && my >=0 && my < maxcounty && used[my][mx] == false) {
					used[my][mx] = true;
					if (intmap[now.second][now.first].first == 1 ) {
						int k = rand()%10;
						if (k > 2) {
							intmap[my][mx].first = 1;// FULL
							q.push(std::make_pair(mx, my));
						} else {
							intmap[my][mx].first = 2;//EDGE
							if (dx[i] == 0) intmap[my][mx].second = dy[i] > 0 ? 2 : 0;
							if (dy[i] == 0) intmap[my][mx].second = dx[i] > 0 ? 1 : 3;
							q.push(std::make_pair(mx,my));
							
						}
					}
					else if (intmap[now.second][now.first].first == 2) {
						int k = intmap[now.second][now.first].second; // смотрим куда повернут спрайт
						auto d = func(k, mx, my);
						if (d.first != -1 && d.first < maxcountx && d.second < maxcounty) q.push(d);
					}
				}
			}
			
		}
		
		//neighbor count

	}
	*/
	
	void rotate(sf::Sprite& s, int angle) {
		s.setOrigin(s.getLocalBounds().width / 2.f, s.getLocalBounds().height / 2.f);
		s.move(s.getLocalBounds().width / 2.f, s.getLocalBounds().height / 2.f);
		s.setRotation(angle * 90);
	}
	
	void defMap() {
		/*
		for (int j = 0; j < maxcounty; j++) {
			for (int i = 0; i < maxcountx; i++) {
				sf::Sprite sprite;
				sprite.setPosition(8*16 + 16*i, 8*16 + 16*j);
				switch (0) {

				case 0:
					continue;

				case 1:
					sprite.setTexture(textures[FULL]);
					break;

				case 2:
					sprite.setTexture(textures[EDGE]);
					
					break;

				case 3:
					sprite.setTexture(textures[CORNER]);
					break;

				case 4:
					sprite.setTexture(textures[FULLONE]);
					break;

				}
				
				sprites.push_back(sprite);
			}
		}
		*/
		int destx = 100;
		int desty = 100;
		int predl = -1, predr = -1;
		for (int v = 1; v < 2*mp.size() - 1; v++) {
			auto d = mp[v/2];
			for (int i = d.first + 1; i < d.second - 1; i++) {// заполняем внутренние спрайты FULL
				sf::Sprite sprite;
				
				sprite.setPosition(destx + 16 * i, desty + 16 * v);
				sprite.setTexture(textures[FULL]);
				sprites.push_back(sprite);
			}
		}
		/*
		predl = 1e10;
		predr = 0;
		for (int v = 0; v < 2*mp.size(); v++) {
			auto d = mp[v / 2];

			sf::Sprite sprite;
			if (d.first < predl) {
				sprite.setPosition(destx + 16 * d.first, desty + 16 * v);
				if (v % 2 == 0) sprite.setTexture(textures[CORNER]);
			}
			else {
				sprite.setPosition(destx + 16 * d.first, desty + 16 * v);
				if (v % 2 == 1) {
					sprite.setTexture(textures[FULLONE]);
					rotate(sprite, 1);
				}
			}
			
			//sprite.setPosition(destx + 16 * i, desty + 16 * v);
			//sprite.setTexture(textures[EDGE]);
			//if (v == 2 * mp.size() - 1) rotate(sprite, 2);
			//else if (i == d.second - 1) rotate(sprite, 1);
			//else if (i == d.first) rotate(sprite, 3);
			predl = d.first;
			predr = d.second;
			sprites.push_back(sprite);
		}
		*/
	}
	void genMap() {
		mp.resize(maxcounty / 2);// идём по вертикали 
		int maxdiff = 4;
		int predl = maxcountx / 4 * 1;
		int predr = maxcountx / 4 * 3;
		for (int v = 0; v < mp.size(); v++) {
			
			int l, r;
			if (v < mp.size() / 2) {
				l = std::max((predl - rand() % maxdiff), 0);
				r = std::min((predr + rand() % maxdiff), (int)maxcountx - 1);
			}
			else {
				l = std::min((predl + rand() % maxdiff), (int)maxcountx - 1);
				r = std::max((predr - rand() % maxdiff), 0);
			}

			mp[v] = std::make_pair(l, r);
			
			predl = l;
			predr = r;
		}
	}

	

	void DRY(sf::Image& im, TileType type, sf::IntRect rect, int sizex = 16, int sizey = 16) {// чтобы не повторяться в инициализации тайлов
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

	void clear() {
		mp.clear();
		sprites.erase(sprites.begin() + 1, sprites.end());
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

		void generateMap(int maxx = 48, int maxy = 30) {
			maxcountx = maxx;
			maxcounty = maxy;
			clear();
			genMap();
			defMap();
		}
		
		void draw(sf::RenderWindow& window) {
			for (auto& obj : sprites) window.draw(obj);
		}
		~MyMap() {}
};