#pragma once
#include "entite.hpp"

class Herbivore : public Entite {
public:
    Herbivore(sf::Vector2f pos, bool rapide = false);
    void update(float dt, Monde& monde) override;
    void dessiner(sf::RenderTarget& cible) const override;

private:
    bool isRapide;
};