

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <Windows.h>
#include <GameMusic.h>


const size_t HEIGHT = 1028;
const size_t WIDTH = 768;



int main() {
    std::cout << "Started\n";
    setlocale(LC_ALL, "ru");
    sf::RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), L"ULTRAGAMIN");
    
    //инициализируем музыку\звуки
    gm::GameMusic sounds;
    try {
        sounds.SetMusic(BACK_MUSIC, MUSICFILE);
        sounds.AddSound(TREE, MUSICFILE);
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << '\n';
    }
    sounds.playMusic(true);
    //sounds.playSound(TREE);
    
    //перечесления для выбора режима - пауза(стартовое окно)\сама игра\режим редактирования\выход
    enum Play {
        PAUSE,
        GAME,
        EDIT,
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
                //TheMap.pause();
                break;
                //TheMap.pause();

            case GAME:
                break;

            case EDIT:
                break;

            case EXIT:
                window.close();

        }
        
        window.clear(sf::Color::Cyan);
        window.display();
    }

	return 0;
}

