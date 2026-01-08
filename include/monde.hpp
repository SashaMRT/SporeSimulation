#pragma once

struct StatsEspece {
    int vivants = 0;
    int morts = 0;
    int naissances = 0;
};

struct Stats {
    StatsEspece algues;
    StatsEspece bacteries;
    StatsEspece herbivores;
    StatsEspece carnivores;
};

class Monde {
private:
    std::vector<std::unique_ptr<Entite>> entites;
    std::vector<Particule> particules;
    sf::FloatRect limites;

    // Compteurs persistants
    int mortsAlgues = 0;
    int mortsBacteries = 0;
    int mortsHerbivores = 0;
    int mortsCarnivores = 0;

    int naissancesAlgues = 0;
    int naissancesBacteries = 0;
    int naissancesHerbivores = 0;
    int naissancesCarnivores = 0;

public:
    Monde(sf::FloatRect limites);
    
    void update(float dt);
    void dessiner(sf::RenderTarget& cible) const;
    void spawnAlgue(sf::Vector2f pos);
    void spawnBacterie(sf::Vector2f pos);
    void remove();
    void reset();
    void spawnRocher(sf::Vector2f pos);
    bool estCache(sf::Vector2f pos) const;
    void gererCollisionsRochers(Entite& e);
    void libererRocherProche(sf::Vector2f pos);
    void setTaille(sf::Vector2f taille);
    void creerExplosion(sf::Vector2f pos, sf::Color couleur, int nb);
    void dessinerFond(sf::RenderTarget& cible) const;
    
    Stats getStats() const;
    sf::FloatRect getLimites() const { return limites; }
    Entite* getPlusProche(sf::Vector2f pos, TypeEntite typeCherche);
    Entite* getEntiteSousSouris(sf::Vector2f pos, float rayon = 30.f);
};