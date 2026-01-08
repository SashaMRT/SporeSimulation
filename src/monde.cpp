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

    for (auto& ent : entites) {
        if (!ent->estVivante()) continue;

        ent->update(dt, *this);

        sf::Vector2f pos = ent->getPosition();
        if (pos.x < 0) ent->setPosition({0.f, pos.y});
        if (pos.y < 0) ent->setPosition({pos.x, 0.f});
        if (pos.x > limites.size.x) ent->setPosition({limites.size.x, pos.y});
        if (pos.y > limites.size.y) ent->setPosition({pos.x, limites.size.y});

        if (ent->getType() != TypeEntite::ROCHER) {
            for (auto& r : entites) {
                if (r->getType() == TypeEntite::ROCHER) {
                    sf::Vector2f diff = ent->getPosition() - r->getPosition();
                    float dist = std::hypot(diff.x, diff.y);
                    float min = ent->getRayon() + r->getRayon();
                    
                    if (dist < min) {
                        if (dist > 0.001f) {
                            ent->setPosition(r->getPosition() + (diff / dist) * min);
                        } else {
                            ent->setPosition(r->getPosition() + sf::Vector2f(min, 0.f));
                        }
                    }
                }
            }
        }

        if (ent->estVivante() && ent->getType() == TypeEntite::BACTERIE && ent->getEnergie() >= 110.f) {
            creerExplosion(ent->getPosition(), sf::Color(255, 215, 0), 20);
            ent->tuer();

            int choix = rand() % 100;

            if (choix < 50) { 
                int sousChoix = rand() % 3;
                
                if (sousChoix == 0) {
                    nouveau.push_back(std::make_unique<Herbivore>(ent->getPosition(), 90.f, 15.f, 150.f));
                } 
                else if (sousChoix == 1) {
                    nouveau.push_back(std::make_unique<Herbivore>(ent->getPosition(), 140.f, 10.f, 250.f));
                }
                else {
                    nouveau.push_back(std::make_unique<Herbivore>(ent->getPosition(), 60.f, 25.f, 300.f));
                }
                naissancesHerbivores++;
            } 
            else { 
                int sousChoix = rand() % 3;

                if (sousChoix == 0) {
                    nouveau.push_back(std::make_unique<Carnivore>(ent->getPosition(), 110.f, 20.f, 200.f));
                } 
                else if (sousChoix == 1) {
                    nouveau.push_back(std::make_unique<Carnivore>(ent->getPosition(), 145.f, 25.f, 350.f));
                }
                else {
                    nouveau.push_back(std::make_unique<Carnivore>(ent->getPosition(), 160.f, 12.f, 250.f));
                }
                naissancesCarnivores++;
            }
        }

        if (ent->estVivante() && ent->getEnergie() >= 200.f) {
            if (ent->getType() == TypeEntite::HERBIVORE) {
                creerExplosion(ent->getPosition(), sf::Color(100, 255, 150), 15);
                Herbivore* parent = dynamic_cast<Herbivore*>(ent.get());
                ent->setEnergie(60.f);
                
                float vitesse = muter(parent->getVitesseMax());
                float rayon = muter(parent->getRayon());
                float vision = muter(parent->getPorteeVue());
                
                nouveau.push_back(std::make_unique<Herbivore>(ent->getPosition() + sf::Vector2f(10.f, 10.f), vitesse, rayon, vision));
                naissancesHerbivores++;
            }
            else if (ent->getType() == TypeEntite::CARNIVORE) {
                creerExplosion(ent->getPosition(), sf::Color(255, 100, 100), 15);
                Carnivore* parent = dynamic_cast<Carnivore*>(ent.get());
                ent->setEnergie(60.f);
                
                float vitesse = muter(parent->getVitesseMax());
                float rayon = muter(parent->getRayon());
                float vision = muter(parent->getPorteeVue());

                nouveau.push_back(std::make_unique<Carnivore>(ent->getPosition() + sf::Vector2f(10.f, 10.f), vitesse, rayon, vision));
                naissancesCarnivores++;
            }
        }
    }

    for (const auto& ent : entites) {
        if (!ent->estVivante()) {
            switch(ent->getType()) {
                case TypeEntite::ALGUE: mortsAlgues++; break;
                case TypeEntite::BACTERIE: mortsBacteries++; break;
                case TypeEntite::HERBIVORE: mortsHerbivores++; break;
                case TypeEntite::CARNIVORE: mortsCarnivores++; break;
                default: break;
            }

            sf::Color couleurParticule = sf::Color::White;

            switch(ent->getType()) {
                case TypeEntite::ALGUE:
                    couleurParticule = sf::Color(34, 139, 34);
                    break;
                case TypeEntite::BACTERIE:
                    couleurParticule = sf::Color(0, 255, 255);
                    break;
                case TypeEntite::HERBIVORE: {
                    Herbivore* h = static_cast<Herbivore*>(ent.get());
                    couleurParticule = h->estRapide() ? sf::Color(0, 191, 255) : sf::Color(50, 220, 100);
                    libererRocherProche(ent->getPosition());
                    break;
                }
                case TypeEntite::CARNIVORE:
                    couleurParticule = sf::Color(255, 50, 50);
                    break;
                default:
                    break;
            }

            int nbParticules = static_cast<int>(ent->getRayon() * 1.5f);
            nbParticules = std::clamp(nbParticules, 5, 30);
            
            creerExplosion(ent->getPosition(), couleurParticule, nbParticules);
        }
    }

    for (auto it = particules.begin(); it != particules.end();) {
        it->update(dt);
        if (it->vie <= 0) it = particules.erase(it);
        else ++it;
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

    for (const auto& p : particules) {
        p.dessiner(cible);
    }
}

