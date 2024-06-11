#pragma once
//#include <iostream>
//#include <vector>
#include <SFML/Graphics.hpp>
#include <queue>
#include <map>
#include <algorithm>
#include <string>
/*
switch mark
1 - периметр
2 - пустое простанство внутри острова
3 - декоративные элементы
4 - элемент взаимодействия
5 - ограждение

7 - место появления
*/

class MyMap {
private:
	std::vector<sf::Sprite> sprites;// спрайты которые мы будем отрисовывать, в том числе и повторяющиеся
	std::vector<std::unique_ptr<Object>> objects;//объекты взаимодействия и анимации
	//sprites хранит декоративные объекты с которыми нельзя взаимодействовать (плитки,цветочки, камешки)
	//objects хранит объекты с которыми можно взаимодейстовать (дерево для сбора плодов, лодка)
	std::pair<int, int> startPosTiles;
	std::pair<int, int> startPosPxls;//позиция где будет появляться ГГ
	enum TileType {//названия типа текстур
		BACKGROUND,
		FULL,
		EDGE,
		CORNER,
		FULLONE,
		//BUSH,
		BUSHNAKED,
		MUSHREDTWO,
		MUSHREDONE,
		MUSHVIOLONE,
		MUSHVIOLTWO,
		LEAFONE,
		LEAFTWO
	};


	size_t tileW = 16, tileH = 16;
	size_t maxcountx, maxcounty;// количество тайлов в ширину и высоту

	size_t leftStart, upStart;//верхняя левая вершина построения карты

	int oXstart, oYstart;// локальный центр острова
	std::unordered_map<TileType, sf::Texture> textures;//единичные экземпляры текстур для спрайта
	//std::vector<std::pair<int, int>> mp;
	std::vector<std::vector<int>> theMap;

	


	

	bool psb(int p) {
		return rand() % 100 < p;
	}

	

	

	void initStartPoint() {
		oXstart = (rand() % (maxcountx / 3)) + maxcountx / 3;// начало оси оХ находится во 2 трети 
		oYstart = (rand() % (maxcounty / 3)) + maxcounty / 3;//тоже самое для oY]
	}
	void genPerimeter() {// генерируем  периметр помощью 4-х кривых на каждой из четвертей координатной плоскости 
		
		int mark = 1;
		int y, x;
		x = oXstart;
		y = 0;
		theMap[y][x++] = mark;//избавляемся от некрасивых мысов из одного тайла
		while (true) {// 1 четверть
			theMap[y][x] = mark;
			if (y == oYstart && x == maxcountx - 1) break;
			if (y == oYstart) ++x;
			else if (x == maxcountx - 1) ++y;
			else {
				int f = rand() % 2;
				if (f) ++x;
				else ++y;
			}
		}
		++y;
		theMap[y++][x] = mark;

		while (true) {//идем по часовой. 4 четверть
			theMap[y][x] = mark;
			if (y == maxcounty - 1 && x == oXstart) break;
			if (y == maxcounty - 1) --x;
			else if (x == oXstart) ++y;
			else {
				int f = rand() % 2;
				if (f) --x;
				else ++y;
			}
		}

		--x;
		theMap[y][x--] = mark;

		while (true) {// 3 четверть
			theMap[y][x] = mark;
			if (y == oYstart && x == 0) break;
			if (y == oYstart) --x;
			else if (x == 0) --y;
			else {
				int f = rand() % 2;
				if (f) --x;
				else --y;
			}
		}

		--y;
		theMap[y--][x] = mark;

		while (true) {//2 четверть
			theMap[y][x] = mark;
			if (y == 0 && x == oXstart - 1) break;
			if (y == 0) ++x;
			else if (x == oXstart - 1) --y;
			else {
				int f = rand() % 2;
				if (f) ++x;
				else --y;
			}
		}
	}

	void genArea() {// BFS realization for filling the Area of island
		int dx[4] = { 1, 0, -1, 0 };
		int dy[4] = { 0, 1, 0, -1 };
		std::queue<std::pair<int, int>> q;
		q.push(std::make_pair(oXstart, oYstart));
		int mark = 2; // filling mark
		while (!q.empty()) {
			auto now = q.front();
			q.pop();
			for (int i = 0; i < std::size(dx); i++) {
				int newx = now.first + dx[i];
				int newy = now.second + dy[i];
				if (theMap[newy][newx] == 0) {
					theMap[newy][newx] = mark;
					q.push(std::make_pair(newx, newy));
				}
			}
			
		}
	}

