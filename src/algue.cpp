#include "algue.hpp"
#include "monde.hpp"

Algue::Algue(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::ALGUE, 12.f, 200.f) {
}

void Algue::update(float dt, Monde& monde) {
}

void Algue::dessiner(sf::RenderTarget& cible) const {
    sf::CircleShape cercle(rayon);
    cercle.setFillColor(sf::Color::Green);
    cercle.setPosition(position - sf::Vector2f(rayon, rayon));
    cible.draw(cercle);
}
