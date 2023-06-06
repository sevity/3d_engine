#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

sf::Vector3f rotateX(const sf::Vector3f& point, float angle) {
    float radian = angle * M_PI / 180.0f;
    float sinAngle = std::sin(radian);
    float cosAngle = std::cos(radian);
    return sf::Vector3f(
        point.x,
        point.y * cosAngle - point.z * sinAngle,
        point.y * sinAngle + point.z * cosAngle
    );
}

sf::Vector3f rotateY(const sf::Vector3f& point, float angle) {
    float radian = angle * M_PI / 180.0f;
    float sinAngle = std::sin(radian);
    float cosAngle = std::cos(radian);
    return sf::Vector3f(
        point.x * cosAngle - point.z * sinAngle,
        point.y,
        point.x * sinAngle + point.z * cosAngle
    );
}

sf::Vector2f project(const sf::Vector3f& point) {
    return sf::Vector2f(point.x / point.z, point.y / point.z);
}


bool loadObjFile(const std::string& filename, std::vector<sf::Vector3f>& vertices, std::vector<std::vector<int>>& faces) {
    std::ifstream file(filename);

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") { // vertex
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(sf::Vector3f(x, y, z));
        } else if (type == "f") { // face
            std::vector<int> face;
            std::string vertex;
            while (iss >> vertex) {
                std::istringstream viss(vertex);
                int index;
                char separator;
                viss >> index >> separator;
                face.push_back(index - 1); // OBJ indices start from 1
            }
            faces.push_back(face);
        }
    }

    file.close();
    return true;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "3D Object");

    std::vector<sf::Vector3f> vertices;
    std::vector<std::vector<int>> faces;
    loadObjFile("model.obj", vertices, faces);

    for (auto& vertex : vertices) {
        vertex.z += 10;
        vertex.y *= -1;
    }

    sf::Vector3f center(0, 0, 0);  // Object's center definition
    for (auto& vertex : vertices) {
        center = center + vertex;
    }
    center /= (float)vertices.size();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for (auto& vertex : vertices) {
            vertex = vertex - center;  // Subtracting the center point to start rotation
            vertex = rotateX(vertex, 3.00f);
            vertex = rotateY(vertex, 5.00f);
            vertex = vertex + center;  // Adding the center point back
        }

        for (const auto& face : faces) {
            sf::Vector2f p1 = project(vertices[face[0]]);
            sf::Vector2f p2 = project(vertices[face[1]]);
            sf::Vector2f p3 = project(vertices[face[2]]);
            sf::Vertex line[] = {
                sf::Vertex(p1 * 400.f + sf::Vector2f(400, 300)),
                sf::Vertex(p2 * 400.f + sf::Vector2f(400, 300)),
                sf::Vertex(p3 * 400.f + sf::Vector2f(400, 300)),
                sf::Vertex(p1 * 400.f + sf::Vector2f(400, 300))
            };
            window.draw(line, 4, sf::Lines);
        }

        window.display();
    }

    return 0;
}
