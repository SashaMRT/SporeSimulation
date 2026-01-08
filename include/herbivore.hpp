#pragma once
#include "entite.hpp"

class Herbivore : public Entite {
public:
    Herbivore(sf::Vector2f pos, bool rapide = false);
    void update(float dt, Monde& monde) override;
    void dessiner(sf::RenderTarget& cible) const override;

    bool estInvisible() const {return invisible;}

private:
    bool isRapide;
    bool invisible = false;
    float chronoCache = 0.f;
    float chronoRecharge = 0.f;
};