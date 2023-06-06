#include <SFML/Graphics.hpp>
#include <cmath>

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
};

sf::Vector2f project(const Point3D& point) {
    return sf::Vector2f(point.x / point.z, point.y / point.z);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "3D Cube");

    Point3D points[8] = {
        Point3D(-1, -1, 6), 
        Point3D(1, -1, 6), 
        Point3D(1, 1, 6),  
        Point3D(-1, 1, 6), 
        Point3D(-1, -1, 8), 
        Point3D(1, -1, 8), 
        Point3D(1, 1, 8), 
        Point3D(-1, 1, 8)   
    };

    Point3D center(0, 0, 7);  // 큐브의 중심점 정의

    int edges[12][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 0}, // lower face
        {4, 5}, {5, 6}, {6, 7}, {7, 4}, // upper face
        {0, 4}, {1, 5}, {2, 6}, {3, 7} // side faces
    };


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        Point3D rotatedPoints[8];
        for (int i = 0; i < 8; ++i) {
            Point3D point = points[i] - center;  // 중심점을 빼서 회전을 시작
            Point3D rotatedPoint = point.rotateY(0.10f);
            rotatedPoints[i] = rotatedPoint + center;  // 중심점을 다시 더함
        }
        for (auto &edge : edges) {
            sf::Vector2f p1 = project(rotatedPoints[edge[0]]);
            sf::Vector2f p2 = project(rotatedPoints[edge[1]]);
            sf::Vertex line[] = {
                sf::Vertex(p1 * 400.f + sf::Vector2f(400, 300)),
                sf::Vertex(p2 * 400.f + sf::Vector2f(400, 300))
            };
            window.draw(line, 2, sf::Lines);
        }
        for (int i = 0; i < 8; ++i) {
            points[i] = rotatedPoints[i];
        }

        window.display();
    }

    return 0;
}

