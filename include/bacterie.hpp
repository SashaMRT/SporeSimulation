#pragma once
#include "entite.hpp"

class Bacterie : public Entite {
public:
    Bacterie(sf::Vector2f pos);
    void update(float dt, Monde& monde) override;
    void dessiner(sf::RenderTarget& cible) const override;

private:
    float vitesseMax;
    void evoluer(Monde& monde);
};