#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <iomanip> 
#include <vector> 
using namespace std;

void drawArrow(sf::RenderWindow& window, const sf::Vector2f& start, const sf::Vector2f& end) {
    // Вычисляем угол между линией и осью X
    float angle = std::atan2(end.y - start.y, end.x - start.x);

    // Создаем треугольник для стрелки
    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3); // Треугольник имеет 3 вершины

    // Устанавливаем координаты треугольника
    arrowHead.setPoint(0, sf::Vector2f(0, -10)); // Острый конец стрелки
    arrowHead.setPoint(1, sf::Vector2f(-5, 5));  // Левый угол основания
    arrowHead.setPoint(2, sf::Vector2f(5, 5));   // Правый угол основания

    // Устанавливаем случайный цвет
    srand(static_cast<unsigned>(time(nullptr))); // Инициализируем генератор случайных чисел
    sf::Color randomColor(rand() % 256, rand() % 256, rand() % 256);
    arrowHead.setFillColor(randomColor);

    // Позиционируем треугольник
    arrowHead.setPosition(end);
    arrowHead.setOrigin(0, 0); // Центр треугольника в (0,0)

    // Вращаем стрелку под углом
    arrowHead.setRotation(angle * 180 / 3.14159); // Прямой угол уже на остром конце

    // Рисуем линию от начала до конца
    sf::Vertex line[] = {
        sf::Vertex(start, sf::Color::Black),
        sf::Vertex(end, sf::Color::Black)
    };
    window.draw(line, 2, sf::Lines);

    // Рисуем треугольник
    window.draw(arrowHead);
}

int main()
{
    class EdgeList {
    private:
        std::vector<std::vector<int>> table;


    public:
        EdgeList() {
            this->table = { {1, 2}, {1, 3}, {1, 5}, {2, 1}, {2, 3}, {3, 1}, {3, 2}, {3, 4}, {3, 5}, {4, 3}, {4, 5}, {5, 3}, {5, 4} };
        }

        void print() const {
            for (const auto& row : table) {
                std::cout << "{ ";
                for (const auto& value : row) {
                    std::cout << value << " ";
                }
                std::cout << "} ";
            }

            std::cout << std::endl;
        }
    };
    auto edgeList = new EdgeList();
    edgeList->print();
    std::cout << std::endl;

    class TableGraph {
    private:
        std::vector<std::vector<int>> table;


    public:
        TableGraph() {
            this->table = { {0, 1, 1, 0, 1}, {1, 0, 1, 0, 0}, {1, 1, 0, 1, 1}, {0, 0, 1, 0, 1}, {0, 0, 1, 1, 0} };
        }

        void print() const {
            std::cout << std::setw(3) << "1" << std::setw(2) << "2" << std::setw(2) << "3" << std::setw(2) << "4" << std::setw(2) << "5" << std::endl;
            int i = 0;
            for (const auto& row : table) {
                i++;
                std::cout << std::setw(1) << i << std::setw(2);
                for (const auto& value : row) {
                    std::cout << value << std::setw(2);
                }
                std::cout << std::endl;
            }
        }
    };
    auto tableGraph = new TableGraph();
    tableGraph->print();
    std::cout << std::endl;

    class ListGraph {
    private:
        std::vector<std::vector<int>> table;


    public:
        ListGraph() {
            this->table = { {2, 3, 5}, {1, 3}, {1, 2, 4, 5}, {3, 5}, {3, 4} };
        }

        void print() const {
            int i = 0;
            for (const auto& row : table) {
                i++;
                std::cout << std::setw(1) << i << std::setw(2);
                for (const auto& value : row) {
                    std::cout << value << std::setw(1);
                }
                std::cout << std::endl;
            }
        }
    };
    auto listGraph = new ListGraph();
    listGraph->print();
    std::cout << std::endl;



    class SfmlGraph {
    private:
        std::vector<std::vector<int>> table;


    public:
        SfmlGraph() {}

        void print() const {
            sf::RenderWindow window(sf::VideoMode(800, 600), "Graph with Arrows");

            // Позиции кругов (звеней графа)
            const int numVertices = 5;
            sf::Vector2f positions[numVertices] = {
                {200, 300},
                {400, 150},
                {600, 300},
                {400, 450},
                {300, 250}
            };

            // Загружаем шрифт
            sf::Font font;
            if (!font.loadFromFile("tuffy.ttf")) {
                // Обработка ошибки, если шрифт не загрузился
                return;
            }



            // Основной цикл приложения
            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    if (event.type == sf::Event::Closed)
                        window.close();
                }

                window.clear(sf::Color::White);

                // Рисуем круги
                for (int i = 0; i < numVertices; ++i) {
                    sf::CircleShape circle(30); // Радиус круга
                    circle.setFillColor(sf::Color::Blue);
                    circle.setOutlineThickness(5); // Толщина контура
                    circle.setOutlineColor(sf::Color::Black); // Цвет контура
                    circle.setPosition(positions[i].x - circle.getRadius(), positions[i].y - circle.getRadius());

                    window.draw(circle);

                    // Текст для номера круга
                    sf::Text text;
                    // text.setFont(sf::Font());
                    text.setFont(font);
                    text.setString(std::to_string(i + 1)); // Номер круга
                    text.setCharacterSize(24); // Размер шрифта
                    text.setFillColor(sf::Color::White); // Цвет текста
                    text.setPosition(
                        positions[i].x - text.getLocalBounds().width / 2,
                        positions[i].y - text.getLocalBounds().height / 2
                    );

                    window.draw(text); // Рисуем текст на круге
                }

                // Рисуем стрелки между кругами
                for (int i = 0; i < numVertices; ++i) {
                    int nextIndex = (i + 1) % numVertices; // Соединяем круги по кругу
                    drawArrow(window, positions[i], positions[nextIndex]);
                }

                window.display(); // Обновляем окно
            }
        }
    };
    auto sfmlGrapgh = new SfmlGraph();
    sfmlGrapgh->print();
    std::cout << std::endl;
}