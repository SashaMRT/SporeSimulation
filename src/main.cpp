#include <SFML/Graphics.hpp>
#include <iostream>
#include "monde.hpp"
#include "rendu.hpp"

void initialiserMonde(Monde& monde) {
    monde.reset();
    for (int i = 0; i < 50; ++i) 
        monde.spawnAlgue({static_cast<float>(rand() % 850 + 25), static_cast<float>(rand() % 680 + 20)});
    for (int i = 0; i < 5; ++i) 
        monde.spawnBacterie({static_cast<float>(rand() % 850 + 25), static_cast<float>(rand() % 680 + 20)});
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1280u, 720u}), "Spore Simulation");
    window.setFramerateLimit(60);

    Monde monde(sf::FloatRect({0.f, 0.f}, {900.f, 720.f}));
    
    Rendu rendu;
    if (!rendu.init("arial.ttf")) {
        std::cerr << "Erreur: Police introuvable" << std::endl;
    }

    initialiserMonde(monde);

    sf::Clock clock;
    bool enPause = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) 
                window.close();
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                    window.close();
                
                if (keyPressed->code == sf::Keyboard::Key::P)
                    enPause = !enPause;
                
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    initialiserMonde(monde);
                    enPause = false; 
                }
            }
        }

        if (!enPause) {
            monde.update(dt);
        }

        window.clear(sf::Color::Black);
        
        sf::RectangleShape zoneJeu({900.f, 720.f});
        zoneJeu.setFillColor(sf::Color(10, 30, 50));
        window.draw(zoneJeu);
        
        monde.dessiner(window);
        
        rendu.menu(monde, window, enPause);
        
        window.display();
    }
    return 0;
}