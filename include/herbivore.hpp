#pragma once

/**
 * @class Herbivore
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Représente une entité se nourrissant d'algues (proie).
 * @details Les herbivores possèdent des comportements complexes :
 * - Ils cherchent activement de la nourriture (algues).
 * - Ils fuient les prédateurs (Carnivores).
 * - Ils peuvent se camoufler (devenir invisibles) s'ils sont près d'un rocher.
 * - Ils transmettent leurs gènes (vitesse, vision) à leur descendance.
 */
class Herbivore : public Entite {
public:
    /**
     * @brief Constructeur de la classe Herbivore.
     * @param pos Position initiale dans le monde.
     * @param vitesse Vitesse maximale (trait génétique).
     * @param taille Rayon de l'entité (trait génétique).
     * @param vue Portée de détection (trait génétique).
     */
    Herbivore(sf::Vector2f pos, float vitesse, float taille, float vue);
    
    /**
     * @brief Met à jour l'IA de l'herbivore.
     * @details Gère la faim, la peur, la fuite et le camouflage.
     * @param dt Temps écoulé (Delta Time).
     * @param monde Référence au monde pour l'interaction avec l'environnement.
     */
    void update(float dt, Monde& monde) override;

    /**
     * @brief Dessine l'herbivore.
     * @details Affiche le corps et les tentacules si l'entité est visible.
     * @param cible Fenêtre de rendu.
     */
    void dessiner(sf::RenderTarget& cible) const override;

    /**
     * @brief Vérifie si l'herbivore est camouflé.
     * @return true si invisible (caché), false sinon.
     */
    bool estInvisible() const {return invisible;}

    /**
     * @brief Récupère la vitesse maximale génétique.
     * @return Vitesse en pixels/seconde.
     */
    float getVitesseMax() const { return vitesseMax; }

    /**
     * @brief Récupère la portée de vue.
     * @return Rayon de détection en pixels.
     */
    float getPorteeVue() const { return porteeVue; }

    /**
     * @brief Indique si l'herbivore est considéré comme "rapide" (trait visuel).
     * @return true si la vitesse dépasse un certain seuil.
     */
    bool estRapide() const { return isRapide; }

private:
    float vitesseMax;   /**< Vitesse de déplacement maximale. */
    float porteeVue;    /**< Distance à laquelle il voit nourriture et dangers. */

    bool invisible;       /**< État actuel du camouflage. */
    float chronoCache;    /**< Temps écoulé depuis le début du camouflage. */
    float chronoRecharge; /**< Temps restant avant de pouvoir se recacher (Cooldown). */
    bool isRapide;        /**< Cache pour déterminer l'apparence (nombre de tentacules). */
};