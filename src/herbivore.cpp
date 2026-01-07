#include "herbivore.hpp"
#include "monde.hpp"

Herbivore::Herbivore(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::HERBIVORE, 15.f, 100.f) {
    vitesse = {100.f, 50.f};
}

void Herbivore::update(float dt, Monde& monde) {
    position += vitesse * dt;
    sf::FloatRect limites = monde.getLimites();
    if (position.x < limites.position.x || position.x > limites.size.x) vitesse.x *= -1;
    if (position.y < limites.position.y || position.y > limites.size.y) vitesse.y *= -1;
}

void Herbivore::dessiner(sf::RenderTarget& cible) const {
    sf::CircleShape forme(rayon);
    forme.setFillColor(sf::Color::Green);
    forme.setOrigin({rayon, rayon});
    forme.setPosition(position);
    cible.draw(forme);
}