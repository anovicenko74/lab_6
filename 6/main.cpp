#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <limits>
#include <utility> // Для std::pair

using namespace std;

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

    void drawGraph(sf::RenderWindow& window) {
        sf::CircleShape vertex(20);
        vertex.setFillColor(sf::Color::Green);
        vertex.setOutlineColor(sf::Color::Black);
        vertex.setOutlineThickness(2);

        for (const auto& pair : vertexPositions) {
            // Рисуем вершину
            vertex.setPosition(pair.second.x - vertex.getRadius(), pair.second.y - vertex.getRadius());
            window.draw(vertex);

            // Рисуем текст для вершины
            sf::Font font;
            font.loadFromFile("arial.ttf");
            sf::Text text(pair.first, font, 20);
            text.setPosition(pair.second.x - 10, pair.second.y - 10);
            window.draw(text);

            // Рисуем рёбра
            for (const auto& neighbor : adjacencyList[pair.first]) {
                sf::Vertex line[] = {
                    sf::Vertex(pair.second),
                    sf::Vertex(vertexPositions[neighbor.first])
                };
                window.draw(line, 2, sf::Lines);
            }
        }
    }

private:
    unordered_map<string, vector<pair<string, int>>> adjacencyList;
    unordered_map<string, sf::Vector2f> vertexPositions;
};

int main() {
    vector<Edge> edges = {
        {"A", "B", 14},
        {"A", "C", 10},
        {"B", "C", 2},
        {"C", "D", 5},
        {"B", "D", 6}
    };

    Graph graph(edges);
    string startVertex = "A";
    string endVertex = "D";

    auto distances = graph.dijkstra(startVertex);
    cout << "Минимальная длина пути из " << startVertex << " в " << endVertex << ": " << distances[endVertex] << endl;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Directed Graph Visualization");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);
        graph.drawGraph(window);
        window.display();
    }

    return 0;
}