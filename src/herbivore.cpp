#include "herbivore.hpp"
#include "monde.hpp"
#include <cmath>

Herbivore::Herbivore(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::HERBIVORE, 15.f, 100.f) {
    vitesse = {100.f, 50.f};
}

void Herbivore::update(float dt, Monde& monde) {
    Entite* cible = monde.getPlusProche(position, TypeEntite::ALGUE);

    if (cible) {
        sf::Vector2f direction = cible->getPosition() - position;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance > 0) {
            vitesse = (direction / distance) * 100.f;
        }

        if (distance < rayon + cible->getRayon()) {
            energie += 20.f;
            cible->tuer();
        }
    }

    position += vitesse * dt;
    sf::FloatRect limites = monde.getLimites();
    if (position.x < limites.position.x || position.x > limites.size.x) vitesse.x *= -1;
    if (position.y < limites.position.y || position.y > limites.size.y) vitesse.y *= -1;

    energie -= 5.f * dt;
    if (energie <= 0) tuer();
}

void Herbivore::dessiner(sf::RenderTarget& cible) const {
    sf::CircleShape forme(rayon);
    forme.setFillColor(sf::Color::Green);
    forme.setOrigin({rayon, rayon});
    forme.setPosition(position);
    cible.draw(forme);
}