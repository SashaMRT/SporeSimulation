#pragma once

class Rocher : public Entite {
public:
    Rocher(sf::Vector2f pos);
    
    void update(float dt, Monde& monde) override;
    void dessiner(sf::RenderTarget& cible) const override;

    bool estOccupe() const { return occupe; }
    void setOccupe(bool etat) { occupe = etat; }

private:
    bool occupe = false;
};