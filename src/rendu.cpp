#include "rendu.hpp"
#include "constantes.hpp"
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>

Rendu::Rendu() : 
    titre(font),
    inspection(font),
    details(font),
    pause(font),
    description(font),
    controles(font),
    controlesGauche(font),
    controlesDroite(font),
    alerte(font),
    infoFps(font),
    infoTemps(font),
    nomStat(font),
    valeurStat(font)
{
    fond.setFillColor(Constantes::COULEUR_FOND_MENU);
    fond.setOutlineThickness(-2.f);
    fond.setOutlineColor(sf::Color(100, 100, 100));
}

bool Rendu::init(const std::string& cheminFont) {
    std::vector<std::string> chemins = {
        "assets/" + cheminFont,
        cheminFont,
        "../assets/" + cheminFont
    };

    bool fontChargee = false;
    for (const auto& chemin : chemins) {
        if (font.openFromFile(chemin)) {
            fontChargee = true;
            break;
        }
    }

    if (!fontChargee) return false;

    titre.setString("SPORE SIM");
    titre.setCharacterSize(28);
    titre.setFillColor(Constantes::COULEUR_TITRE);
    titre.setStyle(sf::Text::Bold);
    
    sf::FloatRect limitesTitre = titre.getLocalBounds();
    titre.setOrigin({limitesTitre.size.x / 2.f, 0.f});

    separateur.setSize({300.f, 2.f});
    separateur.setFillColor(sf::Color(100, 100, 100));

    infoFps.setCharacterSize(14);
    infoFps.setFillColor(sf::Color(180, 180, 180));

    infoTemps.setCharacterSize(14);
    infoTemps.setFillColor(sf::Color(180, 180, 180));

    inspection.setString("--- INSPECTION ---");
    inspection.setCharacterSize(18);
    inspection.setFillColor(sf::Color::Yellow);

    details.setCharacterSize(16);

    pause.setString("--- PAUSE ---");
    pause.setCharacterSize(20);
    pause.setFillColor(sf::Color::Yellow);
    pause.setStyle(sf::Text::Bold);

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
    description.setFillColor(Constantes::COULEUR_TEXTE_GRIS);

    controles.setString("Controles :");
    controles.setCharacterSize(16);
    controles.setFillColor(Constantes::COULEUR_TEXTE_CLAIR);
    controles.setStyle(sf::Text::Bold);

    controlesGauche.setString(
        "Survoler : Infos\n"
        "Clic Gauche : + Algue\n"
        "Clic Droit : + Bacterie"
    );
    controlesGauche.setCharacterSize(14);
    controlesGauche.setFillColor(Constantes::COULEUR_TEXTE_GRIS);

    controlesDroite.setString(
        "P : Pause\n"
        "R : Restart\n"
        "Echap : Quitter"
    );
    controlesDroite.setCharacterSize(14);
    controlesDroite.setFillColor(Constantes::COULEUR_TEXTE_GRIS);

    alerte.setString("Agrandir fenetre\npour plus d'infos");
    alerte.setCharacterSize(16);
    alerte.setStyle(sf::Text::Bold);
    
    sf::FloatRect rectAlerte = alerte.getLocalBounds();
    alerte.setOrigin({rectAlerte.size.x / 2.f, rectAlerte.size.y / 2.f});

    iconeStat.setRadius(8.f);
    iconeStat.setOutlineThickness(1.f);
    iconeStat.setOutlineColor(sf::Color::White);

    nomStat.setCharacterSize(20);
    nomStat.setFillColor(sf::Color(220, 220, 220));

    valeurStat.setCharacterSize(15);
    valeurStat.setStyle(sf::Text::Bold);

    fondJauge.setSize({300.f, 4.f});
    fondJauge.setFillColor(sf::Color(50, 50, 50));

    jauge.setSize({300.f, 4.f});

    return true;
}

