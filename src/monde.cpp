/**
 * @file monde.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Monde.
 * @details Gère la logique centrale de la simulation : mise à jour des entités,
 * gestion des collisions, cycle de vie (naissances/morts), et statistiques.
 */

/**
 * @brief Constructeur de la classe Monde.
 * @param limites Les dimensions (rectangle) de la zone de simulation.
 */
Monde::Monde(sf::FloatRect limites) 
    : limites(limites) {
}

/**
 * @brief Définit la taille du monde.
 * @param taille Nouveau vecteur de taille (largeur, hauteur).
 */
void Monde::setTaille(sf::Vector2f taille) {
    limites.size = taille;
}

/**
 * @brief Fonction utilitaire pour appliquer une mutation génétique.
 * * Applique une variation aléatoire (positive ou négative) à une caractéristique
 * en fonction du taux de mutation défini dans les constantes.
 * * @param valeur La valeur de base (ex: vitesse, rayon).
 * @return La nouvelle valeur mutée.
 */
float muter(float valeur) {
    // Variation entre -TAUX_MUTATION et +TAUX_MUTATION
    float variation = (rand() % 200 - 100) / 100.f * Constantes::TAUX_MUTATION;
    return valeur * (1.0f + variation);
}

/**
 * @brief Met à jour toute la simulation pour une frame donnée.
 * * Cette méthode orchestre :
 * - Le spawn aléatoire d'algues.
 * - Le déplacement et comportement de chaque entité.
 * - La gestion des bords du monde (effet tore/pac-man).
 * - Les collisions physiques avec les rochers.
 * - L'évolution des bactéries et la reproduction des animaux.
 * - La gestion des morts et l'apparition des particules.
 * * @param dt Temps écoulé depuis la dernière mise à jour (Delta Time).
 */
