#include "herbivore.hpp"
#include "monde.hpp"
#include <cmath>

Herbivore::Herbivore(sf::Vector2f pos, bool rapide) 
    : Entite(pos, TypeEntite::HERBIVORE, rapide ? 10.f : 15.f, rapide ? 70.f : 100.f), isRapide(rapide) {
    vitesse = rapide ? sf::Vector2f(160.f, 80.f) : sf::Vector2f(100.f, 50.f);
    invisible = false;
    chronoCache = 0.f;
    chronoRecharge = 0.f;
}

void Herbivore::update(float dt, Monde& monde) {
    if (chronoRecharge > 0.f) chronoRecharge -= dt;

    if (invisible) {
        chronoCache += dt;
        if (chronoCache >= 10.f) {
            invisible = false;
            chronoCache = 0.f;
            chronoRecharge = 20.f;
        }
        return;
    }

    if (chronoRecharge <= 0.f && monde.estCache(position)) {
        invisible = true;
        return;
    }

    Entite* ami = monde.getPlusProche(position, TypeEntite::HERBIVORE);
    Entite* danger = monde.getPlusProche(position, TypeEntite::CARNIVORE);
    Entite* nourriture = monde.getPlusProche(position, TypeEntite::ALGUE);

    float vue = isRapide ? 400.f : 150.f;
    if (ami) {
        float distAmi = std::hypot(ami->getPosition().x - position.x, ami->getPosition().y - position.y);
        if (distAmi < 80.f) vue *= 2.f;
    }

    bool enFuite = false;

    if (danger) {
        sf::Vector2f fuite = position - danger->getPosition();
        float distDanger = std::hypot(fuite.x, fuite.y);

        if (distDanger < vue) {
            vitesse = (fuite / distDanger) * (isRapide ? 180.f : 125.f);
            enFuite = true;
        }
    }

    if (!enFuite && nourriture) {
        sf::Vector2f versMiam = nourriture->getPosition() - position;
        float distMiam = std::hypot(versMiam.x, versMiam.y);
        
        vitesse = (versMiam / distMiam) * (isRapide ? 130.f : 90.f);
        
        if (distMiam < rayon + nourriture->getRayon()) {
            nourriture->tuer();
            energie += (isRapide ? 15.f : 30.f);
        }
    }

    position += vitesse * dt;

    sf::FloatRect limites = monde.getLimites();
    if (position.x < limites.position.x || position.x > limites.size.x) vitesse.x *= -1;
    if (position.y < limites.position.y || position.y > limites.size.y) vitesse.y *= -1;

    energie -= (isRapide ? 7.f : 5.f) * dt;
    if (energie <= 0) tuer();
}

void Herbivore::dessiner(sf::RenderTarget& cible) const {
    if (invisible) return;

    float angle = std::atan2(vitesse.y, vitesse.x);
    sf::Color couleur = isRapide ? sf::Color(0, 191, 255, 200) : sf::Color(50, 220, 100, 200);

    int nbTentacules = isRapide ? 3 : 5;
    for (int i = 0; i < nbTentacules; ++i) {
        float angleDepart = angle + 3.14f + (i - nbTentacules / 2.f) * 0.4f;
        
        for (int k = 0; k < 6; ++k) {
            float animation = std::sin((position.x + position.y) * 0.05f + k * 0.5f) * 0.2f;
            float distance = rayon * (0.8f + k * 0.4f);
            
            sf::Vector2f posT;
            posT.x = position.x + std::cos(angleDepart + animation) * distance;
            posT.y = position.y + std::sin(angleDepart + animation) * distance;

            sf::CircleShape t(rayon * 0.2f);
            t.setOrigin({t.getRadius(), t.getRadius()});
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