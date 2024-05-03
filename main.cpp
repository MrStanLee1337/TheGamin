



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
    Universe universe(window, WIDTH, HEIGHT);
    universe.init();
    universe.playMusic();
    universe.playSound(TREE);

    
    //перечесления для выбора режима - пауза(стартовое окно)\сама игра\выход


    while (window.isOpen()) {
        
        universe.pendingAction();
        universe.state();
        
        window.clear(sf::Color(rand()%256, rand()%256, rand()%256));
        universe.draw();
        window.display();
    }

	return 0;
}

