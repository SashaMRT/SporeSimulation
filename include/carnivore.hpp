#pragma once

/**
 * @class Carnivore
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Représente un prédateur au sommet de la chaîne alimentaire.
 * @details Les carnivores chassent activement les herbivores pour survivre.
 * Ils possèdent une vitesse et une vision généralement supérieures, mais
 * un métabolisme plus coûteux qui les oblige à manger régulièrement.
 */
class Carnivore : public Entite {
public:
    /**
     * @brief Constructeur de la classe Carnivore.
     * @param pos Position initiale.
     * @param vitesse Vitesse maximale de déplacement (trait génétique).
     * @param taille Rayon physique de l'entité.
     * @param vue Portée de détection des proies (trait génétique).
     */
    Carnivore(sf::Vector2f pos, float vitesse, float taille, float vue);
    
    /**
     * @brief Met à jour le comportement du carnivore.
     * @details Gère la recherche de proies, la poursuite, l'attaque
     * et la consommation d'énergie.
     * @param dt Temps écoulé (Delta Time).
     * @param monde Référence au monde pour localiser les proies.
     */
    void update(float dt, Monde& monde) override;

    /**
     * @brief Dessine le carnivore.
     * @details Affiche une forme distinctive (souvent plus agressive ou pointue)
     * pour le différencier des herbivores.
     * @param cible Fenêtre de rendu.
     */
    void dessiner(sf::RenderTarget& cible) const override;

    /**
     * @brief Récupère la vitesse maximale.
     * @return Vitesse en pixels/seconde.
     */
    float getVitesseMax() const { return vitesseMax; }

    /**
     * @brief Récupère la portée de vue.
     * @return Rayon de détection en pixels.
     */
    float getPorteeVue() const { return porteeVue; }

private:
    float vitesseMax;   /**< Vitesse maximale de déplacement. */
    float porteeVue;    /**< Distance de vision pour détecter les herbivores. */
};