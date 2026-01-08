#pragma once

/**
 * @class Algue
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Représente une ressource alimentaire immobile (Producteur primaire).
 * @details Les algues apparaissent aléatoirement dans le monde et servent de
 * nourriture de base pour les bactéries et certains herbivores.
 * Elles sont statiques mais possèdent une animation visuelle.
 */
class Algue : public Entite {
public:
    /**
     * @brief Constructeur de la classe Algue.
     * @param pos Position initiale dans le monde.
     */
    Algue(sf::Vector2f pos);
    
    /**
     * @brief Met à jour la logique de l'algue.
     * @details Actuellement vide car l'algue est une entité passive qui ne vieillit pas
     * et ne se déplace pas.
     * @param dt Temps écoulé (Delta Time).
     * @param monde Référence au monde.
     */
    void update(float dt, Monde& monde) override;

    /**
     * @brief Dessine l'algue.
     * @details Affiche une tige verte composée de plusieurs segments
     * qui ondulent pour simuler un mouvement aquatique.
     * @param cible Fenêtre de rendu.
     */
    void dessiner(sf::RenderTarget& cible) const override;
};