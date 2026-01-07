
#include "carnivore.hpp"
#include "monde.hpp"
#include <cmath>

Carnivore::Carnivore(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::CARNIVORE, 20.f, 120.f) {
    vitesse = {120.f, 80.f};
}

void Carnivore::update(float dt, Monde& monde) {
    Entite* cible = monde.getPlusProche(position, TypeEntite::HERBIVORE);

    if (cible) {
        sf::Vector2f direction = cible->getPosition() - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance > 0) {
            vitesse = (direction / distance) * 130.f;
        }

        if (distance < rayon + cible->getRayon()) {
            energie += 50.f;
            cible->tuer();
        }
    }

    position += vitesse * dt;
    
    sf::FloatRect limites = monde.getLimites();
    if (position.x < limites.position.x || position.x > limites.size.x) vitesse.x *= -1;
    if (position.y < limites.position.y || position.y > limites.size.y) vitesse.y *= -1;

    energie -= 8.f * dt;
    if (energie <= 0) tuer();
}

void Carnivore::dessiner(sf::RenderTarget& cible) const {
    sf::CircleShape forme(rayon, 3);
    forme.setFillColor(sf::Color::Red);
    forme.setOrigin({rayon, rayon});
    forme.setPosition(position);
    cible.draw(forme);
}