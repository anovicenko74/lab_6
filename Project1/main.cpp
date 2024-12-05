#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <utility> // Для std::pair

using namespace std;
#define M_PI 3.14

struct Edge {
    string from;
    string to;
    int weight;
};

class Graph {
public:
    explicit Graph(const vector<Edge>& edges) {
        for (const auto& edge : edges) {
            adjacencyList[edge.from].push_back(make_pair(edge.to, edge.weight));

            // Установка случайной позиции для вершин
            if (vertexPositions.find(edge.from) == vertexPositions.end()) {
                vertexPositions[edge.from] = sf::Vector2f(rand() % 800, rand() % 600);
            }
            if (vertexPositions.find(edge.to) == vertexPositions.end()) {
                vertexPositions[edge.to] = sf::Vector2f(rand() % 800, rand() % 600);
            }
        }
    }

    unordered_map<string, int> dijkstra(const string& start) {
        unordered_map<string, int> distances;
        for (const auto& pair : adjacencyList) {
            distances[pair.first] = numeric_limits<int>::max();
        }
        distances[start] = 0;

        auto cmp = [](const pair<string, int>& left, const pair<string, int>& right) {
            return left.second > right.second;
            };
        priority_queue<pair<string, int>, vector<pair<string, int>>, decltype(cmp)> pq(cmp);
        pq.push(make_pair(start, 0));

        while (!pq.empty()) {
            auto current = pq.top().first;
            int currentDistance = pq.top().second;
            pq.pop();

            if (currentDistance > distances[current]) continue;

            for (const auto& neighbor : adjacencyList[current]) {
                int newDist = currentDistance + neighbor.second;
                if (newDist < distances[neighbor.first]) {
                    distances[neighbor.first] = newDist;
                    pq.push(make_pair(neighbor.first, newDist));
                }
            }
        }
        return distances;
    }

    void drawGraph(sf::RenderWindow& window, float centerX, float centerY, float radius, bool mode) {
        float angleStep = 2 * M_PI / adjacencyList.size();

        int i = 0;
        for (const auto& entry : adjacencyList) {
            float angle = i * angleStep;
            float x = centerX + radius * cos(angle);
            float y = centerY + radius * sin(angle);
            vector<pair<string, int>> ppp = entry.second;
            // std::cout << entry.first << ": "  << ppp[0].first << " | " << ppp[0].second << std::endl;
            // std::cout << pair2.first << " x " << pair2.second[0] << "y " << pair2.second[1];

            vertexPositions[entry.first] = sf::Vector2(x, y);
            i++;
        }

        sf::CircleShape vertex(50);
        vertex.setFillColor(sf::Color::Red);
        vertex.setOutlineColor(sf::Color::White);
        vertex.setOutlineThickness(2);

        sf::Font font;
        font.loadFromFile("fonts/tuffy.ttf");
        sf::Texture edgeTexture;
        edgeTexture.loadFromFile("images/asfalt.jpg");
        for (const auto& pair : vertexPositions) {

            sf::Text text(pair.first, font, 20);
            
            // Рисуем вершину
            if (mode) {
                if (pair.first == "Svetlogorsk") {
                    vertex.setFillColor(sf::Color::Green);
                    text.setColor(sf::Color::Blue);
                }
                if (pair.first == "Baltiysk") {
                    vertex.setFillColor(sf::Color::Black);
                }
                if (pair.first == "Yantarnyy" || pair.first == "Primorsk") {
                    vertex.setFillColor(sf::Color::Blue);
                }
            }
            

            vertex.setPosition(pair.second.x - vertex.getRadius(), pair.second.y - vertex.getRadius());
            text.setPosition(pair.second.x - 40, pair.second.y - 25);
            window.draw(vertex);
            window.draw(text);
           
            vertex.setFillColor(sf::Color::Red);
            text.setColor(sf::Color::White);

            
            for (const auto& neighbor : adjacencyList[pair.first]) {
                // Находим позиции начальной и конечной точек
                sf::Vector2f start = pair.second;
                sf::Vector2f end = vertexPositions[neighbor.first];

                // Вычисляем длину ребра и угол
                float length = std::sqrt(std::pow(end.x - start.x, 2) + std::pow(end.y - start.y, 2));
                float angle = std::atan2(end.y - start.y, end.x - start.x) * 180 / 3.14159; // Угол в градусах

                // Создаем прямоугольник для ребра
                sf::RectangleShape edge(sf::Vector2f(length, 5)); // 5 - это толщина ребра
                edge.setTexture(&edgeTexture);
                edge.setOrigin(0, 2.5f); // Устанавливаем точку поворота в центр по вертикали
                edge.setPosition(start); // Устанавливаем позицию в начальную точку
                edge.setRotation(angle); // Поворачиваем прямоугольник на угол

                // Отображаем текст на ребре
                sf::Text p(to_string(neighbor.second), font, 20);
                int x = static_cast<int>((start.x + end.x) / 2 + sin(start.x ));
                int y = static_cast<int>((start.y + end.y) / 2);
                const float textOffset = 15.0f;
                float offsetX = textOffset * abs(std::cos(angle * 3.14159 / 180)); 
                float offsetY = textOffset * abs(std::sin(angle * 3.14159 / 180));
                p.setPosition(x + offsetX, y + offsetY);
                p.setFillColor(sf::Color::White);

                window.draw(edge);
                window.draw(p);
            }
        }
    }

