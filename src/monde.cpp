#include "monde.hpp"
#include "algue.hpp"
#include <algorithm>

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

void Monde::remove() {
    std::vector<std::unique_ptr<Entite>> vivants;
    for (auto& e : entites) {
        if (e->estVivante()) {
            vivants.push_back(std::move(e));
        }
    }
    entites = std::move(vivants);
}
