
#include "carnivore.hpp"
#include "monde.hpp"

Carnivore::Carnivore(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::CARNIVORE, 20.f, 120.f) {
    vitesse = {120.f, 80.f};
}

void Carnivore::update(float dt, Monde& monde) {
    position += vitesse * dt;
    sf::FloatRect limites = monde.getLimites();
    if (position.x < limites.position.x || position.x > limites.size.x) vitesse.x *= -1;
    if (position.y < limites.position.y || position.y > limites.size.y) vitesse.y *= -1;
}

void Carnivore::dessiner(sf::RenderTarget& cible) const {
    sf::CircleShape forme(rayon, 3);
    forme.setFillColor(sf::Color::Red);
    forme.setOrigin({rayon, rayon});
    forme.setPosition(position);
    cible.draw(forme);
}