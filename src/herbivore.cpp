/**
 * @file herbivore.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Herbivore.
 * @details Gère l'intelligence artificielle des herbivores : fuite, recherche de nourriture,
 * mécanisme de camouflage (invisibilité) et rendu graphique.
 */

/**
 * @brief Constructeur de la classe Herbivore.
 * * Initialise un herbivore avec une direction aléatoire et ses paramètres vitaux.
 * * @param pos Position de départ.
 * @param vitesse Vitesse de déplacement maximale.
 * @param taille Rayon de l'entité.
 * @param vue Portée de la vision.
 */
Herbivore::Herbivore(sf::Vector2f pos, float vitesse, float taille, float vue) 
    : Entite(pos, TypeEntite::HERBIVORE, taille, Constantes::HERBIVORE_ENERGIE_BASE),
      vitesseMax(vitesse), porteeVue(vue), invisible(false), 
      chronoCache(0.f), chronoRecharge(0.f), isRapide(vitesse > 120.f) 
{    
    // Calcul d'un angle de départ aléatoire
    float angle = (rand() % 360) * 3.14159f / 180.f;
    this->vitesse = sf::Vector2f(std::cos(angle), std::sin(angle)) * vitesseMax;
}

/**
 * @brief Met à jour l'état et le comportement de l'herbivore.
 * * Logique :
 * 1. Gestion du camouflage (temps de recharge et durée).
 * 2. Détection du danger (Carnivores) et fuite.
 * 3. Comportement de troupeau (vision augmentée si proche d'un ami).
 * 4. Recherche et consommation d'algues.
 * 5. Gestion des collisions avec les bords et métabolisme.
 * * @param dt Temps écoulé depuis la dernière frame.
 * @param monde Référence au monde pour interagir avec les autres entités.
 */
void Herbivore::update(float dt, Monde& monde) {
    // Diminution du temps de recharge de la compétence "se cacher"
    if (chronoRecharge > 0.f) chronoRecharge -= dt;

    // --- Gestion de l'invisibilité ---
    if (invisible) {
        chronoCache += dt;
        // Si caché depuis 10 secondes, on redevient visible
        if (chronoCache >= 10.f) {
            invisible = false;
            chronoCache = 0.f;
            chronoRecharge = 20.f;
            monde.libererRocherProche(position); // Libère le rocher pour les autres
        }
        return; // L'entité ne fait rien d'autre tant qu'elle est cachée
    }

    // --- Analyse du danger ---
    Entite* danger = monde.getPlusProche(position, TypeEntite::CARNIVORE);
    bool dangerProche = false;
    
    if (danger) {
        float distDanger = std::hypot(danger->getPosition().x - position.x, 
                                      danger->getPosition().y - position.y);
        // Le danger est considéré proche s'il est à moins de 70% de la portée de vue
        dangerProche = (distDanger < porteeVue * 0.7f);
    }

    // Activation du camouflage si danger, compétence prête et rocher à proximité
    if (chronoRecharge <= 0.f && dangerProche && monde.estCache(position)) {
        invisible = true;
        return;
    }

    // --- Comportement de troupeau ---
    Entite* ami = monde.getPlusProche(position, TypeEntite::HERBIVORE);
    float vueEffective = porteeVue;
    
    // Si un ami est proche, la vigilance (vision) est doublée
    if (ami) {
        float distAmi = std::hypot(ami->getPosition().x - position.x, ami->getPosition().y - position.y);
        if (distAmi > 0.1f && distAmi < 100.f) {
            vueEffective *= 2.0f;
        }
    }

    Entite* nourriture = monde.getPlusProche(position, TypeEntite::ALGUE);

    bool enFuite = false;

    // --- Logique de Fuite ---
    if (danger && dangerProche) {
        sf::Vector2f fuite = position - danger->getPosition();
        float distDanger = std::hypot(fuite.x, fuite.y);

        if (distDanger < vueEffective) {
            if (distDanger > 0.001f) {
                // Algorithme de fuite "intelligente" pour éviter les coins
                sf::FloatRect limites = monde.getLimites();
                sf::Vector2f centreMonde = {limites.size.x / 2.f, limites.size.y / 2.f};
                sf::Vector2f versCentre = centreMonde - position;
                float distVersCentre = std::hypot(versCentre.x, versCentre.y);
                
                if (distVersCentre > 0.001f) {
                    versCentre /= distVersCentre;
                    
                    float angleFuite = std::atan2(fuite.y, fuite.x);
                    float angleVersCentre = std::atan2(versCentre.y, versCentre.x);
                    
                    // Calcul de la différence d'angle normalisée
                    float differenceAngle = angleVersCentre - angleFuite;
                    while (differenceAngle > 3.14159f) differenceAngle -= 2 * 3.14159f;
                    while (differenceAngle < -3.14159f) differenceAngle += 2 * 3.14159f;
                    
                    // Pondération de la direction de fuite vers le centre (30%)
                    float poidsVersCentre = 0.3f;
                    float angleFinal = angleFuite + differenceAngle * poidsVersCentre;
                    
                    vitesse.x = std::cos(angleFinal);
                    vitesse.y = std::sin(angleFinal);
                    vitesse = vitesse * (vitesseMax * 1.2f); // Boost de vitesse
                } else {
                    // Fuite simple à l'opposé du danger
                    vitesse = (fuite / distDanger) * (vitesseMax * 1.2f);
                }
            } else {
                // CORRECTION ICI : Si on est pile sur le danger, on choisit une direction au hasard
                float angleHasard = (rand() % 360) * 3.14159f / 180.f;
                vitesse = sf::Vector2f(std::cos(angleHasard), std::sin(angleHasard)) * (vitesseMax * 1.2f);
            }
            enFuite = true;
            energie -= 5.f * dt; // Coût énergétique du stress
        }
    }

    // --- Recherche de nourriture ---
    if (!enFuite && nourriture) {
        sf::Vector2f versNourriture = nourriture->getPosition() - position;
        float distNourriture = std::hypot(versNourriture.x, versNourriture.y);
        
        if (distNourriture < vueEffective) {
            // Déplacement vers l'algue
            if (distNourriture > 0.001f) {
                vitesse = (versNourriture / distNourriture) * vitesseMax;
            }
            
            // Consommation si assez proche
            if (distNourriture < rayon + nourriture->getRayon()) {
                nourriture->tuer();
                energie += 40.f;
            }
        }
    }

    position += vitesse * dt;

    // --- Gestion des collisions avec les limites du monde ---
    sf::FloatRect limites = monde.getLimites();
    if (position.x < limites.position.x) { position.x = limites.position.x; vitesse.x = std::abs(vitesse.x); }
    if (position.x > limites.size.x) { position.x = limites.size.x; vitesse.x = -std::abs(vitesse.x); }
    if (position.y < limites.position.y) { position.y = limites.position.y; vitesse.y = std::abs(vitesse.y); }
    if (position.y > limites.size.y) { position.y = limites.size.y; vitesse.y = -std::abs(vitesse.y); }

    // Dépense énergétique métabolique
    float coutMetabolique = (rayon * 0.1f) + (vitesseMax * 0.02f);
    energie -= coutMetabolique * dt;
    
    // Mort par épuisement
    if (energie <= 0) 
        tuer();
}

