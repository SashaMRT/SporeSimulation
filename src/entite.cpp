#include "entite.hpp"
#include <cmath>

int Entite::compteurId = 0;

Entite::Entite(sf::Vector2f pos, TypeEntite type, float rayon, float energie)
    : position(pos), type(type), rayon(rayon), energie(energie), vivante(true) {    
    id = ++compteurId; 
}