	void genMap() {
		theMap.resize(maxcounty);
		for (int i = 0; i < maxcounty; i++) theMap[i].resize(maxcountx, 0);
		initStartPoint();
		genPerimeter();
		genArea();
		
		
		

	}

	void rotateTile(sf::Sprite& s, int k) {// rotate sprite k times
		k %= 4;
		if (k == 1) {
			s.move(tileW, 0);
			s.setRotation(90);
		}
		else if (k == 2) {
			s.move(tileW, tileH);
			s.setRotation(180);
		}
		else if (k == 3) {
			s.move(0, tileH);
			s.setRotation(270);
		}
	}

	void typeTheTile(sf::Sprite& sprite, int x, int y) {// определить тип плитки
		int neighbor = 0;// количество соседей у тайла
		int dx[8] = { 1, 0, -1, 0,  1, 1, -1, -1 };
		int dy[8] = { 0, 1, 0, -1 ,-1, 1,  1, -1 };
		for (int i = 0; i < std::size(dx); i++) {
			if (y + dy[i] < maxcounty && x + dx[i] < maxcountx && x + dx[i] >= 0 && y + dy[i] >= 0)
				if (theMap[y + dy[i]][x + dx[i]] != 0) ++neighbor;
		}
		TileType TT;
		if (neighbor == 7) {
			TT = FULLONE;
			if (x + 1 < maxcountx && y - 1 >= 0 && theMap[y - 1][x + 1] == 0) rotateTile(sprite, 2);
			else if (x + 1 == maxcountx || y + 1 == maxcounty || theMap[y + 1][x + 1] == 0) rotateTile(sprite, 3);
			else if (x - 1 >= 0 && y - 1 >= 0 && theMap[y - 1][x - 1] == 0) rotateTile(sprite, 1);
		}
		else if (neighbor == 6) {
			TT = EDGE;
			if (x + 1 == maxcountx || theMap[y][x + 1] == 0) rotateTile(sprite, 1);
			else if (x - 1 == -1 || theMap[y][x - 1] == 0) rotateTile(sprite, 3);
			else if (y + 1 == maxcounty || theMap[y + 1][x] == 0) rotateTile(sprite, 2);
		}
		else if (neighbor == 5) {
			int dx[4] = { 1, -1,  0, 0 };
			int dy[4] = { 0,  0, -1, 1 };
			int n = 0;
			for (int i = 0; i < std::size(dx); i++) {
				if (y + dy[i] < maxcounty && x + dx[i] < maxcountx && x + dx[i] >= 0 && y + dy[i] >= 0)
					if (theMap[y + dy[i]][x + dx[i]] != 0) ++n;
			}
			if (n == 3) {
				TT = EDGE;
				if (x + 1 == maxcountx || theMap[y][x + 1] == 0) rotateTile(sprite, 1);
				else if (x - 1 == -1 || theMap[y][x - 1] == 0) rotateTile(sprite, 3);
				else if (y + 1 == maxcounty || theMap[y + 1][x] == 0) rotateTile(sprite, 2);
			} else {
				TT = CORNER;
				if (x + 1 < maxcountx && y - 1 != -1 && theMap[y - 1][x + 1] == 2) rotateTile(sprite, 2);
				else if (x + 1 < maxcountx && y + 1 < maxcounty && theMap[y + 1][x + 1] == 2) rotateTile(sprite, 3);
				else if (x - 1 != -1 && y - 1 != -1 && theMap[y - 1][x - 1] == 2) rotateTile(sprite, 1);
			}
		}
		else if (neighbor == 4 || neighbor == 3) {
			TT = CORNER;
			if (x + 1 < maxcountx && y - 1 != -1 && theMap[y - 1][x + 1] == 2) rotateTile(sprite, 2);
			else if (x + 1 < maxcountx && y + 1 < maxcounty && theMap[y + 1][x + 1] == 2) rotateTile(sprite, 3);
			else if (x - 1 != -1 && y - 1 != -1 && theMap[y - 1][x - 1] == 2) rotateTile(sprite, 1);
		}
		else return;
		sprite.setTexture(textures[TT]);
	}

