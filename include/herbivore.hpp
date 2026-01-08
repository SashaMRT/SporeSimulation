#pragma once

class Herbivore : public Entite {
public:
    Herbivore(sf::Vector2f pos, float vitesse, float taille, float vue);
    void update(float dt, Monde& monde) override;
    void dessiner(sf::RenderTarget& cible) const override;

    bool estInvisible() const {return invisible;}
    float getVitesseMax() const { return vitesseMax; }
    float getPorteeVue() const { return porteeVue; }
    bool estRapide() const { return isRapide; }

private:
    float vitesseMax;
    float porteeVue;

    bool invisible;
    float chronoCache;
    float chronoRecharge;
    bool isRapide;
};