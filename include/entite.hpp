#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

class Monde;

enum class TypeEntite {
    ALGUE,
    HERBIVORE,
    PREDATEUR
};

class Entite {
protected:
    sf::Vector2f position;
    sf::Vector2f vitesse;
    float rayon;
    float energie;
    TypeEntite type;
    bool vivante;

public:
    Entite(sf::Vector2f pos, TypeEntite type, float rayon = 15.f, float energie = 100.f);
    virtual ~Entite() = default;

    virtual void update(float dt, Monde& monde) = 0;
    virtual void dessiner(sf::RenderTarget& cible) const = 0;

    sf::Vector2f getPosition() const { return position; }
    float getRayon() const { return rayon; }
    float getEnergie() const { return energie; }
    TypeEntite getType() const { return type; }
    bool estVivante() const { return vivante; }

protected:
    void setPosition(sf::Vector2f pos) { position = pos; }
    void setEnergie(float e) { energie = e; }
    void tuer() { vivante = false; }
};
