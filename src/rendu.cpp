/**
 * @file rendu.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Rendu.
 * @details Gère toute la partie graphique de l'interface utilisateur (HUD), 
 * l'affichage des menus, des statistiques, et des informations de débogage 
 * (inspection des entités).
 */

/**
 * @brief Constructeur de la classe Rendu.
 * * Initialise les références aux polices pour tous les objets texte 
 * et configure le style de base du fond du menu latéral.
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
    // Configuration du panneau latéral (fond gris foncé avec bordure)
    fond.setFillColor(Constantes::COULEUR_FOND_MENU);
    fond.setOutlineThickness(-2.f);
    fond.setOutlineColor(sf::Color(100, 100, 100));
}

/**
 * @brief Initialise les ressources graphiques.
 * * Charge la police d'écriture depuis plusieurs chemins potentiels (pour la portabilité).
 * Configure ensuite la taille, la couleur et le contenu par défaut de tous les textes statiques.
 * * @param cheminFont Nom du fichier de police (ex: "arial.ttf").
 * @return true si la police a été chargée avec succès, false sinon.
 */
bool Rendu::init(const std::string& cheminFont) {
    // Liste des chemins relatifs possibles pour trouver le fichier de police
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
    titre.setString("SPORE SIM");
    titre.setCharacterSize(28);
    titre.setFillColor(Constantes::COULEUR_TITRE);
    titre.setStyle(sf::Text::Bold);
    
    // Centrage de l'origine du titre pour faciliter son positionnement
    sf::FloatRect limitesTitre = titre.getLocalBounds();
    titre.setOrigin({limitesTitre.size.x / 2.f, 0.f});

    // Ligne séparatrice sous le titre
    separateur.setSize({300.f, 2.f});
    separateur.setFillColor(sf::Color(100, 100, 100));

    // --- Configuration des infos techniques (FPS, Temps) ---
    infoFps.setCharacterSize(14);
    infoFps.setFillColor(sf::Color(180, 180, 180));

    infoTemps.setCharacterSize(14);
    infoTemps.setFillColor(sf::Color(180, 180, 180));

    // --- Configuration de l'Inspecteur (Survol souris) ---
    inspection.setString("--- INSPECTION ---");
    inspection.setCharacterSize(18);
    inspection.setFillColor(sf::Color::Yellow);

    details.setCharacterSize(16);

    // --- Configuration de l'écran de Pause ---
    pause.setString("--- PAUSE ---");
    pause.setCharacterSize(20);
    pause.setFillColor(sf::Color::Yellow);
    pause.setStyle(sf::Text::Bold);

    // Texte descriptif du projet
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

    // Message d'alerte si la fenêtre est trop petite
    alerte.setString("Agrandir fenetre\npour plus d'infos");
    alerte.setCharacterSize(16);
    alerte.setStyle(sf::Text::Bold);
    
    sf::FloatRect rectAlerte = alerte.getLocalBounds();
    alerte.setOrigin({rectAlerte.size.x / 2.f, rectAlerte.size.y / 2.f});

    // --- Configuration des éléments graphiques des stats (Jauges/Icônes) ---
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
 * @brief Dessine le menu latéral complet (HUD).
 * * Gère le positionnement dynamique des éléments en fonction de la taille de la fenêtre.
 * Affiche les FPS, le temps, les statistiques de population, et selon le contexte :
 * les détails de l'entité survolée, l'état de pause, ou les contrôles.
 * * @param monde Référence au monde pour récupérer les statistiques.
 * @param cible Fenêtre de rendu.
 * @param enPause Indique si la simulation est en pause.
 * @param fps Nombre d'images par seconde actuel.
 * @param tempsEcoule Temps total de simulation écoulé.
 * @param survol Pointeur vers l'entité sous la souris (peut être nullptr).
 */
void Rendu::menu(const Monde& monde, sf::RenderTarget& cible, bool enPause, float fps, float tempsEcoule, const Entite* survol) {
    // Récupération des dimensions actuelles de la vue pour ancrer le menu à droite
    sf::Vector2f tailleFenetre = cible.getView().getSize();
    sf::Vector2f centreFenetre = cible.getView().getCenter();

    float fenetreX = centreFenetre.x - tailleFenetre.x / 2.f;
    float fenetreY = centreFenetre.y - tailleFenetre.y / 2.f;
    float menuX = fenetreX + tailleFenetre.x - Constantes::MENU_LARGEUR;

    // Logique d'affichage responsive : on cache certains éléments si la fenêtre est trop petite
    bool afficherControles = tailleFenetre.y > 600.f;
    bool afficherDescription = tailleFenetre.y > 700.f;

    // Dessin du fond du panneau
    fond.setPosition({menuX, fenetreY});
    fond.setSize({Constantes::MENU_LARGEUR, tailleFenetre.y});
    cible.draw(fond);

    // Affichage FPS et Temps
    infoFps.setString("FPS : " + std::to_string(static_cast<int>(fps)));
    infoFps.setPosition({menuX + 20.f, fenetreY + 15.f});
    cible.draw(infoFps);

    int minutes = static_cast<int>(tempsEcoule) / 60;
    int secondes = static_cast<int>(tempsEcoule) % 60;
    std::ostringstream ss;
    ss << "Temps : " << std::setw(2) << std::setfill('0') << minutes << ":" << std::setw(2) << std::setfill('0') << secondes;
    
    infoTemps.setString(ss.str());
    // Alignement à droite du temps
    sf::FloatRect rectTemps = infoTemps.getLocalBounds();
    infoTemps.setOrigin({rectTemps.size.x, 0.f});
    infoTemps.setPosition({menuX + 360.f, fenetreY + 15.f});
    cible.draw(infoTemps);

    // Titre et séparateur
    titre.setPosition({menuX + 190.f, fenetreY + 45.f});
    cible.draw(titre);

    separateur.setPosition({menuX + 40.f, fenetreY + 90.f});
    cible.draw(separateur);

    // Affichage des Statistiques (Appel aux helpers)
    Stats statistiques = monde.getStats();
    float debutStatsY = fenetreY + 130.f;
    dessinerLigneStat(cible, "Algues", statistiques.algues, Constantes::ALGUE_COULEUR, menuX, debutStatsY);
    dessinerLigneStat(cible, "Bacteries", statistiques.bacteries, Constantes::BACTERIE_COULEUR, menuX, debutStatsY + 50.f);
    dessinerLigneStat(cible, "Herbivores", statistiques.herbivores, Constantes::HERBIVORE_COULEUR_LENT, menuX, debutStatsY + 100.f);
    dessinerLigneStat(cible, "Carnivores", statistiques.carnivores, Constantes::CARNIVORE_COULEUR, menuX, debutStatsY + 150.f);

    // Zone Contextuelle (Inspection > Pause > Description)
    if (survol != nullptr) {
        // Mode Inspection : Affiche les détails de l'entité sous la souris
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
        // Mode Pause
        if (tailleFenetre.y > 350.f) {
            pause.setPosition({menuX + 40.f, fenetreY + 360.f});
            cible.draw(pause);
        }
    }
    else if (afficherDescription) {
        // Mode par défaut : Description du jeu
        description.setPosition({menuX + 40.f, fenetreY + 350.f});
        cible.draw(description);
    }

    // Pied de page (Contrôles ou Alerte taille)
    if (afficherControles) {
        controles.setPosition({menuX + 40.f, fenetreY + tailleFenetre.y - 120.f});
        cible.draw(controles);

        controlesGauche.setPosition({menuX + 40.f, fenetreY + tailleFenetre.y - 90.f});
        cible.draw(controlesGauche);

        controlesDroite.setPosition({menuX + 220.f, fenetreY + tailleFenetre.y - 90.f});
        cible.draw(controlesDroite);
    } else {
        // Animation clignotante pour l'alerte
        static sf::Clock clockClignote;
        float transparence = (std::sin(clockClignote.getElapsedTime().asSeconds() * 3.f) + 1.f) / 2.f;
        
        alerte.setFillColor(sf::Color(255, 100, 100, 150 + static_cast<std::uint8_t>(transparence * 105)));
        alerte.setPosition({menuX + 190.f, fenetreY + tailleFenetre.y - 40.f});
        cible.draw(alerte);
    }
}

/**
 * @brief Méthode utilitaire pour dessiner une ligne de statistique.
 * * Affiche une icône colorée, le nom de l'espèce, les chiffres clés (Vivants/Morts/Naissances)
 * et une barre de progression visuelle.
 * * @param cible Fenêtre de rendu.
 * @param nomEspece Nom de l'espèce (ex: "Algues").
 * @param stats Structure contenant les données chiffrées.
 * @param couleur Couleur associée à l'espèce.
 * @param x Position X de départ.
 * @param y Position Y de départ.
 */
void Rendu::dessinerLigneStat(sf::RenderTarget& cible, const std::string& nomEspece, const StatsEspece& stats, sf::Color couleur, float x, float y) {
    // Dessin de l'icône (petit cercle)
    iconeStat.setFillColor(couleur);
    iconeStat.setPosition({x + 40.f, y + 5.f});
    cible.draw(iconeStat);

    // Dessin du nom
    nomStat.setString(nomEspece);
    nomStat.setPosition({x + 70.f, y});
    cible.draw(nomStat);

    // Formatage et dessin des valeurs numériques
    std::string texteStats = "V: " + std::to_string(stats.vivants) + 
                           "  M: " + std::to_string(stats.morts) + 
                           "  N: " + std::to_string(stats.naissances);

    valeurStat.setString(texteStats);
    valeurStat.setFillColor(couleur);
    
    // Alignement à droite
    sf::FloatRect rectValeur = valeurStat.getLocalBounds();
    valeurStat.setOrigin({rectValeur.size.x, 0.f});
    valeurStat.setPosition({x + 340.f, y});
    cible.draw(valeurStat);

    // Dessin du fond de la jauge (gris sombre)
    fondJauge.setPosition({x + 40.f, y + 30.f});
    cible.draw(fondJauge);

    // Calcul et dessin de la jauge remplie
    // (Plafonnée à 50 unités pour l'échelle visuelle)
    float ratio = std::min(stats.vivants / 50.f, 1.f);
    jauge.setSize({300.f * ratio, 4.f});
    jauge.setFillColor(couleur);
    jauge.setPosition({x + 40.f, y + 30.f});
    cible.draw(jauge);
}