	void defMap() {
		for (int j = 0; j < maxcounty; j++) {
			for (int i = 0; i < maxcountx; i++) {
				sf::Sprite sprite;
				sprite.setPosition(leftStart + 16 * i, upStart + 16 * j);
				if (theMap[j][i] == 2) sprite.setTexture(textures[FULL]);
				else if (theMap[j][i] == 1) {
					typeTheTile(sprite, i ,j);
					//sprite.setTexture(textures[EDGE]);
				}
				sprites.push_back(sprite);
			}
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
		//DRY(image, FULLTWO, sf::IntRect(16, 16, 32, 32));
		//DRY(image, FULLTHREE, sf::IntRect(32, 16, 48, 32));
		//DRY(image, FULLFOUR, sf::IntRect(48, 16, 64, 32));
		//DRY(image, FULLDIAG, sf::IntRect(64, 16, 80, 32));


		filebuffer = nullptr;
		filesize = 0;
		if (!getDataFromImage("tiles\\decoration.png", filebuffer, filesize)) throw std::runtime_error("Can't open decoration.png file.");
		image.loadFromMemory(filebuffer, filesize);
		free(filebuffer);
		//DRY(image, BUSH, sf::IntRect(0, 32, 16, 48));
		DRY(image, BUSHNAKED, sf::IntRect(16, 32, 32, 48));
		DRY(image, MUSHREDTWO, sf::IntRect(32,32, 48, 48));
		DRY(image, MUSHREDONE, sf::IntRect(48, 32, 64, 48));
		DRY(image, MUSHVIOLONE, sf::IntRect(64, 32, 80, 48));
		DRY(image, MUSHVIOLTWO, sf::IntRect(80, 32, 96, 48));
		DRY(image, LEAFONE, sf::IntRect(96, 32, 112, 48));
		DRY(image, LEAFTWO, sf::IntRect(112, 32, 128, 48));
		
	}

	void clear() {// очищаем карту и спрайты(кроме background)
		objects.clear();
		theMap.clear();
		sprites.erase(sprites.begin() + 1, sprites.end());
	}

	

	void addSprite(TileType tt, int x, int y) {//заполняем вектор спрайтами
		sf::Sprite sprite;
		sprite.setPosition(x, y);
		sprite.setTexture(textures[tt]);
		sprites.push_back(sprite);
	}

	//заполняем случайным образом
	void addSprites(TileType tt, int mark, int closeR = 0, int k = 1) {//тип, маркировка, радиус, кол-во
		int times = 20;// на случай ошибки
		if (!closeR) {
			for (int c = 0; c < k; c++) {
				int x = 0, y = 0;
				while (theMap[y][x] != 2 && --times) x = rand() % maxcountx, y = rand() % maxcounty;
				if (!times) return;
				theMap[y][x] = mark;
				addSprite(tt, leftStart + tileW * x, upStart + tileH * y);
			}
		}
		else {
			int x = 0, y = 0;
			while (theMap[y][x] != 2) x = rand() % maxcountx, y = rand() % maxcounty;
			theMap[y][x] = mark;
			addSprite(tt, leftStart + tileW * x, upStart + tileH * y);
			int newx = x - closeR, newy = y - closeR;
			newx = newx < 0 ? 0 : newx > maxcountx - 1 ? maxcountx - 1 : newx;//смотрим ограничения
			newy = newy < 0 ? 0 : newy > maxcounty - 1 ? maxcounty - 1 : newy;
			for (int c = 0; c < k - 1; c++) {
				int yy = 0, xx = 0;
				while (theMap[yy][xx] != 2 && --times) {
					xx = newx + rand() % (2 * closeR), yy = newy + rand() % (2 * closeR);
					xx = std::min(xx, (int)maxcountx - 1), yy = std::min(yy, (int)maxcounty - 1);
					xx = std::max(xx, 0), yy = std::max(yy, 0);
				}
				if (!times) return;
				theMap[yy][xx] = mark;
				addSprite(tt, leftStart + tileW * xx, upStart + tileH * yy);
			}
		}
	}

	void addDecor() {
		int mark = 3;
		//addSprites(BUSH, mark, 5, 3);
		addSprites(MUSHREDTWO, mark, 5, 3);
		addSprites(MUSHREDONE, mark, 4, 2);
		addSprites(MUSHVIOLONE, mark, 4, 4);
		addSprites(MUSHVIOLTWO, mark, 4, 4);
		addSprites(LEAFONE, mark, 0, 7);
		addSprites(LEAFTWO, mark, 0, 6);
	}

	void addBoat() {// добавляем лодку у берега в случайное место по оси оХ
		int j = maxcounty - 2;
		int x, y;
		int left = 0; int right = maxcountx - 1;
		while (theMap[j][left++] != 1);
		while (theMap[j][right--] != 1);
		int i = rand() % (right - left + 1) + left;
		int mark = 7;
		theMap[j][i] = mark;//значение лодки
		x = leftStart + tileW * i;
		y = upStart + tileH * j;

		startPosTiles = std::make_pair(i, j);
		startPosPxls = std::make_pair(x, y);//пусть стартовая позиция будет находиться у лодки

		objects.push_back(std::make_unique<Object>("tiles\\boat.png", x, y, "boat"));
	}

	void addTrees() {
		int mark = 4;

		void* filebuffer = nullptr;
		long filesize = 0;
		sf::Image image;
		sf::Image tree;//дерево с плодом
		sf::Image nakedTree;//голое дерево
		sf::Image stump;//пень
		tree.create(32, 32);
		nakedTree.create(32, 32);
		stump.create(32, 32);

		if (!getDataFromImage("tiles\\decoration.png", filebuffer, filesize)) throw std::runtime_error("Can't open decoration.png file.");
		image.loadFromMemory(filebuffer, filesize);

		tree.copy(image, 0, 0, sf::IntRect(0, 0, 32, 32));
		nakedTree.copy(image, 0, 0, sf::IntRect(32, 0, 64, 32));
		stump.copy(image, 0, 0, sf::IntRect(96, 0, 128, 32));

		int countTrees = 2;
		std::vector<sf::Image> trees = {tree, nakedTree, stump};
		while (countTrees--) {
			int x = 0, y = 0;
			while (theMap[y][x] != 2 && theMap[y + 1][x] != 2 && theMap[y][x + 1] != 2 && theMap[y + 1][x + 1] != 2) x = rand() % (maxcountx - 1), y = rand() % (maxcounty - 1);
			theMap[y][x] = mark, theMap[y + 1][x] = mark, theMap[y][x + 1] = mark, theMap[y + 1][x + 1] = mark;

			//Object obj(tree, leftStart + tileW * x, upStart + tileH * y, "boat");
			objects.push_back(std::make_unique<Object>(trees, leftStart + tileW * x, upStart + tileH * y, "boat", false));
			//auto& last = objects.back();
			//tree.copy(image, 0, 0, sf::IntRect(32,0,64,32));//голое дерево (следующий фрейм)
			//(*last).addPicture(nakedTree);
		}
		sf::Image bush;
		bush.create(16, 16);
		bush.copy(image, 0, 0, sf::IntRect(0, 32, 16, 48));
		int countBushes = 3;
		while (countBushes--) {
			int x = 0; int y = 0;
			while (theMap[y][x] != 2) x = rand() % maxcountx, y = rand() % maxcounty;
			theMap[y][x] = mark;
			objects.push_back(std::make_unique<Object>(bush, leftStart + tileW * x, upStart + tileH * y, "bush"));
		}
	}

	void addObjects() {
		addBoat();
		addTrees();
	}
	
	void addDecoration() {
		addObjects();//заполняем vector objects
		addDecor();//заполняем vector sprites
	}

	
	public:
		MyMap() {}
		~MyMap() {}

		void initTiles() {
			try {
				initTextures();
				//generateMap();
			} catch (const std::exception& ex) {
				std::cout << ex.what() << '\n';
			}
		}

		void generateMap(int maxx = 48, int maxy = 30, int leftPos = 100, int upPos = 100) {//кол-во плиток и нач позиция
			maxcountx = maxx;
			maxcounty = maxy;
			leftStart = leftPos;
			upStart = upPos;
			clear();
			genMap();
			defMap();
			addDecoration();

			printMap();

		}
		
		void isInteracted(int x, int y) {
			for (auto& obj : objects) {
				if (obj->isClicked(x, y)) {
					obj->nextFrame();
					//std::cout << "NextFramed\n";
				}
			}
		}

		std::vector<std::vector<int>> getMap() {
			return theMap;
		}

		std::pair<size_t, size_t> getStartIslandPointPxls() {
			return { leftStart, upStart };
		}

		std::pair<int, int> getStartCharacterTilePoint() { return startPosTiles; }
		std::pair<int, int> getStartCharacterPxlsPoint() { return startPosPxls; }
		
		void draw(sf::RenderWindow& window) {
			for (auto& obj : sprites) window.draw(obj);
			for (auto& obj : objects) {
				if (auto ptr = dynamic_cast<Object*>(&*obj)) {
					ptr->draw(window);
				}
			}
		}
		void printMap() {
			for (auto x : theMap) {
				for (auto y : x) {
					std::cout << y;
				}
				std::cout << '\n';
			}
		}

};