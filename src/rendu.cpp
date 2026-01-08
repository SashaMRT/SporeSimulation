#include "rendu.hpp"
#include <vector>
#include <string>
#include <algorithm>

Rendu::Rendu() : titre(font) {
    fond.setSize({380.f, 720.f});
    fond.setFillColor(sf::Color(25, 25, 30, 230));
    fond.setOutlineThickness(-2.f);
    fond.setOutlineColor(sf::Color(100, 100, 100));
    fond.setPosition({900.f, 0.f});
}

bool Rendu::init(const std::string& nomFichier) {
    std::vector<std::string> chemins = {
        "assets/" + nomFichier,
        nomFichier,
        "../assets/" + nomFichier
    };

    for (const auto& chemin : chemins) {
        if (font.openFromFile(chemin)) {
            titre.setFont(font);
            titre.setString("SPORE SIM");
            titre.setCharacterSize(28);
            titre.setFillColor(sf::Color::White);
            titre.setStyle(sf::Text::Bold);
            
            sf::FloatRect limitesTitre = titre.getLocalBounds();
            titre.setOrigin({limitesTitre.size.x / 2.f, 0.f});
            titre.setPosition({1090.f, 30.f});
            return true;
        }
    }
    return false;
}

void Rendu::menu(const Monde& monde, sf::RenderTarget& cible, bool enPause, const Entite* survol) {
    cible.draw(fond);
    cible.draw(titre);

    sf::RectangleShape separateur({300.f, 2.f});
    separateur.setFillColor(sf::Color(100, 100, 100));
    separateur.setPosition({940.f, 80.f});
    cible.draw(separateur);

    Stats statistiques = monde.getStats();
    dessinerLigneStat(cible, "Algues", statistiques.nbAlgues, sf::Color(50, 255, 50), 120.f);
    dessinerLigneStat(cible, "Bacteries", statistiques.nbBacteries, sf::Color(0, 255, 255), 170.f);
    dessinerLigneStat(cible, "Herbivores", statistiques.nbHerbivores, sf::Color(150, 255, 100), 220.f);
    dessinerLigneStat(cible, "Carnivores", statistiques.nbCarnivores, sf::Color(255, 50, 50), 270.f);

    if (survol != nullptr) {
        sf::Text infos(font, "--- INSPECTION ---", 18);
        infos.setFillColor(sf::Color::Yellow);
        infos.setPosition({940.f, 340.f});
        cible.draw(infos);

        std::string typeStr = "Inconnu";
        sf::Color typeCol = sf::Color::White;
        
        switch(survol->getType()) {
            case TypeEntite::ALGUE: typeStr = "Algue"; typeCol = sf::Color::Green; break;
            case TypeEntite::BACTERIE: typeStr = "Bacterie"; typeCol = sf::Color::Cyan; break;
            case TypeEntite::HERBIVORE: typeStr = "Herbivore"; typeCol = sf::Color(150, 255, 100); break;
            case TypeEntite::CARNIVORE: typeStr = "Carnivore"; typeCol = sf::Color::Red; break;
        }

        std::string info = "Type : " + typeStr + "\n";
        info += "ID : " + std::to_string(survol->getId()) + "\n";
        info += "Energie : " + std::to_string((int)survol->getEnergie());

        sf::Text details(font, info, 16);
        details.setFillColor(typeCol);
        details.setPosition({940.f, 370.f});
        cible.draw(details);
    } 
    else if (enPause) {
        sf::Text pause(font, "--- PAUSE ---", 20);
        pause.setFillColor(sf::Color::Yellow); 
        pause.setStyle(sf::Text::Bold);
        pause.setPosition({940.f, 350.f});
        cible.draw(pause);
    } 
    else {
        sf::Text description(font);
        description.setString(
            "Bienvenue dans le Micro-Monde.\n\n"
            "Tout commence par des bacteries.\n"
            "En se nourrissant d'algues, elles\n"
            "grandissent et mutent.\n\n"
            "Certaines deviennent des herbivores\n"
            "agiles (vert), d'autres des\n"
            "predateurs carnivores (rouge).\n\n"
            "Observez la selection naturelle\n"
            "a l'oeuvre !"
        );        
        description.setCharacterSize(15);
        description.setFillColor(sf::Color(150, 150, 150));
        description.setPosition({940.f, 330.f});
        cible.draw(description);
    }

    sf::Text Controles(font, "Controles :", 16);
    Controles.setFillColor(sf::Color(200, 200, 200));
    Controles.setStyle(sf::Text::Bold);
    Controles.setPosition({940.f, 620.f});
    cible.draw(Controles);

    sf::Text colGauche(font);
    colGauche.setString(
        "Survoler : Infos\n"
        "Clic Gauche : + Algue\n"
        "Clic Droit : + Bacterie"
    );
    colGauche.setCharacterSize(14);
    colGauche.setFillColor(sf::Color(150, 150, 150));
    colGauche.setPosition({940.f, 650.f}); 
    cible.draw(colGauche);

    sf::Text colDroite(font);
    colDroite.setString(
        "P : Pause\n"
        "R : Restart\n"
        "Echap : Quitter"
    );
    colDroite.setCharacterSize(14);
    colDroite.setFillColor(sf::Color(150, 150, 150));
    colDroite.setPosition({1120.f, 650.f}); 
    cible.draw(colDroite);
}

void Rendu::dessinerLigneStat(sf::RenderTarget& cible, const std::string& label, int quantite, sf::Color couleur, float y) const {
    sf::CircleShape icone(8.f);
    icone.setFillColor(couleur);
    icone.setOutlineThickness(1.f);
    icone.setOutlineColor(sf::Color::White);
    icone.setPosition({940.f, y + 5.f});
    cible.draw(icone);

    sf::Text nom(font);
    nom.setString(label);
    nom.setCharacterSize(20);
    nom.setFillColor(sf::Color(220, 220, 220));
    nom.setPosition({970.f, y});
    cible.draw(nom);

    sf::Text valeur(font);
    valeur.setString(std::to_string(quantite));
    valeur.setCharacterSize(20);
    valeur.setStyle(sf::Text::Bold);
    valeur.setFillColor(couleur);
    
    sf::FloatRect rectValeur = valeur.getLocalBounds();
    valeur.setOrigin({rectValeur.size.x, 0.f});
    valeur.setPosition({1240.f, y});
    cible.draw(valeur);

    sf::RectangleShape fondJauge({300.f, 4.f});
    fondJauge.setFillColor(sf::Color(50, 50, 50));
    fondJauge.setPosition({940.f, y + 30.f});
    cible.draw(fondJauge);

    float ratio = std::min(quantite / 50.f, 1.f);
    sf::RectangleShape jauge({300.f * ratio, 4.f});
    jauge.setFillColor(couleur);
    jauge.setPosition({940.f, y + 30.f});
    cible.draw(jauge);
}