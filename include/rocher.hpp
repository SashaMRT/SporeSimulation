#pragma once

/**
 * @class Rocher
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Représente un obstacle rocheux dans le monde.
 * @details Les rochers sont des entités statiques qui servent d'obstacles physiques
 * et de points de cachette pour les herbivores fuyant les prédateurs.
 * Elle hérite de la classe de base Entite.
 */
class Rocher : public Entite {
public:
    /**
     * @brief Constructeur de la classe Rocher.
     * @param pos La position centrale du rocher dans le monde (x, y).
     */
    Rocher(sf::Vector2f pos);
    
    /**
     * @brief Met à jour la logique du rocher.
     * @details Bien que statique, cette méthode est requise par l'héritage.
     * Elle ne fait généralement rien pour un rocher inerte.
     * @param dt Temps écoulé depuis la dernière frame (Delta Time).
     * @param monde Référence vers le monde contenant l'entité.
     */
    void update(float dt, Monde& monde) override;

    /**
     * @brief Dessine le rocher sur la cible de rendu.
     * @param cible La fenêtre ou texture où dessiner l'entité.
     */
    void dessiner(sf::RenderTarget& cible) const override;

    /**
     * @brief Vérifie si le rocher est actuellement utilisé comme cachette.
     * @return true si un herbivore est caché derrière ce rocher, false sinon.
     */
    bool estOccupe() const { return occupe; }

    /**
     * @brief Modifie l'état d'occupation du rocher.
     * @details Appelé lorsqu'un herbivore se cache (true) ou quitte la cachette (false).
     * Cela change souvent l'apparence visuelle du rocher pour le joueur.
     * @param etat Nouvel état d'occupation.
     */
    void setOccupe(bool etat) { occupe = etat; }

private:
    bool occupe = false; /**< Indique si une entité est actuellement cachée ici. */
};