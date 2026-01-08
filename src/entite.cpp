/**
 * @file entite.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe abstraite Entite.
 * @details Gère les mécanismes de base communs à toutes les entités :
 * identifiant unique, position, type, et cycle de vie.
 */

// Initialisation du compteur statique d'identifiants
// Ce compteur est partagé par toutes les instances d'Entite
int Entite::compteurId = 0;

/**
 * @brief Constructeur de la classe de base Entite.
 * * Initialise les attributs communs et attribue un ID unique.
 * * @param pos Position initiale (x, y) dans le monde.
 * @param type Type de l'entité (ALGUE, BACTERIE, etc.) pour le polymorphisme.
 * @param rayon Rayon physique pour les collisions et le rendu.
 * @param energie Énergie initiale (points de vie).
 */
Entite::Entite(sf::Vector2f pos, TypeEntite type, float rayon, float energie)
    : position(pos), type(type), rayon(rayon), energie(energie), vivante(true) {    
    
    // Incrémentation atomique (logique) du compteur pour garantir un ID unique
    // à chaque nouvelle entité créée.
    id = ++compteurId; 
}