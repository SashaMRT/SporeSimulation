#include "herbivore.hpp"
#include "monde.hpp"
#include <cmath>

Herbivore::Herbivore(sf::Vector2f pos, bool rapide) 
    : Entite(pos, TypeEntite::HERBIVORE, rapide ? 10.f : 15.f, rapide ? 70.f : 100.f), isRapide(rapide) {
    vitesse = rapide ? sf::Vector2f(160.f, 80.f) : sf::Vector2f(100.f, 50.f);
}

void Herbivore::update(float dt, Monde& monde) {
    Entite* ami = monde.getPlusProche(position, TypeEntite::HERBIVORE);
    Entite* danger = monde.getPlusProche(position, TypeEntite::CARNIVORE);
    Entite* miam = monde.getPlusProche(position, TypeEntite::ALGUE);

    float vue = isRapide ? 400.f : 150.f;
    if (ami && std::hypot(ami->getPosition().x - position.x, ami->getPosition().y - position.y) < 80.f) vue *= 2.f;

    if (danger && std::hypot(danger->getPosition().x - position.x, danger->getPosition().y - position.y) < vue) {
        sf::Vector2f directionFuite = position - danger->getPosition();
        vitesse = (directionFuite / std::hypot(directionFuite.x, directionFuite.y)) * (isRapide ? 180.f : 125.f);
    } else if (miam) {
        sf::Vector2f directionMiam = miam->getPosition() - position;
        float dist = std::hypot(directionMiam.x, directionMiam.y);
        vitesse = (directionMiam / dist) * (isRapide ? 130.f : 90.f);
        if (dist < rayon + miam->getRayon()) { miam->tuer(); energie += (isRapide ? 15.f : 30.f); }
    }

    position += vitesse * dt;
    sf::FloatRect limites = monde.getLimites();
    if (position.x < limites.position.x || position.x > limites.size.x) vitesse.x *= -1;
    if (position.y < limites.position.y || position.y > limites.size.y) vitesse.y *= -1;
    
    energie -= (isRapide ? 7.f : 5.f) * dt;
    if (energie <= 0) tuer();
}

void Herbivore::dessiner(sf::RenderTarget& cible) const {
    float angle = std::atan2(vitesse.y, vitesse.x);
    sf::Color couleur = isRapide ? sf::Color(0, 191, 255, 200) : sf::Color(50, 220, 100, 200);

    int nbT = isRapide ? 3 : 5;
    for (int i = 0; i < nbT; ++i) {
        float angleBase = angle + 3.14f + (i - nbT / 2.f) * 0.4f;
        for (int k = 0; k < 6; ++k) {
            float anim = std::sin((position.x + position.y) * 0.05f + k * 0.5f) * 0.2f;
            sf::CircleShape t(rayon * 0.2f);
            t.setOrigin({t.getRadius(), t.getRadius()});
            sf::Vector2f posT(position.x + std::cos(angleBase + anim) * (rayon * (0.8f + k * 0.4f)),
                              position.y + std::sin(angleBase + anim) * (rayon * (0.8f + k * 0.4f)));
            t.setPosition(posT);
            t.setFillColor(isRapide ? sf::Color(135, 206, 250, 150) : sf::Color(100, 255, 100, 150));
            cible.draw(t);
        }
    }
    sf::CircleShape corps(rayon);
    corps.setOrigin({rayon, rayon});
    corps.setPosition(position);
    corps.setFillColor(couleur);
    corps.setOutlineThickness(2.f);
    corps.setOutlineColor(sf::Color::White);
    cible.draw(corps);
}