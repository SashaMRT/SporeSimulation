#pragma once

class Monde;

enum class TypeEntite {
    ALGUE,
    BACTERIE,
    HERBIVORE,
    CARNIVORE,
    ROCHER
};

class Entite {
protected:
    sf::Vector2f position;
    sf::Vector2f vitesse;
    float rayon;
    float energie;
    TypeEntite type;
    bool vivante;
    int id;
    static int compteurId;

public:
    Entite(sf::Vector2f pos, TypeEntite type, float rayon = 15.f, float energie = 100.f);
    virtual ~Entite() = default;

    virtual void update(float dt, Monde& monde) = 0;
    virtual void dessiner(sf::RenderTarget& cible) const = 0;

    sf::Vector2f getPosition() const { return position; }
    float getRayon() const { return rayon; }
    float getEnergie() const { return energie; }
    int getId() const { return id; }
    TypeEntite getType() const { return type; }

    bool estVivante() const { return vivante; }
    void tuer() { vivante = false; }
    void setEnergie(float e) { energie = e; }
    void setPosition(sf::Vector2f pos) { position = pos; }
};
