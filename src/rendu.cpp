#include "rendu.hpp"

Rendu::Rendu() {
    fond.setSize({380.f, 720.f});
    fond.setFillColor({30, 30, 30, 200});
    fond.setPosition({900.f, 0.f});
}

void Rendu::menu(const Monde& monde, sf::RenderTarget& cible, float, unsigned int ticks) {
    cible.draw(fond);
}

