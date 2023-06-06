#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <numeric>
// 추가로 벡터의 크기를 구하는 함수를 추가하겠습니다.
float magnitude(const sf::Vector3f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

sf::Vector3f normalize(const sf::Vector3f& v) {
    float mag = magnitude(v);
    return sf::Vector3f(v.x / mag, v.y / mag, v.z / mag);
}
sf::Vector3f crossProduct(const sf::Vector3f& v1, const sf::Vector3f& v2) {
    return sf::Vector3f(v1.y * v2.z - v1.z * v2.y,
                        v1.z * v2.x - v1.x * v2.z,
                        v1.x * v2.y - v1.y * v2.x);
}

sf::Vector3f calculateNormal(const sf::Vector3f& v1, const sf::Vector3f& v2, const sf::Vector3f& v3) {
    return normalize(crossProduct(v2 - v1, v3 - v1));
}

sf::Color calculateColor(const sf::Vector3f& normal, const sf::Color& baseColor) {
    sf::Vector3f lightDirection = sf::Vector3f(0.0f, 0.0f, -1.0f);
    float lightIntensity = std::max(0.0f, normal.z);  // Assuming light direction is (0, 0, -1)

    // Increase lightIntensity by a factor to brighten the scene
    lightIntensity *= 1.0f;

    return sf::Color(std::min((int)(baseColor.r * lightIntensity), 255),
                     std::min((int)(baseColor.g * lightIntensity), 255),
                     std::min((int)(baseColor.b * lightIntensity), 255));
}

sf::Vector3f rotate(const sf::Vector3f& point, float angleX, float angleY, float angleZ) {
    float radX = angleX * M_PI / 180.0f;
    float radY = angleY * M_PI / 180.0f;
    float radZ = angleZ * M_PI / 180.0f;
    float sinX = std::sin(radX), cosX = std::cos(radX);
    float sinY = std::sin(radY), cosY = std::cos(radY);
    float sinZ = std::sin(radZ), cosZ = std::cos(radZ);

    return {
        point.x * (cosY * cosZ) + point.y * (-cosX * sinZ + sinX * sinY * cosZ) + point.z * (sinX * sinZ + cosX * sinY * cosZ),
        point.x * (cosY * sinZ) + point.y * (cosX * cosZ + sinX * sinY * sinZ) + point.z * (-sinX * cosZ + cosX * sinY * sinZ),
        point.x * (-sinY) + point.y * (sinX * cosY) + point.z * (cosX * cosY)
    };
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
            float x, y, z; iss >> x >> y >> z;
            vertices.push_back(sf::Vector3f(x, y, z));
        } else if (type == "f") { // face
            std::vector<int> face; std::string vertex;
            while (iss >> vertex) {
                std::istringstream viss(vertex);
                int index; char separator;
                viss >> index >> separator;
                face.push_back(index - 1); // OBJ indices start from 1
            }
            faces.push_back(face);
        }
    }
    file.close();
    return true;
}

// A struct to store information about a face
struct FaceInfo {
    std::vector<int> face;
    sf::Vector3f normal;
    float depth;
};


int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "3D Object");

    std::vector<sf::Vector3f> vertices;
    std::vector<std::vector<int>> faces;
    loadObjFile("model.obj", vertices, faces);

    for (auto& vertex : vertices) {
        vertex.z += 110;
        vertex.y *= -1;
        vertex.y += 1;
    }

    sf::Vector3f center(0, 0, 0);  // 객체의 중심점 정의
    center = std::accumulate(vertices.begin(), vertices.end(), center);
    center /= (float)vertices.size();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float angleX = sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ? 30.0f : 0.0f;
        float angleY = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? 30.0f : 0.0f;
        float angleZ = sf::Keyboard::isKeyPressed(sf::Keyboard::E) ? 30.0f : 0.0f;

        for (auto& vertex : vertices) {
            vertex = rotate(vertex - center, angleX, angleY, angleZ) + center;
        }

        window.clear();

        // Calculate normals and depth for each face
        std::vector<FaceInfo> faceInfos;
        for (const auto& face : faces) {
            sf::Vector3f normal = calculateNormal(vertices[face[0]], vertices[face[1]], vertices[face[2]]);
            float depth = (vertices[face[0]].z + vertices[face[1]].z + vertices[face[2]].z) / 3.0f;
            faceInfos.push_back({face, normal, depth});
        }

        // Sort faces by depth
        std::sort(faceInfos.begin(), faceInfos.end(), [](const FaceInfo& a, const FaceInfo& b) {
            return a.depth > b.depth; // from far to near
        });

        for (const auto& faceInfo : faceInfos) {
            sf::Color faceColor = calculateColor(faceInfo.normal, sf::Color::White);

            // Backface culling
            if (faceInfo.normal.z < 0) continue;

            sf::ConvexShape polygon;
            polygon.setPointCount(faceInfo.face.size());
            for (int i = 0; i < faceInfo.face.size(); i++) {
                polygon.setPoint(i, project(vertices[faceInfo.face[i]]) * 400.f + sf::Vector2f(400, 300));
            }
            polygon.setFillColor(faceColor);

            window.draw(polygon);
        }


        window.display();
    }

    return 0;
}

