#include "carnivore.hpp"
#include "monde.hpp"
#include <cmath>

Carnivore::Carnivore(sf::Vector2f pos, bool alpha) 
    : Entite(pos, TypeEntite::CARNIVORE, alpha ? 30.f : 20.f, alpha ? 200.f : 120.f), isAlpha(alpha) {
    vitesse = alpha ? sf::Vector2f(80.f, 60.f) : sf::Vector2f(120.f, 80.f);
}

void Carnivore::update(float dt, Monde& monde) {
    Entite* ami = monde.getPlusProche(position, TypeEntite::CARNIVORE);
    Entite* cible = monde.getPlusProche(position, TypeEntite::HERBIVORE);

    float boostMeute = 1.f;
    if (ami) {
        float distAmi = std::hypot(ami->getPosition().x - position.x, ami->getPosition().y - position.y);
        if (distAmi < 120.f) boostMeute = isAlpha ? 1.8f : 1.4f;
    }

    if (cible) {
        sf::Vector2f direction = cible->getPosition() - position;
        float distance = std::hypot(direction.x, direction.y);
        float vitesseChasse = (isAlpha ? 90.f : 130.f) * boostMeute;

        vitesse = (direction / distance) * vitesseChasse;

        if (distance < rayon + cible->getRayon()) {
            energie += isAlpha ? 80.f : 50.f;
            cible->tuer();
        }
    }

    position += vitesse * dt;

    sf::FloatRect limites = monde.getLimites();
    if (position.x < limites.position.x || position.x > limites.size.x) vitesse.x *= -1;
    if (position.y < limites.position.y || position.y > limites.size.y) vitesse.y *= -1;

    energie -= (isAlpha ? 12.f : 8.f) * dt;
    if (energie <= 0) tuer();
}

void Carnivore::dessiner(sf::RenderTarget& cible) const {
    float angleRad = std::atan2(vitesse.y, vitesse.x);
    float angleDeg = angleRad * 180.f / 3.14159f;

    for (int i = 3; i >= 1; --i) {
        float tailleQ = rayon * (0.7f - i * 0.1f);
        sf::ConvexShape queue(3);
        queue.setPoint(0, {tailleQ * 1.5f, 0});
        queue.setPoint(1, {-tailleQ, -tailleQ * 0.7f});
        queue.setPoint(2, {-tailleQ, tailleQ * 0.7f});

        float offsetX = std::cos(angleRad) * (i * rayon * 0.7f);
        float offsetY = std::sin(angleRad) * (i * rayon * 0.7f);
        queue.setPosition({position.x - offsetX, position.y - offsetY});
        
        float anim = std::sin((position.x + position.y) * 0.1f + i) * 15.f;
        queue.setRotation(sf::degrees(angleDeg + anim));
        queue.setFillColor(isAlpha ? sf::Color(60, 60, 60) : sf::Color(180 - i * 20, 30, 30));
        cible.draw(queue);
    }

    sf::ConvexShape corps(4);
    corps.setPoint(0, {rayon * 1.8f, 0});
    corps.setPoint(1, {0, -rayon});
    corps.setPoint(2, {-rayon * 0.8f, 0});
    corps.setPoint(3, {0, rayon});
    corps.setPosition(position);
    corps.setRotation(sf::degrees(angleDeg));
    corps.setFillColor(isAlpha ? sf::Color(40, 40, 40) : sf::Color(220, 50, 50));
    corps.setOutlineThickness(2.f);
    corps.setOutlineColor(isAlpha ? sf::Color(255, 215, 0) : sf::Color(150, 20, 20));
    cible.draw(corps);

    sf::CircleShape oeil(rayon * 0.25f);
    oeil.setOrigin({oeil.getRadius(), oeil.getRadius()});
    float oeilX = std::cos(angleRad) * rayon * 0.5f;
    float oeilY = std::sin(angleRad) * rayon * 0.5f;
    oeil.setPosition({position.x + oeilX, position.y + oeilY});
    oeil.setFillColor(isAlpha ? sf::Color::Red : sf::Color::Yellow);
    cible.draw(oeil);
}