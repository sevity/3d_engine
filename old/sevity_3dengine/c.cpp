#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

const double PI = 3.14159265358979323846;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// 3D Vector
struct Vector3 {
    double x, y, z;

    Vector3 operator*(const std::vector<std::vector<double>>& matrix) {
        return {
            x * matrix[0][0] + y * matrix[1][0] + z * matrix[2][0],
            x * matrix[0][1] + y * matrix[1][1] + z * matrix[2][1],
            x * matrix[0][2] + y * matrix[1][2] + z * matrix[2][2]
        };
    }
};

// 2D Point for plotting
struct Point2D {
    double x, y;
};

// Rotate around Y-Axis
Vector3 rotateY(Vector3 vec, double angle) {
    std::vector<std::vector<double>> rotationMatrix = {
        {cos(angle), 0, sin(angle)},
        {0, 1, 0},
        {-sin(angle), 0, cos(angle)}
    };

    return vec * rotationMatrix;
}

// Project 3D to 2D
Point2D project(Vector3 vec) {
    return { vec.x / vec.z, vec.y / vec.z };
}
sf::Vector2f normalizeToScreen(float x, float y, float screenWidth, float screenHeight) {
    // 좌표계를 -1~1에서 0~1로 바꿔줌
    float normalizedX = (x + 1.0f) / 2.0f;
    float normalizedY = (y + 1.0f) / 2.0f;

    // 0~1에서 실제 스크린의 픽셀 수치로 변환
    float screenX = normalizedX * screenWidth;
    float screenY = normalizedY * screenHeight;

    return sf::Vector2f(screenX, screenY);
}
int main() {
    // Create main window
    sf::RenderWindow App(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32), "3D Cube Projection");

    std::vector<Vector3> cubePoints = {
        {-1, -1, 2},
        {1, -1, 2},
        {1, 1, 2},
        {-1, 1, 2},
        {-1, -1, 3},
        {1, -1, 3},
        {1, 1, 3},
        {-1, 1, 3}
    };

    double theta = PI / 8;  // 45 degrees

    std::vector<Point2D> projectedPoints;
    for (auto& point : cubePoints) {
        auto rotatedPoint = rotateY(point, theta);
        projectedPoints.push_back(project(rotatedPoint));
    }

    // Main loop
    while (App.isOpen()) {
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event)) {
            // Close window: exit
            if (Event.type == sf::Event::Closed)
                App.close();
        }

        // Clear screen
        App.clear();

        for (auto& point : projectedPoints) {
            sf::CircleShape shape(2);
            shape.setFillColor(sf::Color::Green);
            sf::Vector2f screenPos = normalizeToScreen(point.x, point.y, WINDOW_WIDTH, WINDOW_HEIGHT);
            shape.setPosition(screenPos);
            App.draw(shape);
        }

        // Display window
        App.display();
    }

    return EXIT_SUCCESS;
}

