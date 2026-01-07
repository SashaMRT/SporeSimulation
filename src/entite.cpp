#include "entite.hpp"
#include <cmath>

Entite::Entite(sf::Vector2f pos, TypeEntite type, float rayon, float energie)
    : position(pos), vitesse(0.f, 0.f), rayon(rayon), 
      energie(energie), type(type), vivante(true) {
}
