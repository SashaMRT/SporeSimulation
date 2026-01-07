#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(1280u, 720u)), "Spore Simulation");
    window.setFramerateLimit(60);

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) 
                window.close();
        }

        window.clear(sf::Color::Black);
        
        sf::RectangleShape Simulation(sf::Vector2f(900.f, 720.f));
        Simulation.setFillColor(sf::Color(10, 30, 50));
        Simulation.setPosition(sf::Vector2f(0.f, 0.f));
        window.draw(Simulation);
        
        sf::RectangleShape menu(sf::Vector2f(380.f, 720.f));
        menu.setFillColor(sf::Color(40, 40, 40, 180));
        menu.setPosition(sf::Vector2f(900.f, 0.f));
        window.draw(menu);
        
        window.display();
    }
    return 0;
}