    vector<vector<int>> adjacencyListToMatrix() {
        int numVertices = adjacencyList.size();

        // Создаем матрицу смежности размером numVertices на numVertices и инициализируем ее значением INF
        vector<vector<int>> adjacencyMatrix(numVertices, vector<int>(numVertices, -1));

        // Словарь для сопоставления строковых идентификаторов с индексами
        unordered_map<string, int> vertexIndex;
        int index = 0;

        // Заполняем словарь для сопоставления
        for (const auto& entry : adjacencyList) {
            vertexIndex[entry.first] = index++;
        }
        // Заполняем матрицу смежности
        for (const auto& entry : adjacencyList) {
            int i = vertexIndex[entry.first]; // Получаем индекс текущей вершины

            for (const auto& edge : entry.second) {
                string neighbor = edge.first; // Получаем строковый идентификатор соседа
                int weight = edge.second;  // Получаем вес ребра

                // Устанавливаем вес в матрице смежности
                adjacencyMatrix[i][vertexIndex[neighbor]] = weight;
            }
        }


        return adjacencyMatrix;
    }

private:
    unordered_map<string, vector<pair<string, int>>> adjacencyList;
    unordered_map<string, sf::Vector2f> vertexPositions;
};

int main() {
    std::setlocale(LC_ALL, "ru");
   
    vector<Edge> edges = {
       {"Baltiysk", "Primorsk", 14}, {"Baltiysk", "Kaliningrad", 45}, {"Primorsk", "Yantarnyy", 20}, {"Primorsk", "Kolosovka", 24}, {"Primorsk", "Kaliningrad", 40},
       {"Yantarnyy", "Svetlogorsk", 24}, {"Svetlogorsk", "Kolosovka", 27}, {"Svetlogorsk", "Pionersky", 4}, {"Pionersky", "Kaliningrad", 44}, {"Pionersky", "Zelenogradsk", 24},
       {"Zelenogradsk", "Kaliningrad", 33}, {"Zelenogradsk", "Guryevsk", 25}, {"Guryevsk", "Gvardeysk", 37}, {"Gvardeysk", "Kaliningrad", 53}, {"Primorsk", "Kaliningrad", 40},
        {"Kaliningrad", "Guryevsk", 13},

       {"Primorsk", "Baltiysk", 14}, {"Kaliningrad", "Baltiysk", 45}, {"Yantarnyy", "Primorsk", 20}, {"Kolosovka", "Primorsk", 24}, {"Kaliningrad", "Primorsk", 40},
       {"Svetlogorsk", "Yantarnyy", 24}, {"Kolosovka", "Svetlogorsk", 27}, {"Pionersky", "Svetlogorsk", 4}, {"Kaliningrad", "Pionersky", 44}, {"Zelenogradsk", "Pionersky", 24},
       {"Kaliningrad", "Zelenogradsk", 33}, {"Guryevsk", "Zelenogradsk", 25}, {"Gvardeysk", "Guryevsk", 37}, {"Kaliningrad", "Gvardeysk", 53}, {"Kaliningrad", "Primorsk", 40},
        {"Guryevsk", "Kaliningrad", 13},
    };

    Graph graph(edges);
    string startVertex = "Svetlogorsk";
    string endVertex = "Baltiysk";

    auto distances = graph.dijkstra(startVertex);
    cout << "Минимальная длина пути из " << startVertex << " в " << endVertex << ": " << distances[endVertex] << endl;

    auto matrix = graph.adjacencyListToMatrix();
    for (const auto& row : matrix) {
        for (auto val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    sf::RenderWindow window(sf::VideoMode(1500, 800), "Directed Graph Visualization");
    bool mode = false;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                mode = true;
            }
        }

        
        window.clear(sf::Color::Black);
        graph.drawGraph(window, 400, 400, 300, mode);
        window.display();
    }

    return 0;
}