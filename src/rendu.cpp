/**
 * @file rendu.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Rendu.
 * @details Gère toute la partie graphique de l'interface utilisateur (HUD), 
 * l'affichage des menus, des statistiques, et des informations contextuelles 
 * (inspection des entités via info-bulles).
 */

#include "rendu.hpp"
#include "constantes.hpp"
#include "herbivore.hpp"
#include "carnivore.hpp"

#include <cmath>
#include <algorithm>
#include <iomanip>
#include <sstream>

/**
 * @brief Constructeur de la classe Rendu.
 * Initialise les références aux polices pour tous les objets texte 
 * et configure le style visuel du panneau latéral.
 */
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
    // Configuration du style du panneau (fond et bordures)
    fond.setFillColor(Constantes::COULEUR_FOND_MENU);
    fond.setOutlineThickness(-2.f);
    fond.setOutlineColor(sf::Color(100, 100, 100));
}

/**
 * @brief Initialise les ressources graphiques (polices et textes statiques).
 * Charge la police d'écriture et configure la taille, la couleur et le 
 * point d'origine de tous les éléments textuels de l'interface.
 * @param cheminFont Nom du fichier de police (ex: "arial.ttf").
 * @return true si la police a été chargée avec succès, false sinon.
 */
bool Rendu::init(const std::string& cheminFont) {
    // Tentative de chargement depuis plusieurs chemins relatifs pour la robustesse
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

    // --- Configuration du Titre principal ---
    titre.setString("SPORE SIMULATION");
    titre.setCharacterSize(28);
    titre.setFillColor(Constantes::COULEUR_TITRE);
    titre.setStyle(sf::Text::Bold);
    
    // Centrage de l'origine pour l'alignement
    sf::FloatRect limitesTitre = titre.getLocalBounds();
    titre.setOrigin({limitesTitre.size.x / 2.f, 0.f});

    // Ligne séparatrice
    separateur.setSize({300.f, 2.f});
    separateur.setFillColor(sf::Color(100, 100, 100));

    // --- Configuration des infos techniques ---
    infoFps.setCharacterSize(14);
    infoFps.setFillColor(sf::Color(180, 180, 180));

    infoTemps.setCharacterSize(14);
    infoTemps.setFillColor(sf::Color(180, 180, 180));

    // --- Configuration de l'en-tête d'inspection ---
    inspection.setString("--- INFORMATIONS ---");
    inspection.setCharacterSize(18);
    inspection.setFillColor(sf::Color::Yellow);

    details.setCharacterSize(16);

    // --- Configuration du texte de Pause ---
    pause.setString("--- PAUSE ---");
    pause.setCharacterSize(20);
    pause.setFillColor(sf::Color::Yellow);
    pause.setStyle(sf::Text::Bold);

    // --- Texte descriptif (Lore) ---
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
    
    // Centrage de l'origine
    sf::FloatRect rectDesc = description.getLocalBounds();
    description.setOrigin({rectDesc.size.x / 2.f, 0.f});

    // --- Configuration de l'aide (Contrôles) ---
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

    // --- Configuration de l'alerte de redimensionnement ---
    alerte.setString("Agrandir fenetre\npour plus d'infos");
    alerte.setCharacterSize(16);
    alerte.setStyle(sf::Text::Bold);
    
    sf::FloatRect rectAlerte = alerte.getLocalBounds();
    alerte.setOrigin({rectAlerte.size.x / 2.f, rectAlerte.size.y / 2.f});

    // --- Configuration des éléments graphiques des stats ---
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

/**
 * @brief Dessine l'ensemble de l'interface utilisateur (HUD).
 * Gère l'affichage adaptatif (Responsive Design) : selon la hauteur de la fenêtre,
 * certains éléments sont masqués pour prioriser les contrôles et les informations essentielles.
 * * @param monde Référence au monde pour récupérer les statistiques en temps réel.
 * @param cible Fenêtre de rendu SFML.
 * @param enPause État de la simulation (affiche le texte "PAUSE" si vrai).
 * @param fps Valeur des images par seconde.
 * @param tempsEcoule Temps total simulé.
 * @param survol Pointeur vers l'entité sous la souris (nullptr si aucune).
 */
void Rendu::menu(const Monde& monde, sf::RenderTarget& cible, bool enPause, float fps, float tempsEcoule, const Entite* survol) {
    // Calcul des coordonnées d'ancrage du menu
    sf::Vector2f tailleFenetre = cible.getView().getSize();
    sf::Vector2f centreFenetre = cible.getView().getCenter();

    float fenetreX = centreFenetre.x - tailleFenetre.x / 2.f;
    float fenetreY = centreFenetre.y - tailleFenetre.y / 2.f;
    float menuX = fenetreX + tailleFenetre.x - Constantes::MENU_LARGEUR;

    // --- Gestion des seuils d'affichage adaptatif ---
    
    // Niveau 1 : Affichage Standard (> 500px)
    // Affiche les contrôles et permet l'affichage de l'info-bulle flottante.
    bool afficherControles = tailleFenetre.y > 500.f;
    bool afficherTooltip = (survol != nullptr) && (tailleFenetre.y > 500.f);

    // Niveau 2 : Affichage Confort (> 750px)
    // Affiche la description textuelle pour combler l'espace vide.
    bool afficherDescription = tailleFenetre.y > 750.f;

    // Dessin du fond du panneau latéral
    fond.setPosition({menuX, fenetreY});
    fond.setSize({Constantes::MENU_LARGEUR, tailleFenetre.y});
    cible.draw(fond);

    // Affichage des FPS
    infoFps.setString("FPS : " + std::to_string(static_cast<int>(fps)));
    infoFps.setPosition({menuX + 20.f, fenetreY + 15.f});
    cible.draw(infoFps);

    // Formatage et affichage du Temps (mm:ss)
    int minutes = static_cast<int>(tempsEcoule) / 60;
    int secondes = static_cast<int>(tempsEcoule) % 60;
    std::ostringstream ss;
    ss << "Temps : " << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << secondes;
    
    infoTemps.setString(ss.str());
    sf::FloatRect rectTemps = infoTemps.getLocalBounds();
    infoTemps.setOrigin({rectTemps.size.x, 0.f}); // Alignement droite
    infoTemps.setPosition({menuX + 360.f, fenetreY + 15.f});
    cible.draw(infoTemps);

    // Titre
    titre.setPosition({menuX + 190.f, fenetreY + 45.f});
    cible.draw(titre);

    separateur.setPosition({menuX + 40.f, fenetreY + 90.f});
    cible.draw(separateur);

    // Affichage des statistiques de population
    Stats statistiques = monde.getStats();
    float debutStatsY = fenetreY + 130.f;
    dessinerLigneStat(cible, "Algues", statistiques.algues, Constantes::ALGUE_COULEUR, menuX, debutStatsY);
    dessinerLigneStat(cible, "Bacteries", statistiques.bacteries, Constantes::BACTERIE_COULEUR, menuX, debutStatsY + 50.f);
    dessinerLigneStat(cible, "Herbivores", statistiques.herbivores, Constantes::HERBIVORE_COULEUR_LENT, menuX, debutStatsY + 100.f);
    dessinerLigneStat(cible, "Carnivores", statistiques.carnivores, Constantes::CARNIVORE_COULEUR, menuX, debutStatsY + 150.f);

    // Affichage de la description (si l'espace le permet)
    if (afficherDescription) {
        description.setPosition({menuX + Constantes::MENU_LARGEUR / 2.f, fenetreY + 360.f});
        cible.draw(description);
    }

    // Affichage de l'indicateur de Pause
    if (enPause) {
        if (tailleFenetre.y > 350.f) {
            sf::FloatRect rectPause = pause.getLocalBounds();
            pause.setOrigin({rectPause.size.x / 2.f, 0.f});
            pause.setPosition({menuX + Constantes::MENU_LARGEUR / 2.f, fenetreY + 330.f});
            cible.draw(pause);
        }
    }

    // --- Gestion de l'Info-bulle Flottante (Tooltip) ---
    if (afficherTooltip) {
        // Identification du type et de la variante de l'entité survolée
        std::string typeStr = "Inconnu";
        std::string sousEspece = "";
        sf::Color typeCol = sf::Color::White;

        switch(survol->getType()) {
            case TypeEntite::ALGUE: typeStr = "Algue"; typeCol = Constantes::ALGUE_COULEUR; break;
            case TypeEntite::BACTERIE: typeStr = "Bacterie"; typeCol = Constantes::BACTERIE_COULEUR; break;
            
            case TypeEntite::HERBIVORE: {
                typeStr = "Herbivore"; 
                typeCol = Constantes::HERBIVORE_COULEUR_LENT;
                const auto* h = dynamic_cast<const Herbivore*>(survol);
                if (h) {
                    float vBase = Constantes::HERBIVORE_VITESSE_BASE;
                    float rBase = Constantes::HERBIVORE_RAYON_BASE;

                    // Détermination de la variante selon les stats
                    if (h->getRayon() > rBase * 1.3f) sousEspece = " (Colosse)";
                    else if (h->getRayon() < rBase * 0.8f && h->getVitesseMax() > vBase * 1.2f) sousEspece = " (Furtif)";
                    else if (h->getVitesseMax() > vBase * 1.1f) sousEspece = " (Rapide)";
                    else if (h->getRayon() < rBase * 0.9f) sousEspece = " (Petit)";
                    else sousEspece = " (Commun)";
                }
                break;
            }
            
            case TypeEntite::CARNIVORE: {
                typeStr = "Carnivore"; 
                typeCol = Constantes::CARNIVORE_COULEUR;
                const auto* c = dynamic_cast<const Carnivore*>(survol);
                if (c) {
                    float vBase = Constantes::CARNIVORE_VITESSE_BASE;
                    float rBase = Constantes::CARNIVORE_RAYON_BASE;

                    // Détermination de la variante selon les stats
                    if (c->getRayon() > rBase * 1.1f) sousEspece = " (Alpha)";
                    else if (c->getRayon() < rBase * 0.8f && c->getVitesseMax() > vBase * 1.2f) sousEspece = " (Traqueur)";
                    else if (c->getVitesseMax() > vBase * 1.1f) sousEspece = " (Chasseur)";
                    else if (c->getVitesseMax() < vBase * 0.9f) sousEspece = " (Charognard)";
                    else sousEspece = " (Commun)";
                }
                break;
            }
            
            case TypeEntite::ROCHER: typeStr = "Rocher"; typeCol = Constantes::ROCHER_COULEUR; break;
            default: break;
        }

        // Construction du texte de l'info-bulle
        std::string info = "ID: " + std::to_string(survol->getId()) + " | " + typeStr + sousEspece + "\n";
        
        if (survol->getType() == TypeEntite::ALGUE) {
            info += "Nutriment : +" + std::to_string((int)survol->getEnergie());
        } 
        else if (survol->getType() != TypeEntite::ROCHER) {
            info += "Energie : " + std::to_string((int)survol->getEnergie());
        }

        details.setString(info);
        details.setFillColor(typeCol);
        details.setCharacterSize(16); 

        // Calcul de la taille de la boîte englobante
        sf::FloatRect bounds = details.getGlobalBounds();
        sf::Vector2f boxSize(bounds.size.x + 10.f, bounds.size.y + 10.f);

        // Positionnement initial (au-dessus à droite de l'entité)
        sf::Vector2f posEntite = survol->getPosition();
        sf::Vector2f posTexte = posEntite + sf::Vector2f(20.f, -boxSize.y - 10.f);

        // --- Repositionnement intelligent (Anti-débordement) ---
        sf::View view = cible.getView();
        float viewRightEdge = view.getCenter().x + view.getSize().x / 2.f;
        float viewTopEdge = view.getCenter().y - view.getSize().y / 2.f;

        // Décalage vers la gauche si ça sort de l'écran à droite
        if (posTexte.x + boxSize.x > viewRightEdge) {
            posTexte.x = posEntite.x - boxSize.x - 20.f;
        }

        // Décalage vers le bas si ça sort de l'écran en haut
        if (posTexte.y < viewTopEdge) {
            posTexte.y = posEntite.y + 20.f;
        }

        // Dessin du fond semi-transparent puis du texte
        sf::RectangleShape fondInfo(boxSize);
        fondInfo.setFillColor(sf::Color(0, 0, 0, 200)); 
        fondInfo.setOutlineColor(typeCol);
        fondInfo.setOutlineThickness(1.f);
        fondInfo.setPosition({posTexte.x - 5.f, posTexte.y - 5.f});

        cible.draw(fondInfo);
        details.setPosition(posTexte);
        cible.draw(details);
    }

    // Pied de page : Affichage des contrôles ou Alerte de taille
    if (afficherControles) {
        controles.setPosition({menuX + 40.f, fenetreY + tailleFenetre.y - 120.f});
        cible.draw(controles);

        controlesGauche.setPosition({menuX + 40.f, fenetreY + tailleFenetre.y - 90.f});
        cible.draw(controlesGauche);

        controlesDroite.setPosition({menuX + 220.f, fenetreY + tailleFenetre.y - 90.f});
        cible.draw(controlesDroite);
    } else {
        // Animation clignotante pour l'alerte si la fenêtre est trop petite
        static sf::Clock clockClignote;
        float transparence = (std::sin(clockClignote.getElapsedTime().asSeconds() * 3.f) + 1.f) / 2.f;
        
        alerte.setFillColor(sf::Color(255, 100, 100, 150 + static_cast<std::uint8_t>(transparence * 105)));
        alerte.setPosition({menuX + 190.f, fenetreY + tailleFenetre.y - 40.f});
        cible.draw(alerte);
    }
}

/**
 * @brief Dessine une ligne de statistiques pour une espèce donnée.
 * Affiche l'icône, le nom, les données chiffrées (Vivants/Morts/Naissances)
 * et une jauge proportionnelle à la population actuelle.
 * * @param cible Fenêtre de rendu.
 * @param nomEspece Nom affiché de l'espèce (ex: "Algues").
 * @param stats Structure contenant les données.
 * @param couleur Couleur associée à l'espèce.
 * @param x Position X dans le menu.
 * @param y Position Y dans le menu.
 */
void Rendu::dessinerLigneStat(sf::RenderTarget& cible, const std::string& nomEspece, const StatsEspece& stats, sf::Color couleur, float x, float y) {
    // Icône de l'espèce
    iconeStat.setFillColor(couleur);
    iconeStat.setPosition({x + 40.f, y + 5.f});
    cible.draw(iconeStat);

    // Nom de l'espèce
    nomStat.setString(nomEspece);
    nomStat.setPosition({x + 70.f, y});
    cible.draw(nomStat);

    // Valeurs numériques
    std::string texteStats = "V: " + std::to_string(stats.vivants) + 
                           "  M: " + std::to_string(stats.morts) + 
                           "  N: " + std::to_string(stats.naissances);

    valeurStat.setString(texteStats);
    valeurStat.setFillColor(couleur);
    
    // Alignement à droite des valeurs
    sf::FloatRect rectValeur = valeurStat.getLocalBounds();
    valeurStat.setOrigin({rectValeur.size.x, 0.f});
    valeurStat.setPosition({x + 340.f, y});
    cible.draw(valeurStat);

    // Fond de la jauge
    fondJauge.setPosition({x + 40.f, y + 30.f});
    cible.draw(fondJauge);

    // Jauge remplie (Ratio limité à 1.0 pour éviter le débordement)
    float ratio = std::min(stats.vivants / 50.f, 1.f);
    jauge.setSize({300.f * ratio, 4.f});
    jauge.setFillColor(couleur);
    jauge.setPosition({x + 40.f, y + 30.f});
    cible.draw(jauge);
}