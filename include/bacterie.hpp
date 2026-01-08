#pragma once

/**
 * @class Bacterie
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Représente la forme de vie la plus simple de la simulation.
 * @details Les bactéries sont les premières entités à apparaître. 
 * Elles se nourrissent exclusivement d'algues et servent de point de départ 
 * à l'évolution vers des formes de vie complexes (Herbivores/Carnivores).
 */
class Bacterie : public Entite {
public:
    /**
     * @brief Constructeur de la classe Bacterie.
     * @param pos Position initiale dans le monde.
     */
    Bacterie(sf::Vector2f pos);

    /**
     * @brief Met à jour le comportement de la bactérie.
     * @details Recherche l'algue la plus proche, se déplace vers elle 
     * et la consomme. Gère également la mort par manque d'énergie.
     * @param dt Temps écoulé (Delta Time).
     * @param monde Référence au monde pour interagir avec les algues.
     */
    void update(float dt, Monde& monde) override;

    /**
     * @brief Dessine la bactérie.
     * @details Affiche une forme cellulaire avec un noyau et des cils vibratiles.
     * @param cible Fenêtre de rendu.
     */
    void dessiner(sf::RenderTarget& cible) const override;

private:
    float vitesseMax; /**< Vitesse de déplacement de la bactérie. */

    /**
     * @brief Gère l'évolution de la bactérie.
     * @details Appelée lorsque la bactérie a accumulé suffisamment d'énergie 
     * pour se transformer en une entité supérieure.
     * @param monde Référence au monde pour faire apparaître la nouvelle entité.
     */
    void evoluer(Monde& monde);
};