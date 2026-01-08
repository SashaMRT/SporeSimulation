#pragma once

/**
 * @class Particule
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Représente une particule visuelle éphémère.
 * @details Les particules sont utilisées pour créer des effets visuels (explosions,
 * trainées, impacts). Elles n'interagissent pas physiquement avec le monde
 * et disparaissent une fois leur durée de vie écoulée.
 */
class Particule {
public:
    sf::Vector2f position;  /**< Position actuelle (x, y). */
    sf::Vector2f vitesse;   /**< Vecteur de déplacement. */
    sf::Color couleur;      /**< Couleur de base de la particule. */
    float vie;              /**< Durée de vie restante (de 1.0 à 0.0). */
    float tailleInitiale;   /**< Taille de départ (pour le calcul de l'échelle). */
    float angle;            /**< Angle de rotation actuel en radians. */
    float rotation;         /**< Vitesse de rotation angulaire. */

    /**
     * @brief Constructeur de la particule.
     * * Initialise une particule avec une position, une vitesse et une couleur données.
     * Génère aléatoirement la taille, l'angle initial et la vitesse de rotation
     * pour apporter de la variété visuelle.
     * * @param pos Position de départ.
     * @param vit Vecteur vitesse initial.
     * @param col Couleur de la particule.
     */
    Particule(sf::Vector2f pos, sf::Vector2f vit, sf::Color col)
        : position(pos), vitesse(vit), couleur(col), vie(1.0f),
          // Taille aléatoire entre 2.0 et 5.0 pixels
          tailleInitiale(2.f + (rand() % 100) / 100.f * 3.f),
          // Angle initial aléatoire (0 à 2*PI)
          angle((rand() % 360) * 3.14159f / 180.f),
          // Vitesse de rotation aléatoire (-90 à +90 degrés/sec)
          rotation((rand() % 100) / 100.f * 180.f - 90.f) {}

    /**
     * @brief Met à jour la physique de la particule.
     * * Applique le mouvement, le ralentissement (friction) et le vieillissement.
     * * @param dt Temps écoulé depuis la dernière frame.
     */
    void update(float dt) {
        position += vitesse * dt;               // Déplacement
        vitesse *= Constantes::PARTICULE_FRICTION; // Ralentissement progressif (inertie)
        angle += rotation * dt;                 // Rotation sur elle-même
        vie -= dt * Constantes::PARTICULE_VIE_DECAY; // Réduction de la vie
    }

    /**
     * @brief Dessine la particule.
     * * Affiche un carré dont la taille et l'opacité diminuent avec le temps
     * pour simuler la dissipation.
     * * @param cible Fenêtre de rendu SFML.
     */
    void dessiner(sf::RenderTarget& cible) const {
        if (vie <= 0) return; // Ne rien dessiner si morte
        
        // La taille diminue légèrement à mesure que la particule meurt
        float taille = tailleInitiale * (0.5f + vie * 0.5f);
        
        sf::RectangleShape forme({taille, taille});
        forme.setOrigin({taille / 2.f, taille / 2.f}); // Centre de rotation au milieu
        forme.setPosition(position);
        forme.setRotation(sf::degrees(angle));
        
        // Gestion de la transparence (Alpha)
        sf::Color c = couleur;
        // L'alpha diminue proportionnellement à la vie restante (Fade out)
        c.a = static_cast<std::uint8_t>(std::max(0.f, vie) * 200);
        
        forme.setFillColor(c);
        cible.draw(forme);
    }
};