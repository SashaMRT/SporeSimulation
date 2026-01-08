#pragma once

class Monde;

/**
 * @enum TypeEntite
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Énumération des différents types d'entités existant dans la simulation.
 * @details Utilisé pour identifier le comportement et le rôle de chaque objet 
 * sans avoir recours systématiquement au RTTI (dynamic_cast).
 */
enum class TypeEntite {
    ALGUE,      /**< Nourriture statique. */
    BACTERIE,   /**< Organisme simple primaire. */
    HERBIVORE,  /**< Consommateur primaire. */
    CARNIVORE,  /**< Prédateur. */
    ROCHER      /**< Obstacle statique. */
};

/**
 * @class Entite
 * @brief Classe de base abstraite pour tous les objets du monde.
 * @details Définit les propriétés communes (position, physique, vie) et l'interface
 * que toutes les entités doivent implémenter (update, dessiner).
 */
class Entite {
protected:
    sf::Vector2f position;  /**< Position actuelle (x, y) dans le monde. */
    sf::Vector2f vitesse;   /**< Vecteur de déplacement courant. */
    float rayon;            /**< Rayon physique (utilisé pour collisions et rendu). */
    float energie;          /**< Jauge de vie/stamina. Si <= 0, l'entité meurt. */
    TypeEntite type;        /**< Type de l'entité (pour identification rapide). */
    bool vivante;           /**< Flag de vie. Si false, l'entité sera supprimée au prochain cycle. */
    int id;                 /**< Identifiant unique de l'instance. */
    static int compteurId;  /**< Compteur statique partagé pour générer les IDs uniques. */

public:
    /**
     * @brief Constructeur de la classe Entite.
     * @param pos Position de départ.
     * @param type Type de l'entité.
     * @param rayon Taille de l'entité (défaut 15.f).
     * @param energie Énergie de départ (défaut 100.f).
     */
    Entite(sf::Vector2f pos, TypeEntite type, float rayon = 15.f, float energie = 100.f);
    
    /**
     * @brief Destructeur virtuel par défaut.
     * @details Nécessaire pour que le destructeur des classes filles soit appelé correctement.
     */
    virtual ~Entite() = default;

    /**
     * @brief Méthode virtuelle pure de mise à jour.
     * @details Doit être implémentée par chaque classe fille pour définir son comportement spécifique (IA).
     * @param dt Temps écoulé depuis la dernière frame.
     * @param monde Référence au monde pour interagir avec l'environnement.
     */
    virtual void update(float dt, Monde& monde) = 0;

    /**
     * @brief Méthode virtuelle pure de rendu.
     * @details Doit être implémentée pour définir l'apparence graphique de l'entité.
     * @param cible Fenêtre de rendu.
     */
    virtual void dessiner(sf::RenderTarget& cible) const = 0;

    // --- Accesseurs (Getters) ---

    /** @brief Retourne la position actuelle. */
    sf::Vector2f getPosition() const { return position; }
    
    /** @brief Retourne le rayon (taille). */
    float getRayon() const { return rayon; }
    
    /** @brief Retourne la quantité d'énergie restante. */
    float getEnergie() const { return energie; }
    
    /** @brief Retourne l'identifiant unique. */
    int getId() const { return id; }
    
    /** @brief Retourne le type de l'entité. */
    TypeEntite getType() const { return type; }

    /** @brief Vérifie si l'entité est toujours en vie. */
    bool estVivante() const { return vivante; }

    // --- Mutateurs (Setters) ---

    /** @brief Marque l'entité comme morte (sera supprimée). */
    void tuer() { vivante = false; }
    
    /** @brief Modifie directement le niveau d'énergie. */
    void setEnergie(float e) { energie = e; }
    
    /** @brief Téléporte l'entité à une nouvelle position. */
    void setPosition(sf::Vector2f pos) { position = pos; }
};