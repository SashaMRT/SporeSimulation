#pragma once

/**
 * @class Rendu
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Gère l'affichage de l'interface utilisateur (HUD) et des menus.
 * @details Cette classe est responsable de tous les éléments graphiques
 * qui ne font pas partie de la simulation elle-même : panneau latéral,
 * statistiques, textes d'information, jauges, etc.
 */
class Rendu {
public:
    /**
     * @brief Constructeur de la classe Rendu.
     * Initialise les éléments graphiques de base (formes, couleurs).
     */
    Rendu();

    /**
     * @brief Charge les ressources nécessaires (polices) et configure les textes.
     * @param cheminFont Chemin vers le fichier de police (.ttf).
     * @return true si le chargement a réussi, false sinon.
     */
    bool init(const std::string& cheminFont);

    /**
     * @brief Affiche le menu complet et les informations de simulation.
     * @details Cette méthode est appelée à chaque frame pour dessiner l'interface
     * par-dessus le rendu du monde.
     * @param monde Référence au monde pour accéder aux statistiques.
     * @param cible Fenêtre de rendu SFML.
     * @param enPause État de la simulation (pour afficher le texte "PAUSE").
     * @param fps Nombre d'images par seconde actuel.
     * @param tempsEcoule Temps total simulé.
     * @param survol Pointeur vers l'entité sous la souris (optionnel, pour l'inspection).
     */
    void menu(const Monde& monde, sf::RenderTarget& cible, bool enPause, float fps, float tempsEcoule, const Entite* survol = nullptr);

private:
    sf::Font font;              /**< Police d'écriture utilisée pour tous les textes. */
    sf::RectangleShape fond;    /**< Fond du panneau latéral droit. */
    sf::RectangleShape separateur; /**< Ligne de séparation décorative. */
    sf::Text titre;             /**< Titre principal "SPORE SIM". */

    // --- Textes d'information et d'état ---
    sf::Text inspection;        /**< Titre de la section d'inspection d'entité. */
    sf::Text details;           /**< Détails de l'entité inspectée (ID, Energie, etc.). */
    sf::Text pause;             /**< Indicateur visuel "PAUSE". */
    sf::Text description;       /**< Texte explicatif du projet. */
    sf::Text controles;         /**< Titre de la section des contrôles. */
    sf::Text controlesGauche;   /**< Liste des contrôles souris (colonne gauche). */
    sf::Text controlesDroite;   /**< Liste des contrôles clavier (colonne droite). */
    sf::Text alerte;            /**< Message d'avertissement si la fenêtre est trop petite. */
    
    // --- Informations techniques ---
    sf::Text infoFps;           /**< Affichage des FPS. */
    sf::Text infoTemps;         /**< Affichage du temps écoulé (MM:SS). */

    // --- Éléments graphiques pour les statistiques ---
    // Ces membres sont réutilisés pour dessiner chaque ligne de stat afin d'optimiser la mémoire
    sf::CircleShape iconeStat;      /**< Pastille de couleur de l'espèce. */
    sf::Text nomStat;               /**< Nom de l'espèce affichée. */
    sf::Text valeurStat;            /**< Chiffres (Vivants/Morts/Naissances). */
    sf::RectangleShape fondJauge;   /**< Arrière-plan de la barre de population. */
    sf::RectangleShape jauge;       /**< Barre de progression représentant la population. */

    /**
     * @brief Méthode utilitaire interne pour dessiner une ligne de statistiques complète.
     * @param cible Fenêtre de rendu.
     * @param nomEspece Nom à afficher.
     * @param stats Données chiffrées de l'espèce.
     * @param couleur Couleur associée à l'espèce.
     * @param x Position X de l'élément.
     * @param y Position Y de l'élément.
     */
    void dessinerLigneStat(sf::RenderTarget& cible, const std::string& nomEspece, const StatsEspece& stats, sf::Color couleur, float x, float y);
};