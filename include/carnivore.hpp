#pragma once

class Carnivore : public Entite {
public:
    Carnivore(sf::Vector2f pos, float vitesse, float taille, float vue);
    void update(float dt, Monde& monde) override;
    void dessiner(sf::RenderTarget& cible) const override;

    float getVitesseMax() const { return vitesseMax; }
    float getPorteeVue() const { return porteeVue; }

private:
    float vitesseMax;
    float porteeVue;
};