void Monde::update(float dt) {
    // Liste tampon pour stocker les entités nées durant cette frame
    std::vector<std::unique_ptr<Entite>> nouveau;

    for (auto& ent : entites) {
        // Chance aléatoire de faire apparaître une algue (pour chaque entité traitée)
        if (rand() % 5000 == 0) {
            float x = static_cast<float>(rand() % static_cast<int>(limites.size.x));
            float y = static_cast<float>(rand() % static_cast<int>(limites.size.y));
            nouveau.push_back(std::make_unique<Algue>(sf::Vector2f(x, y)));
            naissancesAlgues++;
        }

        // Si l'entité est morte, on passe à la suivante (elle sera nettoyée plus tard)
        if (!ent->estVivante()) continue;

        // Mise à jour de l'IA et de la physique de l'entité
        ent->update(dt, *this);

        // --- Gestion des limites du monde (Topologie Torique) ---
        // Si une entité sort d'un côté, elle réapparaît de l'autre
        sf::Vector2f pos = ent->getPosition();
        if (pos.x < 0) ent->setPosition({0.f, pos.y});
        if (pos.y < 0) ent->setPosition({pos.x, 0.f});
        if (pos.x > limites.size.x) ent->setPosition({limites.size.x, pos.y});
        if (pos.y > limites.size.y) ent->setPosition({pos.x, limites.size.y});

        // --- Collision avec les rochers (Repousse) ---
        if (ent->getType() != TypeEntite::ROCHER) {
            for (auto& r : entites) {
                if (r->getType() == TypeEntite::ROCHER) {
                    sf::Vector2f diff = ent->getPosition() - r->getPosition();
                    float dist = std::hypot(diff.x, diff.y);
                    float min = ent->getRayon() + r->getRayon();
                    
                    // Si collision détectée, on repousse l'entité hors du rocher
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

        // --- EVOLUTION DES BACTERIES ---
        // Si une bactérie a accumulé assez d'énergie, elle évolue en animal
        if (ent->estVivante() && ent->getType() == TypeEntite::BACTERIE && ent->getEnergie() >= Constantes::BACTERIE_SEUIL_EVOLUTION) {
            creerExplosion(ent->getPosition(), sf::Color(255, 215, 0), Constantes::PARTICULE_NB_EXPLOSION);
            ent->tuer(); // La bactérie meurt pour devenir autre chose

            int choix = rand() % 100;

            if (choix < 50) { 
                // Devient un Herbivore (3 variantes possibles)
                int sousChoix = rand() % 3;
                if (sousChoix == 0) nouveau.push_back(std::make_unique<Herbivore>(ent->getPosition(), Constantes::HERBIVORE_VITESSE_BASE, Constantes::HERBIVORE_RAYON_BASE, Constantes::HERBIVORE_ENERGIE_BASE));
                else if (sousChoix == 1) nouveau.push_back(std::make_unique<Herbivore>(ent->getPosition(), Constantes::HERBIVORE_VITESSE_BASE * 1.5f, Constantes::HERBIVORE_RAYON_BASE * 0.7f, Constantes::HERBIVORE_ENERGIE_BASE * 1.6f));
                else nouveau.push_back(std::make_unique<Herbivore>(ent->getPosition(), Constantes::HERBIVORE_VITESSE_BASE * 0.6f, Constantes::HERBIVORE_RAYON_BASE * 1.6f, Constantes::HERBIVORE_ENERGIE_BASE * 2.0f));
                naissancesHerbivores++;
            } 
            else { 
                // Devient un Carnivore (3 variantes possibles)
                int sousChoix = rand() % 3;
                if (sousChoix == 0) nouveau.push_back(std::make_unique<Carnivore>(ent->getPosition(), Constantes::CARNIVORE_VITESSE_BASE, Constantes::CARNIVORE_RAYON_BASE, Constantes::CARNIVORE_ENERGIE_BASE));
                else if (sousChoix == 1) nouveau.push_back(std::make_unique<Carnivore>(ent->getPosition(), Constantes::CARNIVORE_VITESSE_BASE * 1.3f, Constantes::CARNIVORE_RAYON_BASE * 1.2f, Constantes::CARNIVORE_ENERGIE_BASE * 1.7f));
                else nouveau.push_back(std::make_unique<Carnivore>(ent->getPosition(), Constantes::CARNIVORE_VITESSE_BASE * 1.45f, Constantes::CARNIVORE_RAYON_BASE * 0.6f, Constantes::CARNIVORE_ENERGIE_BASE * 1.2f));
                naissancesCarnivores++;
            }
        }

        // --- REPRODUCTION & EVOLUTION (HERBIVORE / CARNIVORE) ---
        if (ent->estVivante() && ent->getEnergie() >= Constantes::ENERGIE_REPRODUCTION) {
            
            if (ent->getType() == TypeEntite::HERBIVORE) {
                creerExplosion(ent->getPosition(), sf::Color(100, 255, 150), 15);
                Herbivore* parent = dynamic_cast<Herbivore*>(ent.get());
                ent->setEnergie(Constantes::COUT_REPRODUCTION); // Le parent perd de l'énergie
                
                // Mutation des stats pour l'enfant
                float vitesse = muter(parent->getVitesseMax());
                float rayon = muter(parent->getRayon());
                float vision = muter(parent->getPorteeVue());
                
                nouveau.push_back(std::make_unique<Herbivore>(ent->getPosition() + sf::Vector2f(10.f, 10.f), vitesse, rayon, vision));
                naissancesHerbivores++;
            }
            else if (ent->getType() == TypeEntite::CARNIVORE) {
                creerExplosion(ent->getPosition(), sf::Color(255, 100, 100), 15);
                Carnivore* parent = dynamic_cast<Carnivore*>(ent.get());
                ent->setEnergie(Constantes::COUT_REPRODUCTION); // Le parent perd de l'énergie
                
                // Mutation des stats pour l'enfant
                float vitesse = muter(parent->getVitesseMax());
                float rayon = muter(parent->getRayon());
                float vision = muter(parent->getPorteeVue());

                nouveau.push_back(std::make_unique<Carnivore>(ent->getPosition() + sf::Vector2f(10.f, 10.f), vitesse, rayon, vision));
                naissancesCarnivores++;
            }
        }
    }

    // --- GESTION DES MORTS ET PARTICULES ---
    for (const auto& ent : entites) {
        if (!ent->estVivante()) {
            // Mise à jour des statistiques de mortalité
            switch(ent->getType()) {
                case TypeEntite::ALGUE: mortsAlgues++; break;
                case TypeEntite::BACTERIE: mortsBacteries++; break;
                case TypeEntite::HERBIVORE: mortsHerbivores++; break;
                case TypeEntite::CARNIVORE: mortsCarnivores++; break;
                default: break;
            }

            sf::Color couleurParticule = sf::Color::White;

            // Choix de la couleur d'explosion selon le type
            switch(ent->getType()) {
                case TypeEntite::ALGUE: couleurParticule = Constantes::ALGUE_COULEUR; break;
                case TypeEntite::BACTERIE: couleurParticule = Constantes::BACTERIE_COULEUR; break;
                case TypeEntite::HERBIVORE: {
                    Herbivore* h = static_cast<Herbivore*>(ent.get());
                    couleurParticule = h->estRapide() ? Constantes::HERBIVORE_COULEUR_RAPIDE : Constantes::HERBIVORE_COULEUR_LENT;
                    libererRocherProche(ent->getPosition());
                    break;
                }
                case TypeEntite::CARNIVORE: couleurParticule = Constantes::CARNIVORE_COULEUR; break;
                default: break;
            }

            // Création de l'effet visuel
            int nbParticules = static_cast<int>(ent->getRayon() * 1.5f);
            nbParticules = std::clamp(nbParticules, 5, 30);
            creerExplosion(ent->getPosition(), couleurParticule, nbParticules);
        }
    }

    // Mise à jour des particules
    for (auto it = particules.begin(); it != particules.end();) {
        it->update(dt);
        if (it->vie <= 0) {
            it = particules.erase(it);
        } else {
            ++it;
        }
    }

    // Suppression des entités mortes et ajout des nouvelles
    remove();
    for (auto& n : nouveau) entites.push_back(std::move(n));
}

/**
 * @brief Dessine l'ensemble du monde.
 * @param cible La fenêtre de rendu SFML.
 */
void Monde::dessiner(sf::RenderTarget& cible) const {
    for (const auto& e : entites) {
        if (e->estVivante()) e->dessiner(cible);
    }
    for (const auto& p : particules) {
        p.dessiner(cible);
    }
}

/**
 * @brief Fait apparaître une algue.
 * @param pos Position de spawn.
 */
void Monde::spawnAlgue(sf::Vector2f pos) {
    entites.emplace_back(std::make_unique<Algue>(pos));
    naissancesAlgues++;
}

/**
 * @brief Fait apparaître une bactérie.
 * @param pos Position de spawn.
 */
void Monde::spawnBacterie(sf::Vector2f pos) {
    entites.emplace_back(std::make_unique<Bacterie>(pos));
    naissancesBacteries++;
}

/**
 * @brief Fait apparaître un rocher.
 * @param pos Position de spawn.
 */
void Monde::spawnRocher(sf::Vector2f pos) {
    entites.emplace_back(std::make_unique<Rocher>(pos));
}

/**
 * @brief Gère les collisions d'une entité avec les rochers du monde.
 * * Repousse l'entité si elle pénètre dans un rocher et applique des dégâts.
 * * @param e L'entité à vérifier.
 */
void Monde::gererCollisionsRochers(Entite& e) {
    if (e.getType() == TypeEntite::ROCHER) return;

    for (auto& r : entites) {
        if (r->estVivante() && r->getType() == TypeEntite::ROCHER) {
            sf::Vector2f diff = e.getPosition() - r->getPosition();
            float dist = std::hypot(diff.x, diff.y);
            float distMin = e.getRayon() + r->getRayon();

            if (dist < distMin) {
                if (dist > 0.001f) {
                    sf::Vector2f direction = diff / dist;
                    // On peut ajouter une petite force de repousse ici si on veut
                }
                e.setEnergie(e.getEnergie() - Constantes::ROCHER_DEGATS);
            }
        }
    }
}

/**
 * @brief Vérifie si une position permet de se cacher derrière un rocher.
 * * Si un rocher libre est trouvé à proximité, il est marqué comme occupé.
 * * @param pos Position de l'entité qui cherche à se cacher.
 * @return true si une cachette a été trouvée, false sinon.
 */
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

/**
 * @brief Libère un rocher précédemment occupé.
 * * Appelé lorsqu'une entité cachée meurt ou quitte sa cachette.
 * * @param pos Position autour de laquelle chercher le rocher à libérer.
 */
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

/**
 * @brief Génère une explosion de particules colorées.
 * * @param pos Centre de l'explosion.
 * @param couleur Couleur de base des particules.
 * @param nb Nombre de particules à créer.
 */
void Monde::creerExplosion(sf::Vector2f pos, sf::Color couleur, int nb) {
    for (int i = 0; i < nb; ++i) {
        float angle = (rand() % 360) * 3.14159f / 180.f;
        float vitesse = static_cast<float>((rand() % 80) + 30);
        float variationCouleur = static_cast<float>((rand() % 40) - 20);
        
        sf::Color couleurVar = couleur;
        
        int r = std::clamp(static_cast<int>(couleurVar.r + variationCouleur), 0, 255);
        int g = std::clamp(static_cast<int>(couleurVar.g + variationCouleur), 0, 255);
        int b = std::clamp(static_cast<int>(couleurVar.b + variationCouleur), 0, 255);
        
        couleurVar.r = static_cast<std::uint8_t>(r);
        couleurVar.g = static_cast<std::uint8_t>(g);
        couleurVar.b = static_cast<std::uint8_t>(b);
        
        sf::Vector2f vit(std::cos(angle) * vitesse, std::sin(angle) * vitesse);
        particules.push_back(Particule(pos, vit, couleurVar));
    }
}

/**
 * @brief Dessine le fond de la simulation (dégradé).
 * @param cible La fenêtre de rendu.
 */
void Monde::dessinerFond(sf::RenderTarget& cible) const {
    sf::Vector2f taille = limites.size;
    sf::VertexArray degrade(sf::PrimitiveType::TriangleStrip, 4);
    degrade[0].position = {0, 0}; degrade[0].color = sf::Color(0, 105, 148);
    degrade[2].position = {taille.x, 0}; degrade[2].color = sf::Color(0, 105, 148);
    degrade[1].position = {0, taille.y}; degrade[1].color = sf::Color(2, 12, 27);
    degrade[3].position = {taille.x, taille.y}; degrade[3].color = sf::Color(2, 12, 27);
    cible.draw(degrade);
}

/**
 * @brief Réinitialise la simulation (supprime tout et remet les compteurs à zéro).
 */
void Monde::reset() {
    entites.clear();
    mortsAlgues = 0; mortsBacteries = 0; mortsHerbivores = 0; mortsCarnivores = 0;
    naissancesAlgues = 0; naissancesBacteries = 0; naissancesHerbivores = 0; naissancesCarnivores = 0;
}

/**
 * @brief Trouve l'entité la plus proche d'un certain type.
 * @param pos Position depuis laquelle chercher.
 * @param typeCherche Le type d'entité recherché (ex: CARNIVORE).
 * @return Pointeur vers l'entité trouvée, ou nullptr si aucune n'est trouvée.
 */
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

/**
 * @brief Supprime les entités mortes de la mémoire.
 */
void Monde::remove() {
    std::erase_if(entites, [](const auto& e) { return !e->estVivante(); });
}

/**
 * @brief Récupère les statistiques globales de la simulation.
 * @return Une structure Stats contenant les comptes de vivants, morts et naissances par espèce.
 */
Stats Monde::getStats() const {
    Stats s;
    s.algues.morts = mortsAlgues; s.algues.naissances = naissancesAlgues;
    s.bacteries.morts = mortsBacteries; s.bacteries.naissances = naissancesBacteries;
    s.herbivores.morts = mortsHerbivores; s.herbivores.naissances = naissancesHerbivores;
    s.carnivores.morts = mortsCarnivores; s.carnivores.naissances = naissancesCarnivores;
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

/**
 * @brief Récupère une entité située sous une position donnée (pour le clic souris).
 * @param pos Position de la souris.
 * @param rayon Rayon de tolérance pour la sélection.
 * @return Pointeur vers l'entité trouvée ou nullptr.
 */
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