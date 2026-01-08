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

void Monde::setTaille(sf::Vector2f taille) {
    limites.size = taille;
}

float muter(float valeur) {
    float variation = (rand() % 20 - 10) / 100.f;
    return valeur * (1.0f + variation);
}

void Monde::update(float dt) {
    std::vector<std::unique_ptr<Entite>> nouveau;

    for (auto& e : entites) {
        if (!e->estVivante()) continue;

        e->update(dt, *this);

        sf::Vector2f pos = e->getPosition();
        if (pos.x < 0) e->setPosition({0.f, pos.y});
        if (pos.y < 0) e->setPosition({pos.x, 0.f});
        if (pos.x > limites.size.x) e->setPosition({limites.size.x, pos.y});
        if (pos.y > limites.size.y) e->setPosition({pos.x, limites.size.y});

        if (e->getType() != TypeEntite::ROCHER) {
            for (auto& r : entites) {
                if (r->getType() == TypeEntite::ROCHER) {
                    sf::Vector2f diff = e->getPosition() - r->getPosition();
                    float dist = std::hypot(diff.x, diff.y);
                    float min = e->getRayon() + r->getRayon();
                    
                    if (dist < min) {
                        if (dist > 0.001f) {
                            e->setPosition(r->getPosition() + (diff / dist) * min);
                        } else {
                            e->setPosition(r->getPosition() + sf::Vector2f(min, 0.f));
                        }
                    }
                }
            }
        }

        if (e->estVivante() && e->getType() == TypeEntite::BACTERIE && e->getEnergie() >= 110.f) {
            e->tuer();

            int choix = rand() % 100;

            if (choix < 50) { 
                int sousChoix = rand() % 3;
                
                if (sousChoix == 0) {
                    nouveau.push_back(std::make_unique<Herbivore>(e->getPosition(), 90.f, 15.f, 150.f));
                } 
                else if (sousChoix == 1) {
                    nouveau.push_back(std::make_unique<Herbivore>(e->getPosition(), 140.f, 10.f, 250.f));
                }
                else {
                    nouveau.push_back(std::make_unique<Herbivore>(e->getPosition(), 60.f, 25.f, 300.f));
                }
            } 
            else { 
                int sousChoix = rand() % 3;

                if (sousChoix == 0) {
                    nouveau.push_back(std::make_unique<Carnivore>(e->getPosition(), 110.f, 20.f, 200.f));
                } 
                else if (sousChoix == 1) {
                    nouveau.push_back(std::make_unique<Carnivore>(e->getPosition(), 145.f, 25.f, 350.f));
                }
                else {
                    nouveau.push_back(std::make_unique<Carnivore>(e->getPosition(), 160.f, 12.f, 250.f));
                }
            }
        }

        if (e->estVivante() && e->getEnergie() >= 200.f) {
            if (e->getType() == TypeEntite::HERBIVORE) {
                Herbivore* parent = dynamic_cast<Herbivore*>(e.get());
                e->setEnergie(60.f);
                
                float vitesse = muter(parent->getVitesseMax());
                float rayon = muter(parent->getRayon());
                float vision = muter(parent->getPorteeVue());
                
                nouveau.push_back(std::make_unique<Herbivore>(e->getPosition() + sf::Vector2f(10.f, 10.f), vitesse, rayon, vision));
            }
            else if (e->getType() == TypeEntite::CARNIVORE) {
                Carnivore* parent = dynamic_cast<Carnivore*>(e.get());
                e->setEnergie(60.f);
                
                float vitesse = muter(parent->getVitesseMax());
                float rayon = muter(parent->getRayon());
                float vision = muter(parent->getPorteeVue());

                nouveau.push_back(std::make_unique<Carnivore>(e->getPosition() + sf::Vector2f(10.f, 10.f), vitesse, rayon, vision));
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
            Rocher* r = static_cast<Rocher*>(e.get());
            
            if (!r->estOccupe()) {
                sf::Vector2f diff = pos - e->getPosition();
                float dist = std::hypot(diff.x, diff.y);
                if (dist < e->getRayon() + 20.f) {
                    r->setOccupe(true);
                    return true;
                }
            }
        }
    }
    return false;
}

void Monde::libererRocherProche(sf::Vector2f pos) {
    for (auto& e : entites) {
        if (e->getType() == TypeEntite::ROCHER) {
            float d = std::hypot(e->getPosition().x - pos.x, e->getPosition().y - pos.y);
            if (d < e->getRayon() + 25.f) {
                static_cast<Rocher*>(e.get())->setOccupe(false); //
                return;
            }
        }
    }
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