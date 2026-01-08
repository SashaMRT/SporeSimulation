#pragma once
#include "entite.hpp"
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

struct Stats {
    int nbAlgues = 0;
    int nbBacteries = 0;
    int nbHerbivores = 0;
    int nbCarnivores = 0;
};

class Monde {
private:
    std::vector<std::unique_ptr<Entite>> entites;
    sf::FloatRect limites;

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
    
    Stats getStats() const;
    sf::FloatRect getLimites() const { return limites; }
    Entite* getPlusProche(sf::Vector2f pos, TypeEntite typeCherche);
    Entite* getEntiteSousSouris(sf::Vector2f pos, float rayon = 30.f);
};