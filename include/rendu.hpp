#pragma once

class Rendu {
public:
    Rendu();
    bool init(const std::string& cheminFont);
    void menu(const Monde& monde, sf::RenderTarget& cible, bool enPause, float fps, float tempsEcoule, const Entite* survol = nullptr);

private:
    sf::Font font;
    sf::RectangleShape fond;
    sf::RectangleShape separateur;
    sf::Text titre;

    sf::Text inspection;
    sf::Text details;
    sf::Text pause;
    sf::Text description;
    sf::Text controles;
    sf::Text controlesGauche;
    sf::Text controlesDroite;
    sf::Text alerte;
    
    sf::Text infoFps;
    sf::Text infoTemps;

    sf::CircleShape iconeStat;
    sf::Text nomStat;
    sf::Text valeurStat;
    sf::RectangleShape fondJauge;
    sf::RectangleShape jauge;

    void dessinerLigneStat(sf::RenderTarget& cible, const std::string& nomEspece, const StatsEspece& stats, sf::Color couleur, float x, float y);
};