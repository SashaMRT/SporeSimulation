#pragma once

/**
 * @namespace Constantes
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Regroupe toutes les valeurs constantes de configuration de la simulation.
 * @details Ce namespace centralise les paramètres d'équilibrage du jeu (Game Design),
 * les configurations graphiques et les règles physiques.
 * Modifier ces valeurs permet d'ajuster la difficulté et le comportement de l'écosystème.
 */
namespace Constantes {

    // --- Configuration de la Fenêtre ---
    constexpr float FENETRE_LARGEUR = 1280.f; /**< Largeur de la fenêtre en pixels. */
    constexpr float FENETRE_HAUTEUR = 720.f;  /**< Hauteur de la fenêtre en pixels. */
    constexpr unsigned int FPS_LIMITE = 60;   /**< Limite d'images par seconde (V-Sync). */
    
    // --- Configuration Initiale du Monde ---
    constexpr int NB_ROCHERS_INIT = 6;    /**< Nombre de rochers générés au démarrage. */
    constexpr int NB_ALGUES_INIT = 60;    /**< Nombre d'algues générées au démarrage. */
    constexpr int NB_BACTERIES_INIT = 10; /**< Nombre de bactéries générées au démarrage. */

    // --- Configuration de l'Interface Utilisateur (HUD) ---
    constexpr float MENU_LARGEUR = 380.f; /**< Largeur du panneau latéral droit. */
    const sf::Color COULEUR_FOND_MENU(25, 25, 30, 230); /**< Couleur de fond du menu (Gris foncé semi-transparent). */
    const sf::Color COULEUR_TITRE = sf::Color::White;   /**< Couleur du titre principal. */
    const sf::Color COULEUR_TEXTE_GRIS(150, 150, 150);  /**< Couleur pour le texte secondaire. */
    const sf::Color COULEUR_TEXTE_CLAIR(200, 200, 200); /**< Couleur pour le texte principal. */
    const sf::Color COULEUR_ZONE_JEU(10, 30, 50);       /**< Couleur de fond de la zone de simulation (Bleu nuit). */

    // --- Paramètres des Algues (Ressource primaire) ---
    constexpr float ALGUE_RAYON = 5.f;      /**< Taille d'une algue. */
    constexpr float ALGUE_ENERGIE = 50.f;   /**< Énergie apportée à celui qui la mange. */
    const sf::Color ALGUE_COULEUR(34, 139, 34); /**< Vert forêt. */

    // --- Paramètres des Bactéries (Organisme primaire) ---
    constexpr float BACTERIE_RAYON = 8.f;       /**< Taille d'une bactérie. */
    constexpr float BACTERIE_ENERGIE = 100.f;   /**< Énergie de départ. */
    constexpr float BACTERIE_SEUIL_EVOLUTION = 110.f; /**< Énergie requise pour évoluer (Facile : 1 algue suffit). */
    const sf::Color BACTERIE_COULEUR(0, 255, 255); /**< Cyan. */

    // --- Paramètres des Herbivores (Consommateurs primaires) ---
    constexpr float HERBIVORE_ENERGIE_BASE = 150.f; /**< Énergie de départ. */
    constexpr float HERBIVORE_VITESSE_BASE = 90.f;  /**< Vitesse de déplacement standard. */
    constexpr float HERBIVORE_RAYON_BASE = 15.f;    /**< Taille standard. */
    const sf::Color HERBIVORE_COULEUR_LENT(50, 220, 100);   /**< Vert clair pour les lents. */
    const sf::Color HERBIVORE_COULEUR_RAPIDE(0, 191, 255);  /**< Bleu ciel pour les rapides. */

    // --- Paramètres des Carnivores (Prédateurs) ---
    constexpr float CARNIVORE_ENERGIE_BASE = 160.f; /**< Énergie de départ (basse pour forcer la chasse rapide). */
    constexpr float CARNIVORE_VITESSE_BASE = 110.f; /**< Vitesse de base (plus rapide que les proies). */
    constexpr float CARNIVORE_RAYON_BASE = 20.f;    /**< Taille standard (plus imposant). */
    const sf::Color CARNIVORE_COULEUR(255, 50, 50); /**< Rouge vif. */

    // --- Paramètres des Rochers (Obstacles) ---
    constexpr float ROCHER_RAYON_BASE = 20.f; /**< Taille minimale d'un rocher. */
    constexpr float ROCHER_ENERGIE = 1000.f;  /**< "Santé" structurelle (non utilisé actuellement). */
    constexpr float ROCHER_DEGATS = 0.01f;    /**< Dégâts infligés par contact (collision). */
    const sf::Color ROCHER_COULEUR(100, 100, 100); /**< Gris pierre. */

    // --- Règles de Reproduction et Mutation ---
    constexpr float ENERGIE_REPRODUCTION = 190.f; /**< Seuil d'énergie pour se reproduire. */
    constexpr float COUT_REPRODUCTION = 70.f;     /**< Énergie perdue par le parent lors de la naissance. */
    constexpr float TAUX_MUTATION = 0.15f;        /**< Pourcentage de variation génétique (+/- 15%). */

    // --- Système de Particules (Effets Visuels) ---
    constexpr float PARTICULE_VIE_DECAY = 0.8f;   /**< Vitesse de disparition (Fading). */
    constexpr float PARTICULE_FRICTION = 0.98f;   /**< Ralentissement par friction de l'air/eau. */
    constexpr int PARTICULE_NB_EXPLOSION = 20;    /**< Nombre de particules générées lors d'une explosion. */
}