#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

class Point3D {
public:
    float x, y, z;

    Point3D() : x(0), y(0), z(0) {}
    Point3D(float x, float y, float z) : x(x), y(y), z(z) {}

    Point3D operator+(const Point3D& right) const {
        return Point3D(x + right.x, y + right.y, z + right.z);
    }

    Point3D operator-(const Point3D& right) const {
        return Point3D(x - right.x, y - right.y, z - right.z);
    }

    Point3D rotateY(float angle) const {
        float radian = angle * M_PI / 180.0f;
        float sinAngle = std::sin(radian);
        float cosAngle = std::cos(radian);
        sf::Vector3f rotated = sf::Vector3f(
            x * cosAngle - z * sinAngle,
            y,
            x * sinAngle + z * cosAngle
        );
        return Point3D(rotated.x, rotated.y, rotated.z);
    }
    Point3D rotateX(float angle) const {
        float radian = angle * M_PI / 180.0f;
        float sinAngle = std::sin(radian);
        float cosAngle = std::cos(radian);
        sf::Vector3f rotated = sf::Vector3f(
            x,
            y * cosAngle - z * sinAngle,
            y * sinAngle + z * cosAngle
        );
        return Point3D(rotated.x, rotated.y, rotated.z);
    }
};

sf::Vector2f project(const Point3D& point, float fov) {
    float fovRad = 1.0 / tan(fov * 0.5 / 180.0 * M_PI);
    float qx = point.x * fovRad / point.z;
    float qy = point.y * fovRad / point.z;
    return sf::Vector2f(qx, qy);
}

bool loadObjFile(const std::string& filename, std::vector<Point3D>& vertices, std::vector<std::vector<int>>& faces) {
    std::ifstream file(filename);
    if (!file) {
        std::cout << "Failed to open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v") { // vertex
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(Point3D(x, y, z));
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

    std::vector<Point3D> vertices;
    std::vector<std::vector<int>> faces;
    if (!loadObjFile("model.obj", vertices, faces)) {
        return 1;
    }

    for (auto& vertex : vertices) {
        vertex.z += 10;
        vertex.y *= -1;
    }

    Point3D center(0, 0, 0);  // 객체의 중심점 정의
    for (auto& vertex : vertices) {
        center = center + vertex;
    }
    center.x /= vertices.size();
    center.y /= vertices.size();
    center.z /= vertices.size();

    // FOV value set to 90 for effect equivalent to z=1
    float fov = 45.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        for (auto& vertex : vertices) {
            vertex = vertex - center;  // 중심점을 빼서 회전을 시작
            vertex = vertex.rotateX(3.00f);
            vertex = vertex.rotateY(5.00f);
            vertex = vertex + center;  // 중심점을 다시 더함
        }

        for (const auto& face : faces) {
            sf::Vector2f p1 = project(vertices[face[0]], fov);
            sf::Vector2f p2 = project(vertices[face[1]], fov);
            sf::Vector2f p3 = project(vertices[face[2]], fov);
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