void Rendu::menu(const Monde& monde, sf::RenderTarget& cible, bool enPause, float fps, float tempsEcoule, const Entite* survol) {
    sf::Vector2f tailleFenetre = cible.getView().getSize();
    sf::Vector2f centreFenetre = cible.getView().getCenter();

    float fenetreX = centreFenetre.x - tailleFenetre.x / 2.f;
    float fenetreY = centreFenetre.y - tailleFenetre.y / 2.f;
    float menuX = fenetreX + tailleFenetre.x - Constantes::MENU_LARGEUR;

    bool afficherControles = tailleFenetre.y > 600.f;
    bool afficherDescription = tailleFenetre.y > 700.f;

    fond.setPosition({menuX, fenetreY});
    fond.setSize({Constantes::MENU_LARGEUR, tailleFenetre.y});
    cible.draw(fond);

    infoFps.setString("FPS : " + std::to_string(static_cast<int>(fps)));
    infoFps.setPosition({menuX + 20.f, fenetreY + 15.f});
    cible.draw(infoFps);

    int minutes = static_cast<int>(tempsEcoule) / 60;
    int secondes = static_cast<int>(tempsEcoule) % 60;
    std::ostringstream ss;
    ss << "Temps : " << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << secondes;
    
    infoTemps.setString(ss.str());
    sf::FloatRect rectTemps = infoTemps.getLocalBounds();
    infoTemps.setOrigin({rectTemps.size.x, 0.f});
    infoTemps.setPosition({menuX + 360.f, fenetreY + 15.f});
    cible.draw(infoTemps);

    titre.setPosition({menuX + 190.f, fenetreY + 45.f});
    cible.draw(titre);

    separateur.setPosition({menuX + 40.f, fenetreY + 90.f});
    cible.draw(separateur);

    Stats statistiques = monde.getStats();
    float debutStatsY = fenetreY + 130.f;
    dessinerLigneStat(cible, "Algues", statistiques.algues, Constantes::ALGUE_COULEUR, menuX, debutStatsY);
    dessinerLigneStat(cible, "Bacteries", statistiques.bacteries, Constantes::BACTERIE_COULEUR, menuX, debutStatsY + 50.f);
    dessinerLigneStat(cible, "Herbivores", statistiques.herbivores, Constantes::HERBIVORE_COULEUR_LENT, menuX, debutStatsY + 100.f);
    dessinerLigneStat(cible, "Carnivores", statistiques.carnivores, Constantes::CARNIVORE_COULEUR, menuX, debutStatsY + 150.f);

    if (survol != nullptr) {
        if (tailleFenetre.y > 350.f) {
            inspection.setPosition({menuX + 40.f, fenetreY + 350.f});
            cible.draw(inspection);

            std::string typeStr = "Inconnu";
            sf::Color typeCol = sf::Color::White;

            switch(survol->getType()) {
                case TypeEntite::ALGUE: typeStr = "Algue"; typeCol = Constantes::ALGUE_COULEUR; break;
                case TypeEntite::BACTERIE: typeStr = "Bacterie"; typeCol = Constantes::BACTERIE_COULEUR; break;
                case TypeEntite::HERBIVORE: typeStr = "Herbivore"; typeCol = Constantes::HERBIVORE_COULEUR_LENT; break;
                case TypeEntite::CARNIVORE: typeStr = "Carnivore"; typeCol = Constantes::CARNIVORE_COULEUR; break;
                default: break;
            }

            std::string info = "Type : " + typeStr + "\n";
            info += "ID : " + std::to_string(survol->getId()) + "\n";
            info += "Energie : " + std::to_string((int)survol->getEnergie());

            details.setString(info);
            details.setFillColor(typeCol);
            details.setPosition({menuX + 40.f, fenetreY + 380.f});
            cible.draw(details);
        }
    }
    else if (enPause) {
        if (tailleFenetre.y > 350.f) {
            pause.setPosition({menuX + 40.f, fenetreY + 360.f});
            cible.draw(pause);
        }
    }
    else if (afficherDescription) {
        description.setPosition({menuX + 40.f, fenetreY + 350.f});
        cible.draw(description);
    }

    if (afficherControles) {
        controles.setPosition({menuX + 40.f, fenetreY + tailleFenetre.y - 120.f});
        cible.draw(controles);

        controlesGauche.setPosition({menuX + 40.f, fenetreY + tailleFenetre.y - 90.f});
        cible.draw(controlesGauche);

        controlesDroite.setPosition({menuX + 220.f, fenetreY + tailleFenetre.y - 90.f});
        cible.draw(controlesDroite);
    } else {
        static sf::Clock clockClignote;
        float transparence = (std::sin(clockClignote.getElapsedTime().asSeconds() * 3.f) + 1.f) / 2.f;
        
        alerte.setFillColor(sf::Color(255, 100, 100, 150 + static_cast<std::uint8_t>(transparence * 105)));
        alerte.setPosition({menuX + 190.f, fenetreY + tailleFenetre.y - 40.f});
        cible.draw(alerte);
    }
}

void Rendu::dessinerLigneStat(sf::RenderTarget& cible, const std::string& nomEspece, const StatsEspece& stats, sf::Color couleur, float x, float y) {
    iconeStat.setFillColor(couleur);
    iconeStat.setPosition({x + 40.f, y + 5.f});
    cible.draw(iconeStat);

    nomStat.setString(nomEspece);
    nomStat.setPosition({x + 70.f, y});
    cible.draw(nomStat);

    std::string texteStats = "V: " + std::to_string(stats.vivants) + 
                           "  M: " + std::to_string(stats.morts) + 
                           "  N: " + std::to_string(stats.naissances);

    valeurStat.setString(texteStats);
    valeurStat.setFillColor(couleur);
    
    sf::FloatRect rectValeur = valeurStat.getLocalBounds();
    valeurStat.setOrigin({rectValeur.size.x, 0.f});
    valeurStat.setPosition({x + 340.f, y});
    cible.draw(valeurStat);

    fondJauge.setPosition({x + 40.f, y + 30.f});
    cible.draw(fondJauge);

    float ratio = std::min(stats.vivants / 50.f, 1.f);
    jauge.setSize({300.f * ratio, 4.f});
    jauge.setFillColor(couleur);
    jauge.setPosition({x + 40.f, y + 30.f});
    cible.draw(jauge);
}