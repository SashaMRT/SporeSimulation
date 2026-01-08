/**
 * @file carnivore.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Carnivore.
 * @details Gère le comportement des prédateurs : chasse active des herbivores,
 * gestion de l'énergie, déplacements et rendu visuel agressif.
 */

/**
 * @brief Constructeur de la classe Carnivore.
 * * Initialise un prédateur avec une position, une vitesse et une vision données.
 * La direction initiale est aléatoire.
 * * @param pos Position initiale (x, y).
 * @param vitesse Vitesse de déplacement maximale.
 * @param taille Rayon de l'entité.
 * @param vue Portée de détection des proies.
 */
Carnivore::Carnivore(sf::Vector2f pos, float vitesse, float taille, float vue) 
    : Entite(pos, TypeEntite::CARNIVORE, taille, Constantes::CARNIVORE_ENERGIE_BASE), 
    vitesseMax(vitesse),  porteeVue(vue)
{    
    // Calcul d'une direction de départ aléatoire
    float angle = (rand() % 360) * 3.14159f / 180.f;
    this->vitesse = sf::Vector2f(std::cos(angle), std::sin(angle)) * vitesseMax;
}

/**
 * @brief Met à jour l'état et le comportement du carnivore.
 * * Logique implémentée :
 * 1. Recherche l'herbivore le plus proche.
 * 2. Vérifie si la proie est visible (pas invisible) et à portée.
 * 3. Si oui, oriente sa vitesse vers la proie (Chasse).
 * 4. Si contact, mange la proie (gain d'énergie) et la tue.
 * 5. Gestion des collisions avec les murs (rebond).
 * 6. Calcul de la perte d'énergie (métabolisme).
 * * @param dt Temps écoulé depuis la dernière frame.
 * @param monde Référence au monde pour trouver les proies.
 */
void Carnivore::update(float dt, Monde& monde) {
    // Trouver la proie potentielle la plus proche
    Entite* proie = monde.getPlusProche(position, TypeEntite::HERBIVORE);

    if (proie) {
        // Casting pour accéder aux méthodes spécifiques de l'Herbivore (comme estInvisible)
        Herbivore* herbivore = dynamic_cast<Herbivore*>(proie);
        float dist = std::hypot(proie->getPosition().x - position.x, proie->getPosition().y - position.y);
        
        // Vérification des conditions de chasse
        // On ne chasse que si l'herbivore est visible et dans le rayon de vision
        if (herbivore && !herbivore->estInvisible() && dist < porteeVue) {
            
            // Poursuite
            sf::Vector2f direction = proie->getPosition() - position;
            if (dist > 0.001f) {
                // Normalisation du vecteur et application de la vitesse max
                vitesse = (direction / dist) * vitesseMax;
            }

            // Attaque / Manger
            if (dist < rayon + proie->getRayon()) {
                energie += 80.f; // Gain d'énergie important
                proie->tuer();   // La proie meurt instantanément
            }
        }
    }

    // Application du déplacement
    position += vitesse * dt;

    // Gestion des limites du monde (Rebond simple)
    sf::FloatRect limites = monde.getLimites();
    if (position.x < limites.position.x || position.x > limites.size.x) vitesse.x *= -1;
    if (position.y < limites.position.y || position.y > limites.size.y) vitesse.y *= -1;

    // Métabolisme
    // Les carnivores dépensent plus d'énergie s'ils sont gros ou rapides
    energie -= (rayon * 0.15f + vitesseMax * 0.01f) * dt;
    
    // Mort par famine
    if (energie <= 0) tuer();
}

/**
 * @brief Dessine le carnivore.
 * * Affiche une forme agressive (pointue) avec une queue animée.
 * * Apparence :
 * - Corps : Forme convexe allongée.
 * - Couleur : Nuances de rouge (plus foncé si gros).
 * - Indicateurs : Contour doré si très rapide (> 130), oeil rouge ou jaune.
 * * @param cible Fenêtre de rendu SFML.
 */
void Carnivore::dessiner(sf::RenderTarget& cible) const {
    // Calcul de l'angle de rotation en radians et degrés pour orienter le sprite
    float angleRad = 0.f;
    if (std::abs(vitesse.x) > 0.1f || std::abs(vitesse.y) > 0.1f) {
        angleRad = std::atan2(vitesse.y, vitesse.x);
    }
    float angleDeg = angleRad * 180.f / 3.14159f;

    // Couleur de peau basée sur la taille (plus sombre si plus gros)
    std::uint8_t rouge = static_cast<std::uint8_t>(std::clamp(255.f - (rayon * 2.f), 100.f, 255.f));
    sf::Color couleurPeau(rouge, 40, 40);
    
    // Les prédateurs d'élite (rapides) ont un contour doré, les autres rouge sombre
    sf::Color couleurContour = (vitesseMax > 130.f) ? sf::Color(255, 215, 0) : sf::Color(150, 20, 20);

    // --- Dessin de la queue (3 segments) ---
    for (int i = 3; i >= 1; --i) {
        float tailleQueue = rayon * (0.7f - i * 0.1f);
        
        // Forme triangulaire pour chaque segment de queue
        sf::ConvexShape queue(3);
        queue.setPoint(0, {tailleQueue * 1.5f, 0});
        queue.setPoint(1, {-tailleQueue, -tailleQueue * 0.7f});
        queue.setPoint(2, {-tailleQueue, tailleQueue * 0.7f});

        // Animation d'ondulation basée sur la position (effet de nage)
        float oscillation = std::sin((position.x + position.y) * 0.1f + i) * 15.f;
        
        // Positionnement des segments derrière le corps
        sf::Vector2f posQueue;
        posQueue.x = position.x - std::cos(angleRad) * (i * rayon * 0.7f);
        posQueue.y = position.y - std::sin(angleRad) * (i * rayon * 0.7f);

        queue.setPosition(posQueue);
        queue.setRotation(sf::degrees(angleDeg + oscillation));
        queue.setFillColor(sf::Color(rouge - 20, 30, 30));
        cible.draw(queue);
    }

    // --- Dessin du corps (Forme de diamant/flèche) ---
    sf::ConvexShape corps(4);
    corps.setPoint(0, {rayon * 1.8f, 0});  // Nez pointu
    corps.setPoint(1, {0, -rayon});        // Flanc gauche
    corps.setPoint(2, {-rayon * 0.8f, 0}); // Arrière
    corps.setPoint(3, {0, rayon});         // Flanc droit
    
    corps.setPosition(position);
    corps.setRotation(sf::degrees(angleDeg));
    corps.setFillColor(couleurPeau);
    corps.setOutlineThickness(2.f);
    corps.setOutlineColor(couleurContour);
    cible.draw(corps);

    // --- Dessin de l'œil ---
    sf::CircleShape oeil(rayon * 0.25f);
    oeil.setOrigin({oeil.getRadius(), oeil.getRadius()});
    
    // Position de l'œil décalée vers l'avant
    sf::Vector2f posOeil;
    posOeil.x = position.x + std::cos(angleRad) * (rayon * 0.5f);
    posOeil.y = position.y + std::sin(angleRad) * (rayon * 0.5f);
    
    oeil.setPosition(posOeil);
    // Œil rouge pour les élites, jaune pour les normaux
    oeil.setFillColor((vitesseMax > 130.f) ? sf::Color::Red : sf::Color::Yellow);
    cible.draw(oeil);
}