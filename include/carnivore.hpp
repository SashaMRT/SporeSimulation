#pragma once
#include "entite.hpp"

class Carnivore : public Entite {
public:
    Carnivore(sf::Vector2f pos, bool alpha = false);
    void update(float dt, Monde& monde) override;
    void dessiner(sf::RenderTarget& cible) const override;

private:
    bool isAlpha;
};