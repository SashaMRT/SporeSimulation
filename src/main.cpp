#include <SFML/Graphics.hpp>
#include "monde.hpp"
#include "rendu.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1280u, 720u)), "Spore Simulation");
    window.setFramerateLimit(60);

    Monde monde(sf::FloatRect({0.f, 0.f}, {900.f, 720.f}));
    Rendu rendu;
    sf::Clock clock;

    for (int i = 0; i < 10; ++i) {
        monde.spawnAlgue({static_cast<float>(rand() % 800 + 50), static_cast<float>(rand() % 600 + 50)});
    }

    for (int i = 0; i < 5; ++i) {
        monde.spawnBacterie({static_cast<float>(rand() % 800 + 50), static_cast<float>(rand() % 600 + 50)});
    }

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) 
                window.close();
        }

        monde.update(dt);

        window.clear(sf::Color::Black);
        
        sf::RectangleShape ZoneSimulation(sf::Vector2f(900.f, 720.f));
        ZoneSimulation.setFillColor(sf::Color(10, 30, 50));
        window.draw(ZoneSimulation);
        
        monde.dessiner(window);
        rendu.menu(monde, window, 0.f, 0);
        
        window.display();
    }
    return 0;
}