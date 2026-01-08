#pragma once

class Algue : public Entite {
public:
    Algue(sf::Vector2f pos);
    
    void update(float dt, Monde& monde) override;
    void dessiner(sf::RenderTarget& cible) const override;
};
