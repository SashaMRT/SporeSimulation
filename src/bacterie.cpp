/**
 * @file bacterie.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Bacterie.
 * @details Gère le comportement de la forme de vie la plus simple de la simulation :
 * recherche de nourriture (algues), déplacement simple et rendu visuel cellulaire.
 */

/**
 * @brief Constructeur de la classe Bacterie.
 * * Initialise une bactérie à une position donnée avec les statistiques de base définies dans les constantes.
 * * @param pos Position initiale (x, y) dans le monde.
 */
Bacterie::Bacterie(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::BACTERIE, Constantes::BACTERIE_RAYON, Constantes::BACTERIE_ENERGIE) {
}

/**
 * @brief Met à jour l'état de la bactérie.
 * * Logique implémentée :
 * 1. Recherche l'algue la plus proche.
 * 2. Se déplace vers elle en ligne droite.
 * 3. Consomme l'algue au contact pour gagner de l'énergie (nécessaire à l'évolution).
 * 4. Perd de l'énergie avec le temps (métabolisme de base).
 * * @param dt Temps écoulé depuis la dernière frame.
 * @param monde Référence au monde pour interagir avec les entités.
 */
void Bacterie::update(float dt, Monde& monde) {
    // Recherche de la nourriture la plus proche
    Entite* cible = monde.getPlusProche(position, TypeEntite::ALGUE);
    
    if (cible) {
        // Calcul du vecteur direction vers la cible
        sf::Vector2f diff = cible->getPosition() - position;
        float dist = std::hypot(diff.x, diff.y);
        
        // Déplacement si on n'est pas déjà superposé
        if (dist > 0.001f) {
            sf::Vector2f dir = diff / dist; // Normalisation
            position += dir * 40.f * dt;    // Vitesse fixe de 40.f
        }

        // Gestion de la collision (Manger)
        if (dist < rayon + cible->getRayon()) {
            // CORRECTION : On utilise la constante (50) au lieu de 15
            energie += Constantes::ALGUE_ENERGIE; 
            cible->tuer(); // L'algue disparaît
        }
    }
    
    // Métabolisme : perte constante d'énergie
    energie -= 5.f * dt;
    
    // Mort si plus d'énergie
    if (energie <= 0) tuer();
}

/**
 * @brief Dessine la bactérie.
 * * Représentation visuelle complexe imitant une cellule :
 * - Une membrane semi-transparente.
 * - Des cils vibratiles (pattes) tout autour.
 * - Un noyau coloré avec un reflet.
 * * @param cible Fenêtre de rendu SFML.
 */
void Bacterie::dessiner(sf::RenderTarget& cible) const {
    // Dessin de la membrane cellulaire (Cyan transparent)
    sf::CircleShape membrane(rayon);
    membrane.setOrigin({rayon, rayon});
    membrane.setPosition(position);
    membrane.setFillColor(sf::Color(0, 255, 255, 40));
    membrane.setOutlineThickness(2.f);
    membrane.setOutlineColor(sf::Color(255, 255, 255, 150));
    cible.draw(membrane);

    // Dessin des cils/pattes (petits cercles autour de la membrane)
    sf::CircleShape patte(rayon * 0.15f);
    patte.setFillColor(sf::Color(200, 255, 255, 180));
    patte.setOrigin({rayon * 0.15f, rayon * 0.15f});
    
    // Boucle pour placer 8 cils à intervalles réguliers (45 degrés)
    for (int i = 0; i < 8; ++i) {
        float a = i * 45.f * 3.14159f / 180.f; // Conversion degrés -> radians
        patte.setPosition(position + sf::Vector2f(std::cos(a), std::sin(a)) * rayon);
        cible.draw(patte);
    }

    // Dessin du noyau principal (Bleu)
    sf::CircleShape noyauexterne(rayon);
    noyauexterne.setOutlineThickness(0);
    noyauexterne.setRadius(rayon * 0.4f);
    noyauexterne.setOrigin({rayon * 0.4f, rayon * 0.4f});
    noyauexterne.setFillColor(sf::Color(0, 180, 255));
    cible.draw(noyauexterne);

    // Dessin du reflet sur le noyau (Blanc, décalé vers le haut-gauche)
    sf::CircleShape noyauinterne(rayon);
    noyauinterne.setRadius(rayon * 0.2f);
    noyauinterne.setOrigin({rayon * 0.2f, rayon * 0.2f});
    noyauinterne.setFillColor(sf::Color(255, 255, 255, 200));
    noyauinterne.setPosition(position - sf::Vector2f(rayon * 0.1f, rayon * 0.1f));
    cible.draw(noyauinterne);
}