
#include <Universe.h>
#include <ctime>

const size_t HEIGHT = 768;
const size_t WIDTH = 1028;



int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    system("chcp 1251");
    setlocale(LC_ALL, "ru");
    srand(static_cast<unsigned>(time(NULL)));
    std::cout << "Started\n";
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), L"Sprout Lands :)");
    window.setFramerateLimit(60);

    //�������������� ���� ���������
    Universe universe(window, WIDTH, HEIGHT);
    universe.init();
    universe.playMusic();
    //universe.playSound(TREE);


    while (window.isOpen()) {
        universe.pendingAction();//���� �������
        universe.state();//���������� ������ ����(�����, ����)
        universe.tickrate();//���� ��� ��������
        universe.animation();//��������
        window.clear();
        universe.draw();//������ ��� ��� visible
        window.display();
    }

	return 0;
}

