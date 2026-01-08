#include "carnivore.hpp"
#include "monde.hpp"
#include "herbivore.hpp"
#include <cmath>

Carnivore::Carnivore(sf::Vector2f pos, bool alpha) 
    : Entite(pos, TypeEntite::CARNIVORE, alpha ? 30.f : 20.f, alpha ? 200.f : 120.f), isAlpha(alpha) {
    vitesse = alpha ? sf::Vector2f(80.f, 60.f) : sf::Vector2f(120.f, 80.f);
}

void Carnivore::update(float dt, Monde& monde) {
    Entite* ami = monde.getPlusProche(position, TypeEntite::CARNIVORE);
    Entite* proie = monde.getPlusProche(position, TypeEntite::HERBIVORE);

    float boostMeute = 1.f;
    if (ami) {
        float distanceAmi = std::hypot(ami->getPosition().x - position.x, ami->getPosition().y - position.y);
        if (distanceAmi < 120.f) boostMeute = isAlpha ? 1.8f : 1.4f;
    }

    bool chasseEnCours = false;
    if (proie) {
        Herbivore* h = dynamic_cast<Herbivore*>(proie);
        if (h && !h->estInvisible()) {
            chasseEnCours = true;
        }
    }

    if (chasseEnCours) {
        sf::Vector2f direction = proie->getPosition() - position;
        float distance = std::hypot(direction.x, direction.y);
        
        if (distance > 0.001f) {
            vitesse = (direction / distance) * ((isAlpha ? 90.f : 130.f) * boostMeute);
        }

        if (distance < rayon + proie->getRayon()) {
            energie += isAlpha ? 80.f : 50.f;
            proie->tuer();
        }
    }

    position += vitesse * dt;

    sf::FloatRect limites = monde.getLimites();

    if (position.x < limites.position.x) { 
        position.x = limites.position.x; 
        vitesse.x = std::abs(vitesse.x); 
    }

    if (position.x > limites.size.x) { 
        position.x = limites.size.x; 
        vitesse.x = -std::abs(vitesse.x); 
    }

    if (position.y < limites.position.y) { 
        position.y = limites.position.y; 
        vitesse.y = std::abs(vitesse.y); 
    }

    if (position.y > limites.size.y) { 
        position.y = limites.size.y; 
        vitesse.y = -std::abs(vitesse.y); 
    }

    energie -= (isAlpha ? 12.f : 8.f) * dt;
    if (energie <= 0) tuer();
}

void Carnivore::dessiner(sf::RenderTarget& cible) const {
    float angleRad = std::atan2(vitesse.y, vitesse.x);
    float angleDeg = angleRad * 180.f / 3.14159f;

    for (int i = 3; i >= 1; --i) {
        float taille = rayon * (0.7f - i * 0.1f);
        
        sf::ConvexShape queue(3);
        queue.setPoint(0, {taille * 1.5f, 0});
        queue.setPoint(1, {-taille, -taille * 0.7f});
        queue.setPoint(2, {-taille, taille * 0.7f});

        float oscillation = std::sin((position.x + position.y) * 0.1f + i) * 15.f;
        
        sf::Vector2f posQueue;
        posQueue.x = position.x - std::cos(angleRad) * (i * rayon * 0.7f);
        posQueue.y = position.y - std::sin(angleRad) * (i * rayon * 0.7f);

        queue.setPosition(posQueue);
        queue.setRotation(sf::degrees(angleDeg + oscillation));
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
    
    sf::Vector2f posOeil;
    posOeil.x = position.x + std::cos(angleRad) * rayon * 0.5f;
    posOeil.y = position.y + std::sin(angleRad) * rayon * 0.5f;
    
    oeil.setPosition(posOeil);
    oeil.setFillColor(isAlpha ? sf::Color::Red : sf::Color::Yellow);
    cible.draw(oeil);
}