#include <SFML/Graphics.hpp>
#include "monde.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Spore Simulation");
    window.setFramerateLimit(60);
    
    Monde monde;
    
    monde.spawnAlgue({400, 360});
    monde.spawnAlgue({200, 200});
    monde.spawnAlgue({600, 500});
    
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        
        monde.update(0.016f);
        
        window.clear(sf::Color::Black);
        monde.dessiner(window);
        window.display();
    }
    
    return 0;
}
