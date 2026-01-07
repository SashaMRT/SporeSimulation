#include "herbivore.hpp"
#include "monde.hpp"
#include <cmath>

Herbivore::Herbivore(sf::Vector2f pos) : Entite(pos, TypeEntite::HERBIVORE, 15.f, 100.f) { vitesse = {100.f, 50.f}; }

void Herbivore::update(float dt, Monde& monde) {
    float v = 150.f;
    Entite *a = monde.getPlusProche(position, TypeEntite::HERBIVORE), *d = monde.getPlusProche(position, TypeEntite::CARNIVORE), *m = monde.getPlusProche(position, TypeEntite::ALGUE);
    if (a && std::hypot(a->getPosition().x - position.x, a->getPosition().y - position.y) < 80.f) v = 300.f;
    if (d && std::hypot(d->getPosition().x - position.x, d->getPosition().y - position.y) < v) {
        sf::Vector2f f = position - d->getPosition();
        vitesse = (f / std::hypot(f.x, f.y)) * 125.f;
    } else if (m) {
        sf::Vector2f dir = m->getPosition() - position;
        float dist = std::hypot(dir.x, dir.y);
        vitesse = (dir / dist) * 90.f;
        if (dist < rayon + m->getRayon()) { energie += 30.f; m->tuer(); }
    }
    position += vitesse * dt;
    sf::FloatRect l = monde.getLimites();
    if (position.x < l.position.x || position.x > l.size.x) vitesse.x *= -1;
    if (position.y < l.position.y || position.y > l.size.y) vitesse.y *= -1;
    if ((energie -= 5.f * dt) <= 0) tuer();
}

void Herbivore::dessiner(sf::RenderTarget& cible) const {
    float aM = std::atan2(vitesse.y, vitesse.x);
    for (int i = 0; i < 5; ++i) {
        float aB = aM + 3.14159f + (i - 2.5f) * 0.3f;
        for (int k = 0; k < 6; ++k) {
            float ang = aB + std::sin((position.x + position.y) * 0.05f + k * 0.5f + i) * 0.2f;
            float dist = rayon * (0.8f + k * 0.4f), t = rayon * 0.25f * (1.f - k / 6.f);
            sf::CircleShape ten(t); ten.setOrigin({t, t});
            ten.setPosition(position + sf::Vector2f(std::cos(ang), std::sin(ang)) * dist);
            ten.setFillColor(sf::Color(100, 255, 100, 150 - k * 20)); cible.draw(ten);
        }
    }
    sf::CircleShape c(rayon); c.setOrigin({rayon, rayon}); c.setPosition(position);
    c.setFillColor(sf::Color(50, 220, 100, 200)); c.setOutlineThickness(2.f); c.setOutlineColor(sf::Color(150, 255, 150));
    cible.draw(c);
    sf::CircleShape r(rayon * 0.3f); r.setOrigin({rayon * 0.3f, rayon * 0.3f});
    r.setPosition(position + sf::Vector2f(-rayon * 0.2f, -rayon * 0.2f));
    r.setFillColor(sf::Color(255, 255, 255, 100)); cible.draw(r);
}