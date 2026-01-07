#include "carnivore.hpp"
#include "monde.hpp"
#include <cmath>

Carnivore::Carnivore(sf::Vector2f pos) : Entite(pos, TypeEntite::CARNIVORE, 20.f, 120.f) { vitesse = {120.f, 80.f}; }

void Carnivore::update(float dt, Monde& monde) {
    float b = 1.f;
    Entite *a = monde.getPlusProche(position, TypeEntite::CARNIVORE), *c = monde.getPlusProche(position, TypeEntite::HERBIVORE);
    if (a && std::hypot(a->getPosition().x - position.x, a->getPosition().y - position.y) < 100.f) b = 1.4f;
    if (c) {
        sf::Vector2f d = c->getPosition() - position;
        float dist = std::hypot(d.x, d.y);
        vitesse = (d / dist) * (130.f * b);
        if (dist < rayon + c->getRayon()) { energie += 50.f; c->tuer(); }
    }
    position += vitesse * dt;
    sf::FloatRect l = monde.getLimites();
    if (position.x < l.position.x || position.x > l.size.x) vitesse.x *= -1;
    if (position.y < l.position.y || position.y > l.size.y) vitesse.y *= -1;
    if ((energie -= 8.f * dt) <= 0) tuer();
}

void Carnivore::dessiner(sf::RenderTarget& cible) const {
    float aR = std::atan2(vitesse.y, vitesse.x), aD = aR * 180.f / 3.14159f;
    sf::Vector2f dA = {-std::cos(aR), -std::sin(aR)};
    for(int i = 3; i >= 1; --i) {
        float t = rayon * (0.7f - i * 0.1f);
        sf::ConvexShape s(3);
        s.setPoint(0, {t * 1.5f, 0}); s.setPoint(1, {-t, -t * 0.7f}); s.setPoint(2, {-t, t * 0.7f});
        s.setPosition(position + dA * (float)i * (rayon * 0.7f));
        s.setRotation(sf::degrees(aD + std::sin((position.x + position.y) * 0.1f + i) * 15.f));
        s.setFillColor(sf::Color(180 - i*20, 30, 30)); s.setOutlineThickness(1.f); s.setOutlineColor(sf::Color(100, 0, 0));
        cible.draw(s);
    }
    sf::ConvexShape c(4);
    c.setPoint(0, {rayon * 1.8f, 0}); c.setPoint(1, {0, -rayon}); c.setPoint(2, {-rayon * 0.8f, 0}); c.setPoint(3, {0, rayon});
    c.setPosition(position); c.setRotation(sf::degrees(aD));
    c.setFillColor(sf::Color(220, 50, 50)); c.setOutlineThickness(2.f); c.setOutlineColor(sf::Color(150, 20, 20));
    cible.draw(c);
    sf::CircleShape o(rayon * 0.25f); o.setOrigin({rayon * 0.25f, rayon * 0.25f});
    o.setPosition(position + sf::Vector2f(std::cos(aR), std::sin(aR)) * rayon * 0.5f);
    o.setFillColor(sf::Color::Yellow); o.setOutlineThickness(1.f); o.setOutlineColor(sf::Color::Black);
    cible.draw(o);
    sf::CircleShape p(rayon * 0.1f); p.setOrigin({rayon * 0.1f, rayon * 0.1f});
    p.setPosition(o.getPosition() + sf::Vector2f(std::cos(aR), std::sin(aR)) * rayon * 0.1f);
    p.setFillColor(sf::Color::Black); cible.draw(p);
    sf::ConvexShape po(3); po.setPoint(0, {rayon * 0.4f, 0}); po.setPoint(1, {0, -rayon * 0.2f}); po.setPoint(2, {0, rayon * 0.2f});
    po.setFillColor(sf::Color(100, 0, 0));
    po.setPosition(position + sf::Vector2f(std::cos(aR - 1.57f), std::sin(aR - 1.57f)) * rayon); po.setRotation(sf::degrees(aD - 90));
    cible.draw(po);
    po.setPosition(position + sf::Vector2f(std::cos(aR + 1.57f), std::sin(aR + 1.57f)) * rayon); po.setRotation(sf::degrees(aD + 90));
    cible.draw(po);
}