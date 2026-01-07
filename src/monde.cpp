#include "monde.hpp"
#include "algue.hpp"
#include "bacterie.hpp"
#include "herbivore.hpp"
#include "carnivore.hpp"
#include <algorithm>
#include <cmath>

Monde::Monde(sf::FloatRect limites) 
    : limites(limites) {
}

void Monde::update(float dt) {
    std::vector<std::unique_ptr<Entite>> nouveau;

    for (auto& e : entites) {
        if (!e->estVivante()) continue;

        sf::Vector2f pos = e->getPosition();
        TypeEntite type = e->getType();

        e->update(dt, *this);

        if (!e->estVivante() && type == TypeEntite::BACTERIE && e->getEnergie() >= 100.f) {
            if (rand() % 2 == 0)
                nouveau.push_back(std::make_unique<Herbivore>(pos));
            else
                nouveau.push_back(std::make_unique<Carnivore>(pos));
        }

        if (e->estVivante() && (type == TypeEntite::HERBIVORE || type == TypeEntite::CARNIVORE)) {
            if (e->getEnergie() >= 180.f) {
                e->setEnergie(90.f); 
                if (type == TypeEntite::HERBIVORE)
                    nouveau.push_back(std::make_unique<Herbivore>(pos + sf::Vector2f(10.f, 10.f)));
                else
                    nouveau.push_back(std::make_unique<Carnivore>(pos + sf::Vector2f(10.f, 10.f)));
            }
        }
    }

    remove();

    for (auto& n : nouveau) {
        entites.push_back(std::move(n));
    }
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

void Monde::reset() {
    entites.clear();
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

Stats Monde::getStats() const {
    Stats s;
    for (const auto& e : entites) {
        if (e->estVivante()) {
            switch (e->getType()) {
                case TypeEntite::ALGUE: s.nbAlgues++; break;
                case TypeEntite::BACTERIE: s.nbBacteries++; break;
                case TypeEntite::HERBIVORE: s.nbHerbivores++; break;
                case TypeEntite::CARNIVORE: s.nbCarnivores++; break;
            }
        }
    }
    return s;
}