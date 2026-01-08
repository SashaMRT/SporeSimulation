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
            monde.libererRocherProche(position);
        }
        return;
    }
    Entite* danger = monde.getPlusProche(position, TypeEntite::CARNIVORE);
    float vue = isRapide ? 400.f : 150.f;
    bool estChasse = false;
    float distDanger = 0.f;

    if (danger) {
        sf::Vector2f diff = position - danger->getPosition();
        distDanger = std::hypot(diff.x, diff.y);
        if (distDanger < vue) estChasse = true;
    }

    if (estChasse) {
        if (chronoRecharge <= 0.f && monde.estCache(position)) {
            invisible = true;
            vitesse = {0.f, 0.f};
            return;
        } 
        
        sf::Vector2f fuite = position - danger->getPosition();
        vitesse = (fuite / distDanger) * (isRapide ? 180.f : 125.f);
    } 
    
    else {
        Entite* miam = monde.getPlusProche(position, TypeEntite::ALGUE);
        if (miam) {
            sf::Vector2f dir = miam->getPosition() - position;
            float dist = std::hypot(dir.x, dir.y);
            vitesse = (dir / dist) * (isRapide ? 130.f : 90.f);
            
            if (dist < rayon + miam->getRayon()) { 
                miam->tuer(); 
                energie += (isRapide ? 15.f : 30.f); 
            }
        }
    }

    position += vitesse * dt;

    sf::FloatRect l = monde.getLimites();
    if (position.x < l.position.x || position.x > l.size.x) vitesse.x *= -1;
    if (position.y < l.position.y || position.y > l.size.y) vitesse.y *= -1;

    if ((energie -= (isRapide ? 7.f : 5.f) * dt) <= 0) tuer();
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