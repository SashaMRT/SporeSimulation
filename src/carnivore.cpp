
#include "carnivore.hpp"
#include "monde.hpp"
#include <cmath>
#include <SFML/System/Angle.hpp>

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

        if (vitesse.x != 0 || vitesse.y != 0) {
            float angle = std::atan2(vitesse.y, vitesse.x) * 180.f / 3.14159f;
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
    float angleRad = std::atan2(vitesse.y, vitesse.x);
    float angleDeg = angleRad * 180.f / 3.14159f;
    sf::Vector2f dirArriere = {-std::cos(angleRad), -std::sin(angleRad)};

    for(int i = 3; i >= 1; --i) {
        float tailleQueue = rayon * (0.7f - i * 0.1f);
        sf::ConvexShape segment(3);
        segment.setPoint(0, {tailleQueue * 1.5f, 0});
        segment.setPoint(1, {-tailleQueue, -tailleQueue * 0.7f});
        segment.setPoint(2, {-tailleQueue, tailleQueue * 0.7f});
        segment.setPosition(position + dirArriere * (float)i * (rayon * 0.7f));
        segment.setRotation(sf::degrees(angleDeg + std::sin((position.x + position.y) * 0.1f + i) * 15.f));
        segment.setFillColor(sf::Color(180 - i*20, 30, 30));
        segment.setOutlineThickness(1.f); segment.setOutlineColor(sf::Color(100, 0, 0));
        cible.draw(segment);
    }

    sf::ConvexShape corps(4);
    corps.setPoint(0, {rayon * 1.8f, 0}); corps.setPoint(1, {0, -rayon});
    corps.setPoint(2, {-rayon * 0.8f, 0}); corps.setPoint(3, {0, rayon});
    corps.setPosition(position);
    corps.setRotation(sf::degrees(angleDeg));
    corps.setFillColor(sf::Color(220, 50, 50));
    corps.setOutlineThickness(2.f); corps.setOutlineColor(sf::Color(150, 20, 20));
    cible.draw(corps);

    sf::Vector2f off = {std::cos(angleRad), std::sin(angleRad)};
    sf::CircleShape oeil(rayon * 0.25f);
    oeil.setOrigin({rayon * 0.25f, rayon * 0.25f});
    oeil.setPosition(position + off * rayon * 0.5f);
    oeil.setFillColor(sf::Color::Yellow); oeil.setOutlineThickness(1.f); oeil.setOutlineColor(sf::Color::Black);
    cible.draw(oeil);

    sf::CircleShape pupille(rayon * 0.1f);
    pupille.setOrigin({rayon * 0.1f, rayon * 0.1f});
    pupille.setPosition(oeil.getPosition() + off * rayon * 0.1f);
    pupille.setFillColor(sf::Color::Black);
    cible.draw(pupille);

    sf::ConvexShape pointe(3);
    pointe.setPoint(0, {rayon * 0.4f, 0}); pointe.setPoint(1, {0, -rayon * 0.2f}); pointe.setPoint(2, {0, rayon * 0.2f});
    pointe.setFillColor(sf::Color(100, 0, 0));

    pointe.setPosition(position + sf::Vector2f(std::cos(angleRad - 1.57f), std::sin(angleRad - 1.57f)) * rayon);
    pointe.setRotation(sf::degrees(angleDeg - 90));
    cible.draw(pointe);

    pointe.setPosition(position + sf::Vector2f(std::cos(angleRad + 1.57f), std::sin(angleRad + 1.57f)) * rayon);
    pointe.setRotation(sf::degrees(angleDeg + 90));
    cible.draw(pointe);
}