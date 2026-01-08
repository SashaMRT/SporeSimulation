#pragma once

class Rendu {
public:
    Rendu();
    bool init(const std::string& cheminFont);
    void menu(const Monde& monde, sf::RenderTarget& cible, bool enPause, const Entite* survol = nullptr);

private:
    sf::Font font;
    sf::RectangleShape fond;
    sf::Text titre;

    void dessinerLigneStat(sf::RenderTarget& cible, const std::string& nomEspece, int valeur, sf::Color couleur, float x, float y) const;
};