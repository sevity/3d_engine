#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "My 3D Engine");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.display();
    }

    return 0;
}