/**
 * @brief Dessine l'herbivore.
 * * Affiche le corps, les tentacules animés et les yeux orientés selon le mouvement.
 * * @param cible Fenêtre de rendu.
 */
void Herbivore::dessiner(sf::RenderTarget& cible) const {
    if (invisible) return; // Ne rien dessiner si caché

    // Calcul de l'angle pour orienter les yeux
    float angle = 0.f;
    if (std::abs(vitesse.x) > 0.1f || std::abs(vitesse.y) > 0.1f) {
        angle = std::atan2(vitesse.y, vitesse.x);
    }

    // Couleur basée sur la vitesse (génétique)
    std::uint8_t bleu = static_cast<std::uint8_t>(std::clamp(vitesseMax + 50.f, 0.f, 255.f));
    sf::Color couleurCorps(50, 200, bleu, 200);
    sf::Color couleurTentacules(50, 220, 150, 150);

    // Nombre de tentacules selon la vitesse
    int nbTentacules = (vitesseMax > 120.f) ? 3 : 5;

    // Dessin des tentacules
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
            t.setFillColor(couleurTentacules);
            cible.draw(t);
        }
    }

    // Dessin du corps
    sf::CircleShape corps(rayon);
    corps.setOrigin({rayon, rayon});
    corps.setPosition(position);
    corps.setFillColor(couleurCorps);
    corps.setOutlineThickness(2.f);
    corps.setOutlineColor(sf::Color::White);
    cible.draw(corps);

    // Dessin des yeux si l'entité bouge
    if (std::abs(vitesse.x) > 0.1f || std::abs(vitesse.y) > 0.1f) {
        sf::Vector2f posOeil = position + sf::Vector2f(std::cos(angle), std::sin(angle)) * (rayon * 0.6f);
        
        sf::CircleShape oeil(rayon * 0.3f);
        oeil.setOrigin({rayon * 0.3f, rayon * 0.3f});
        oeil.setPosition(posOeil);
        oeil.setFillColor(sf::Color::White);
        cible.draw(oeil);

        sf::CircleShape pupille(rayon * 0.15f);
        pupille.setOrigin({pupille.getRadius(), pupille.getRadius()});
        pupille.setPosition(posOeil + sf::Vector2f(std::cos(angle), std::sin(angle)) * (rayon * 0.15f));
        pupille.setFillColor(sf::Color::Black);
        cible.draw(pupille);
    }
}