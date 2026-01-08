#include "monde.hpp"
#include "algue.hpp"
#include "bacterie.hpp"
#include "herbivore.hpp"
#include "carnivore.hpp"
#include "rocher.hpp"
#include <algorithm>
#include <cmath>

Monde::Monde(sf::FloatRect limites) 
    : limites(limites) {
}

void Monde::update(float dt) {
    std::vector<std::unique_ptr<Entite>> nouveau;

    for (auto& e : entites) {
        if (!e->estVivante()) continue;

        e->update(dt, *this);

        if (e->getType() != TypeEntite::ROCHER) {
            for (auto& r : entites) {
                if (r->getType() == TypeEntite::ROCHER) {
                    sf::Vector2f diff = e->getPosition() - r->getPosition();
                    float dist = std::hypot(diff.x, diff.y);
                    float min = e->getRayon() + r->getRayon();
                    if (dist < min) {
                        e->setPosition(r->getPosition() + (diff / dist) * min);
                    }
                }
            }
        }

        if (e->estVivante() && e->getType() == TypeEntite::BACTERIE && e->getEnergie() >= 100.f) {
            e->tuer();
            bool var = (rand() % 2 == 0);
            if (rand() % 2 == 0) nouveau.push_back(std::make_unique<Herbivore>(e->getPosition(), var));
            else nouveau.push_back(std::make_unique<Carnivore>(e->getPosition(), var));
        }

        if (e->estVivante() && (e->getType() == TypeEntite::HERBIVORE || e->getType() == TypeEntite::CARNIVORE)) {
            if (e->getEnergie() >= 200.f) {
                e->setEnergie(60.f);
                bool var = (rand() % 2 == 0);
                sf::Vector2f off(30.f, 30.f);
                if (e->getType() == TypeEntite::HERBIVORE)
                    nouveau.push_back(std::make_unique<Herbivore>(e->getPosition() + off, var));
                else
                    nouveau.push_back(std::make_unique<Carnivore>(e->getPosition() + off, var));
            }
        }
    }

    remove();
    for (auto& n : nouveau) entites.push_back(std::move(n));
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

void Monde::spawnRocher(sf::Vector2f pos) {
    entites.emplace_back(std::make_unique<Rocher>(pos));
}

void Monde::gererCollisionsRochers(Entite& e) {
    if (e.getType() == TypeEntite::ROCHER) return;

    for (auto& r : entites) {
        if (r->estVivante() && r->getType() == TypeEntite::ROCHER) {
            sf::Vector2f diff = e.getPosition() - r->getPosition();
            float dist = std::hypot(diff.x, diff.y);
            float distMin = e.getRayon() + r->getRayon();

            if (dist < distMin) {
                sf::Vector2f direction = diff / dist;
                e.setEnergie(e.getEnergie() - 0.01f);
            }
        }
    }
}

bool Monde::estCache(sf::Vector2f pos) const {
    for (const auto& e : entites) {
        if (e->estVivante() && e->getType() == TypeEntite::ROCHER) {
            sf::Vector2f diff = pos - e->getPosition();
            float dist = std::hypot(diff.x, diff.y);
            if (dist < e->getRayon() + 10.f) return true;
        }
    }
    return false;
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
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < distMin) {
                distMin = distance;
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

Entite* Monde::getEntiteSousSouris(sf::Vector2f pos, float rayon) {
    Entite* cible = nullptr;
    float rayonCarre = rayon * rayon;

    for (const auto& e : entites) {
        if (!e->estVivante()) continue;
        
        float dx = e->getPosition().x - pos.x;
        float dy = e->getPosition().y - pos.y;
        float distanceCarree = dx*dx + dy*dy;

        if (distanceCarree < rayonCarre) {
            rayonCarre = distanceCarree;
            cible = e.get();
        }
    }
    return cible;
}