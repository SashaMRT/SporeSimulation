#pragma once
#include "entite.hpp"
#include <vector>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

class Monde {
private:
    std::vector<std::unique_ptr<Entite>> entites;
    std::map<TypeEntite, int> stats;
    sf::FloatRect limites;

public:
    Monde();
    Monde(sf::FloatRect limites);
    
    void update(float dt);
    void dessiner(sf::RenderTarget& cible) const;
    void spawnAlgue(sf::Vector2f pos);
    void spawnBacterie(sf::Vector2f pos);
    void remove();
    
    const auto& getStats() const { return stats; }
    sf::FloatRect getLimites() const { return limites; }
    Entite* getPlusProche(sf::Vector2f pos, TypeEntite typeCherche);
};
