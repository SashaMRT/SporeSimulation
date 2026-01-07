#include "monde.hpp"
#include "algue.hpp"
#include "bacterie.hpp"
#include <algorithm>
#include <cmath>

Monde::Monde() 
    : limites(sf::Vector2f(0, 0), sf::Vector2f(900, 720)) {
}

Monde::Monde(sf::FloatRect limites) 
    : limites(limites) {
}

void Monde::update(float dt) {
    for (auto& e : entites) {
        if (e->estVivante()) {
            e->update(dt, *this);
        }
    }
    remove();
}

void Monde::dessiner(sf::RenderTarget& cible) const {
    for (const auto& e : entites) {
        if (e->estVivante()) {
            e->dessiner(cible);
        }
    }
}

void Monde::spawnAlgue(sf::Vector2f pos) {
    entites.emplace_back(std::make_unique<Algue>(pos));
}

void Monde::spawnBacterie(sf::Vector2f pos) {
    entites.emplace_back(std::make_unique<Bacterie>(pos));
}

Entite* Monde::getPlusProche(sf::Vector2f pos, TypeEntite typeCherche) {
    Entite* cible = nullptr;
    float distMin = 1e9f;

    for (auto& e : entites) {
        if (e->estVivante() && e->getType() == typeCherche) {
            float dx = e->getPosition().x - pos.x;
            float dy = e->getPosition().y - pos.y;
            float d = std::sqrt(dx * dx + dy * dy);
            if (d < distMin) {
                distMin = d;
                cible = e.get();
            }
        }
    }
    return cible;
}

void Monde::remove() {
    std::erase_if(entites, [](const auto& e) { 
        return !e->estVivante(); 
    });
}