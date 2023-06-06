#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
using namespace std;

typedef sf::Vector3f point_3d;
typedef sf::Vector2f point_2d;


float magnitude(const point_3d& v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}
point_3d normalize(const point_3d& v) {
    float mag = magnitude(v);
    return point_3d(v.x / mag, v.y / mag, v.z / mag);
}
point_3d crossProduct(const point_3d& v1, const point_3d& v2) {
    return point_3d(v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x);
}
point_3d calculateNormal(point_3d& v1, point_3d& v2, point_3d& v3) {
    return normalize(crossProduct(v2 - v1, v3 - v1));
}
sf::Color calculateColor(point_3d& normal, const sf::Color& baseColor) {
    point_3d lightDirection = point_3d(0, 0, -1);
    float lightIntensity = max(0.0f, normal.z);
    lightIntensity *= 1.0f;
    return sf::Color(
        min((int)(baseColor.r * lightIntensity), 255),
        min((int)(baseColor.g * lightIntensity), 255),
        min((int)(baseColor.b * lightIntensity), 255));
}

// 3차원 점을 2차원으로 투영
point_2d project(point_3d p)
{
    return { p.x / p.z, p.y / p.z };
}

// 뷰포트 변환
point_2d viewport(float x, float y, float screenWidth, float screenHeight) {
    // 좌표계를 (-1, 1)에서 (0, 1)로 변환
    float normalizedX = (x + 1.0f) / 2.0f;
    float normalizedY = (y + 1.0f) / 2.0f;

    // (0, 1)에서 실제 스크린 사이즈 만큼 확대
    float screenX = normalizedX * screenWidth;
    float screenY = normalizedY * screenHeight;

    return point_2d(screenX, screenY);
}

point_3d rotate_y(point_3d p, float angle) {
    float rad = angle * 3.1415926 / 180.0f;
    float sin_ = sin(rad);
    float cos_ = cos(rad);
    point_3d rotated = point_3d(
        p.x * cos_ - p.z * sin_,
        p.y,
        p.x * sin_ + p.z * cos_
    );
    return rotated;
}
point_3d rotate_x(point_3d p, float angle) {
    float rad = angle * 3.1415926 / 180.0f;
    float sin_ = sin(rad);
    float cos_ = cos(rad);
    point_3d rotated = point_3d(
        p.x,
        p.y * cos_ - p.z * sin_,
        p.y * sin_ + p.z * cos_
    );
    return rotated;
}
point_3d rotate_z(point_3d p, float angle) {
    float rad = angle * 3.1415926 / 180.0f;
    float sin_ = sin(rad);
    float cos_ = cos(rad);
    point_3d rotated = point_3d(
        p.x * cos_ - p.y * sin_,
        p.x * sin_ + p.y * cos_,
        p.z
    );
    return rotated;
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
        }
        else if (type == "f") { // face
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

struct FaceInfo {
    vector<int> face;
    point_3d normal;
    float depth;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(600, 600), "My 3D Engine");

    vector<point_3d> vertices;
    vector<vector<int>> faces;
    loadObjFile("model.obj", vertices, faces);

    for (auto& vertex : vertices) {
        vertex.z += 7;  // 로딩한 물체를 z방향으로 7만큼 이동시켜서 눈(카메라)와 너무 가깝지 않도록 만듦
        vertex.y *= -1; // 주전자가 뒤집어져서 나와서.. 
        vertex.y += 1.5;
    }

    point_3d center(0, 0, 0);
    for (auto p : vertices) center += p;
    center /= (float)vertices.size();

    vector<point_2d> projected(vertices.size());
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float angX = sf::Keyboard::isKeyPressed(sf::Keyboard::Q) ? 3.0f : 0.0f;
        float angY = sf::Keyboard::isKeyPressed(sf::Keyboard::W) ? 3.0f : 0.0f;
        float angZ = sf::Keyboard::isKeyPressed(sf::Keyboard::E) ? 3.0f : 0.0f;

        for (int i = 0; i < vertices.size(); i++) {
            vertices[i] = rotate_x(vertices[i] - center, angX) + center;
            vertices[i] = rotate_y(vertices[i] - center, angY) + center;
            vertices[i] = rotate_z(vertices[i] - center, angZ) + center;
            projected[i] = project(vertices[i]);
        }


        window.clear();

        vector<FaceInfo> faceInfos;
        for (auto& face : faces) {
            point_3d normal = calculateNormal(vertices[face[0]], vertices[face[1]], vertices[face[2]]);
            float depth = (vertices[face[0]].z + vertices[face[1]].z + vertices[face[2]].z) / 3.0f;
            faceInfos.push_back({ face, normal, depth });
        }

        sort(faceInfos.begin(), faceInfos.end(), [](FaceInfo& a, FaceInfo& b) {
            return a.depth > b.depth; });

        for (auto& faceInfo : faceInfos) {
            sf::Color faceColor = calculateColor(faceInfo.normal, sf::Color::White);

            if (faceInfo.normal.z < 0) continue;

            sf::ConvexShape polygon;
            polygon.setPointCount(faceInfo.face.size());
            for (int i = 0; i < faceInfo.face.size(); i++) {
                point_2d projected = project(vertices[faceInfo.face[i]]);
                polygon.setPoint(i, viewport(projected.x, projected.y, 600, 600));
            }
            polygon.setFillColor(faceColor);
            window.draw(polygon);
        }

        window.display();
    }

    return 0;
}