void Monde::spawnAlgue(sf::Vector2f pos) {
    entites.emplace_back(std::make_unique<Algue>(pos));
    naissancesAlgues++;
}

void Monde::spawnBacterie(sf::Vector2f pos) {
    entites.emplace_back(std::make_unique<Bacterie>(pos));
    naissancesBacteries++;
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
                    const_cast<Rocher*>(r)->setOccupe(true);
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
                static_cast<Rocher*>(e.get())->setOccupe(false);
                return;
            }
        }
    }
}

void Monde::creerExplosion(sf::Vector2f pos, sf::Color couleur, int nb) {
    for (int i = 0; i < nb; ++i) {
        float angle = (rand() % 360) * 3.14159f / 180.f;
        float vitesse = (rand() % 80) + 30.f;
        float variationCouleur = (rand() % 40) - 20;
        
        sf::Color couleurVar = couleur;
        if (couleur.r + variationCouleur >= 0 && couleur.r + variationCouleur <= 255)
            couleurVar.r += variationCouleur;
        if (couleur.g + variationCouleur >= 0 && couleur.g + variationCouleur <= 255)
            couleurVar.g += variationCouleur;
        if (couleur.b + variationCouleur >= 0 && couleur.b + variationCouleur <= 255)
            couleurVar.b += variationCouleur;
        
        sf::Vector2f vit(std::cos(angle) * vitesse, std::sin(angle) * vitesse);
        particules.push_back(Particule(pos, vit, couleurVar));
    }
}

void Monde::dessinerFond(sf::RenderTarget& cible) const {
    sf::Vector2f taille = limites.size;

    sf::VertexArray degrade(sf::PrimitiveType::TriangleStrip, 4);

    degrade[0].position = {0, 0};
    degrade[0].color = sf::Color(0, 105, 148);
    degrade[2].position = {taille.x, 0};
    degrade[2].color = sf::Color(0, 105, 148);

    degrade[1].position = {0, taille.y};
    degrade[1].color = sf::Color(2, 12, 27);
    degrade[3].position = {taille.x, taille.y};
    degrade[3].color = sf::Color(2, 12, 27);

    cible.draw(degrade);
}

void Monde::reset() {
    entites.clear();
    mortsAlgues = 0;
    mortsBacteries = 0;
    mortsHerbivores = 0;
    mortsCarnivores = 0;
    naissancesAlgues = 0;
    naissancesBacteries = 0;
    naissancesHerbivores = 0;
    naissancesCarnivores = 0;
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
    
    s.algues.morts = mortsAlgues;
    s.algues.naissances = naissancesAlgues;
    s.bacteries.morts = mortsBacteries;
    s.bacteries.naissances = naissancesBacteries;
    s.herbivores.morts = mortsHerbivores;
    s.herbivores.naissances = naissancesHerbivores;
    s.carnivores.morts = mortsCarnivores;
    s.carnivores.naissances = naissancesCarnivores;

    for (const auto& e : entites) {
        if (e->estVivante()) {
            switch (e->getType()) {
                case TypeEntite::ALGUE: s.algues.vivants++; break;
                case TypeEntite::BACTERIE: s.bacteries.vivants++; break;
                case TypeEntite::HERBIVORE: s.herbivores.vivants++; break;
                case TypeEntite::CARNIVORE: s.carnivores.vivants++; break;
                default: break;
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