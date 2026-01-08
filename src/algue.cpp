/**
 * @file algue.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Algue.
 * @details Représente la ressource de base de la chaîne alimentaire.
 * Les algues sont des entités statiques mais possèdent une animation visuelle de flottement.
 */

/**
 * @brief Constructeur de la classe Algue.
 * * Initialise une algue à une position donnée avec les propriétés définies dans les constantes.
 * * @param pos Position (x, y) dans le monde.
 */
Algue::Algue(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::ALGUE, Constantes::ALGUE_RAYON, Constantes::ALGUE_ENERGIE) {
}

/**
 * @brief Met à jour l'état de l'algue.
 * * @details Les algues sont des entités passives. Elles ne se déplacent pas
 * et n'ont pas de logique comportementale propre (elles attendent juste d'être mangées).
 * * @param dt Temps écoulé (non utilisé ici).
 * @param monde Référence au monde (non utilisé ici).
 */
void Algue::update(float dt, Monde& monde) {
    // L'algue est statique, aucune logique de mise à jour nécessaire.
}

/**
 * @brief Dessine l'algue avec une animation procédurale.
 * * Simule une plante aquatique qui ondule sous l'effet du courant
 * en dessinant plusieurs segments superposés qui oscillent.
 * * @param cible Fenêtre de rendu SFML.
 */
void Algue::dessiner(sf::RenderTarget& cible) const {
    // Utilisation d'une horloge statique pour que toutes les algues ondulent de manière synchronisée
    // sans avoir besoin de stocker un temps individuel pour chacune.
    static sf::Clock horloge;
    float temps = horloge.getElapsedTime().asSeconds();
    
    // Paramètres graphiques pour la structure de la plante
    const int nbSegments = 10;
    const float epaisseurBase = 4.f;
    const float hauteurSegment = 4.f;
    
    // Boucle pour construire la tige du bas vers le haut
    for (int i = 0; i < nbSegments; ++i) {
        // Ratio de progression (0.0 à la base, ~1.0 au sommet)
        float ratio = static_cast<float>(i) / nbSegments;
        
        // La tige s'affine vers le haut
        float taille = epaisseurBase * (1.0f - ratio * 0.7f);
        
        // Calcul de l'ondulation : fonction sinus basée sur le temps et l'indice du segment
        // Plus on monte (i grand), plus l'oscillation est forte (effet de levier)
        float oscillation = std::sin(temps * 1.5f + i * 0.5f) * (i * 0.6f);
        
        sf::CircleShape segment(taille);
        segment.setOrigin({taille, taille});
        
        // Dégradé de couleur : de vert foncé à la base vers vert clair/transparent au sommet
        segment.setFillColor(sf::Color(34 + i*5, 120 + i*8, 50 + i*2, 255 - i*15));
        
        // Positionnement : on empile les cercles vers le haut (-y) avec le décalage X (oscillation)
        segment.setPosition(position + sf::Vector2f(oscillation, -i * hauteurSegment));
        
        cible.draw(segment);
    }
}