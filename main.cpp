



//#include <GameMusic.h>
#include <Universe.h>
#include <ctime>

const size_t HEIGHT = 768;
const size_t WIDTH = 1028;



int main() {
    system("chcp 1251");
    srand(static_cast<unsigned>(time(NULL)));
    std::cout << "Started\n";
    setlocale(LC_ALL, "ru");
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), L"ULTRAGAMIN");
    window.setFramerateLimit(60);

    //инициализируем нашу вселенную
    Universe universe;
    universe.init(window);
    universe.playMusic();
    universe.playSound(TREE);

    
    //перечесления для выбора режима - пауза(стартовое окно)\сама игра\выход
    enum Play {
        PAUSE,
        GAME,
        EXIT
    } now = PAUSE;


    


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        switch (now) {
            case PAUSE:
                //universe.pause();
                break;
                //TheMap.pause();

            case GAME:
                break;

            case EXIT:
                window.close();

        }
        
        window.clear(sf::Color(rand()%256, rand()%256, rand()%256));
        universe.draw(window);
        window.display();
    }

	return 0;
}

