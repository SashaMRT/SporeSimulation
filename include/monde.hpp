#pragma once

/**
 * @struct StatsEspece
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Structure regroupant les compteurs pour une espèce donnée.
 * @details Utilisée pour le suivi démographique (vivants, morts, naissances).
 */
struct StatsEspece {
    int vivants = 0;    /**< Nombre d'individus actuellement en vie. */
    int morts = 0;      /**< Nombre total de morts depuis le début. */
    int naissances = 0; /**< Nombre total de naissances depuis le début. */
};

/**
 * @struct Stats
 * @brief Conteneur global des statistiques de la simulation.
 * @details Regroupe les stats par espèce. Contient notamment la `std::map`
 * requise pour valider l'utilisation de 3 conteneurs STL différents dans le projet.
 */
struct Stats {
    // Membres explicites pour accès rapide (legacy)
    StatsEspece algues;
    StatsEspece bacteries;
    StatsEspece herbivores;
    StatsEspece carnivores;

    /** * @brief Stockage associatif des statistiques.
     * @details Clé : TypeEntite, Valeur : StatsEspece.
     * Valide l'exigence du 3ème conteneur STL (Map).
     */
    std::map<TypeEntite, StatsEspece> data;
};

/**
 * @class Monde
 * @brief Classe principale gérant la simulation.
 * @details Le Monde possède toutes les entités, gère la boucle de mise à jour,
 * les collisions, le cycle de vie (spawn/death) et le rendu.
 */
class Monde {
private:
    /**
     * @brief Conteneur principal des entités (Polymorphisme).
     * @details Utilisation de vector pour un parcours séquentiel rapide lors de l'update/dessin.
     * unique_ptr assure la gestion automatique de la mémoire.
     */
    std::vector<std::unique_ptr<Entite>> entites;

    /**
     * @brief Conteneur des particules (Effets visuels).
     * @details Utilisation de list car les particules sont fréquemment supprimées
     * au milieu de la collection (suppression en O(1)).
     */
    std::list<Particule> particules;

    sf::FloatRect limites; /**< Dimensions physiques du monde. */

    // --- Compteurs persistants pour les statistiques ---
    int mortsAlgues = 0;
    int mortsBacteries = 0;
    int mortsHerbivores = 0;
    int mortsCarnivores = 0;

    int naissancesAlgues = 0;
    int naissancesBacteries = 0;
    int naissancesHerbivores = 0;
    int naissancesCarnivores = 0;

public:
    /**
     * @brief Constructeur du Monde.
     * @param limites Rectangle définissant la zone de jeu.
     */
    Monde(sf::FloatRect limites);
    
    /**
     * @brief Met à jour la simulation.
     * @details Appelle update() sur toutes les entités, gère les collisions,
     * la physique des particules et le nettoyage des morts.
     * @param dt Temps écoulé (Delta Time).
     */
    void update(float dt);

    /**
     * @brief Dessine tout le contenu du monde.
     * @param cible Fenêtre de rendu.
     */
    void dessiner(sf::RenderTarget& cible) const;

    /**
     * @brief Fait apparaître une algue à une position donnée.
     * @param pos Position (x,y).
     */
    void spawnAlgue(sf::Vector2f pos);

    /**
     * @brief Fait apparaître une bactérie.
     * @param pos Position (x,y).
     */
    void spawnBacterie(sf::Vector2f pos);

    /**
     * @brief Supprime les entités mortes de la mémoire.
     * @details Utilise l'idiome erase-remove sur le vecteur d'entités.
     */
    void remove();

    /**
     * @brief Réinitialise la simulation (supprime tout, remet les stats à 0).
     */
    void reset();

    /**
     * @brief Fait apparaître un rocher (obstacle).
     * @param pos Position (x,y).
     */
    void spawnRocher(sf::Vector2f pos);

    /**
     * @brief Vérifie si une position permet de se cacher derrière un rocher.
     * @param pos Position à tester.
     * @return true si une cachette valide est trouvée.
     */
    bool estCache(sf::Vector2f pos) const;

    /**
     * @brief Gère la physique de collision entre une entité et les rochers.
     * @param e L'entité à vérifier.
     */
    void gererCollisionsRochers(Entite& e);

    /**
     * @brief Libère un rocher (le rend disponible pour une autre cachette).
     * @param pos Position autour de laquelle chercher le rocher.
     */
    void libererRocherProche(sf::Vector2f pos);

    /**
     * @brief Redimensionne le monde (ex: redimensionnement fenêtre).
     * @param taille Nouvelle taille (largeur, hauteur).
     */
    void setTaille(sf::Vector2f taille);

    /**
     * @brief Génère une explosion de particules.
     * @param pos Centre de l'explosion.
     * @param couleur Couleur des particules.
     * @param nb Nombre de particules.
     */
    void creerExplosion(sf::Vector2f pos, sf::Color couleur, int nb);

    /**
     * @brief Dessine l'arrière-plan du monde.
     * @param cible Fenêtre de rendu.
     */
    void dessinerFond(sf::RenderTarget& cible) const;
    
    /**
     * @brief Compile et retourne les statistiques actuelles.
     * @return Structure Stats contenant la Map de données.
     */
    Stats getStats() const;

    /**
     * @brief Getter pour les limites du monde.
     */
    sf::FloatRect getLimites() const { return limites; }

    /**
     * @brief Trouve l'entité la plus proche d'un certain type.
     * @details Utilisé par l'IA pour chercher nourriture, partenaires ou dangers.
     * @param pos Position de départ.
     * @param typeCherche Type d'entité recherché.
     * @return Pointeur vers l'entité ou nullptr.
     */
    Entite* getPlusProche(sf::Vector2f pos, TypeEntite typeCherche);

    /**
     * @brief Récupère l'entité sous la souris (Interaction utilisateur).
     * @param pos Position de la souris dans le monde.
     * @param rayon Rayon de tolérance pour le clic.
     * @return Pointeur vers l'entité ou nullptr.
     */
    Entite* getEntiteSousSouris(sf::Vector2f pos, float rayon = 30.f);
};