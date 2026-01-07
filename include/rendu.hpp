#pragma once
#include "monde.hpp"
#include <SFML/Graphics.hpp>

class Rendu {
public:
    Rendu();
    void menu(const Monde& monde, sf::RenderTarget& cible, float tempsReel, unsigned int ticks);

private:
    sf::Font font;
    sf::RectangleShape fond;
};
