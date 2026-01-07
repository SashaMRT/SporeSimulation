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
    float angleMouv = std::atan2(vitesse.y, vitesse.x);

    for (int i = 0; i < 5; ++i) {
        float angleBase = angleMouv + 3.14159f + (i - 2.5f) * 0.3f;
        for (int k = 0; k < 6; ++k) {
            float angle = angleBase + std::sin((position.x + position.y) * 0.05f + k * 0.5f + i) * 0.2f;
            float dist = rayon * (0.8f + k * 0.4f);
            float taille = rayon * 0.25f * (1.f - k / 6.f);

            sf::CircleShape tentacules(taille);
            tentacules.setOrigin({taille, taille});
            tentacules.setPosition(position + sf::Vector2f(std::cos(angle), std::sin(angle)) * dist);
            tentacules.setFillColor(sf::Color(100, 255, 100, 150 - k * 20));
            cible.draw(tentacules);
        }
    }

    sf::CircleShape corps(rayon);
    corps.setOrigin({rayon, rayon}); 
    corps.setPosition(position);
    corps.setFillColor(sf::Color(50, 220, 100, 200));
    corps.setOutlineThickness(2.f); 
    corps.setOutlineColor(sf::Color(150, 255, 150));
    cible.draw(corps);

    sf::CircleShape reflet(rayon * 0.3f);
    reflet.setOrigin({rayon * 0.3f, rayon * 0.3f});
    reflet.setPosition(position + sf::Vector2f(-rayon * 0.2f, -rayon * 0.2f));
    reflet.setFillColor(sf::Color(255, 255, 255, 100));
    cible.draw(reflet);
}