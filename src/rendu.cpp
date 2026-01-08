Rendu::Rendu() : titre(font) {
    fond.setSize({380.f, 2000.f});
    fond.setFillColor(sf::Color(25, 25, 30, 230));
    fond.setOutlineThickness(-2.f);
    fond.setOutlineColor(sf::Color(100, 100, 100));
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
            return true;
        }
    }
    return false;
}

void Rendu::menu(const Monde& monde, sf::RenderTarget& cible, bool enPause, const Entite* survol) {
    sf::Vector2f tailleFenetre = cible.getView().getSize();
    sf::Vector2f centreFenetre = cible.getView().getCenter();

    float fenetreX = centreFenetre.x - tailleFenetre.x / 2.f;
    float fenetreY = centreFenetre.y - tailleFenetre.y / 2.f;

    float menuX = fenetreX + tailleFenetre.x - 380.f;

    bool afficherControles = tailleFenetre.y > 600.f; 
    bool afficherDescription = tailleFenetre.y > 700.f; 

    fond.setPosition({menuX, fenetreY});
    fond.setSize({380.f, tailleFenetre.y});
    cible.draw(fond);

    titre.setPosition({menuX + 190.f, fenetreY + 30.f});
    cible.draw(titre);

    sf::RectangleShape separateur({300.f, 2.f});
    separateur.setFillColor(sf::Color(100, 100, 100));
    separateur.setPosition({menuX + 40.f, fenetreY + 80.f});
    cible.draw(separateur);

    Stats statistiques = monde.getStats();
    dessinerLigneStat(cible, "Algues", statistiques.nbAlgues, sf::Color(50, 255, 50), menuX, fenetreY + 120.f);
    dessinerLigneStat(cible, "Bacteries", statistiques.nbBacteries, sf::Color(0, 255, 255), menuX, fenetreY + 170.f);
    dessinerLigneStat(cible, "Herbivores", statistiques.nbHerbivores, sf::Color(150, 255, 100), menuX, fenetreY + 220.f);
    dessinerLigneStat(cible, "Carnivores", statistiques.nbCarnivores, sf::Color(255, 50, 50), menuX, fenetreY + 270.f);

    if (survol != nullptr) {
        if (tailleFenetre.y > 350.f) {
            sf::Text infos(font, "--- INSPECTION ---", 18);
            infos.setFillColor(sf::Color::Yellow);
            infos.setPosition({menuX + 40.f, fenetreY + 340.f});
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
            details.setPosition({menuX + 40.f, fenetreY + 370.f});
            cible.draw(details);
        }
    }
    else if (enPause) {
        if (tailleFenetre.y > 350.f) {
            sf::Text pause(font, "--- PAUSE ---", 20);
            pause.setFillColor(sf::Color::Yellow); 
            pause.setStyle(sf::Text::Bold);
            pause.setPosition({menuX + 40.f, fenetreY + 350.f});
            cible.draw(pause);
        }
    } 
    else if (afficherDescription) {
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
        description.setPosition({menuX + 40.f, fenetreY + 340.f});
        cible.draw(description);
    }

    if (afficherControles) {
        sf::Text Controles(font, "Controles :", 16);
        Controles.setFillColor(sf::Color(200, 200, 200));
        Controles.setStyle(sf::Text::Bold);
        Controles.setPosition({menuX + 40.f, fenetreY + tailleFenetre.y - 120.f});
        cible.draw(Controles);

        sf::Text colGauche(font);
        colGauche.setString(
            "Survoler : Infos\n"
            "Clic Gauche : + Algue\n"
            "Clic Droit : + Bacterie"
        );
        colGauche.setCharacterSize(14);
        colGauche.setFillColor(sf::Color(150, 150, 150));
        colGauche.setPosition({menuX + 40.f, fenetreY + tailleFenetre.y - 90.f});
        cible.draw(colGauche);

        sf::Text colDroite(font);
        colDroite.setString(
            "P : Pause\n"
            "R : Restart\n"
            "Echap : Quitter"
        );
        colDroite.setCharacterSize(14);
        colDroite.setFillColor(sf::Color(150, 150, 150));
        colDroite.setPosition({menuX + 220.f, fenetreY + tailleFenetre.y - 90.f});
        cible.draw(colDroite);
    } else {
        static sf::Clock clockClignote;
        float transparence = (std::sin(clockClignote.getElapsedTime().asSeconds() * 3.f) + 1.f) / 2.f;
        
        sf::Text alerte(font, "Agrandir fenetre\npour plus d'infos", 16);
        alerte.setFillColor(sf::Color(255, 100, 100, 150 + static_cast<std::uint8_t>(transparence * 105)));
        alerte.setStyle(sf::Text::Bold);
        
        sf::FloatRect rectangle = alerte.getLocalBounds();
        alerte.setOrigin({rectangle.size.x / 2.f, rectangle.size.y / 2.f});
        alerte.setPosition({menuX + 190.f, fenetreY + tailleFenetre.y - 40.f});
        
        cible.draw(alerte);
    }
}

void Rendu::dessinerLigneStat(sf::RenderTarget& cible, const std::string& label, int quantite, sf::Color couleur, float x, float y) const {
    sf::CircleShape icone(8.f);
    icone.setFillColor(couleur);
    icone.setOutlineThickness(1.f);
    icone.setOutlineColor(sf::Color::White);
    icone.setPosition({x + 40.f, y + 5.f});
    cible.draw(icone);

    sf::Text nom(font);
    nom.setString(label);
    nom.setCharacterSize(20);
    nom.setFillColor(sf::Color(220, 220, 220));
    nom.setPosition({x + 70.f, y});
    cible.draw(nom);

    sf::Text valeur(font);
    valeur.setString(std::to_string(quantite));
    valeur.setCharacterSize(20);
    valeur.setStyle(sf::Text::Bold);
    valeur.setFillColor(couleur);
    
    sf::FloatRect rectValeur = valeur.getLocalBounds();
    valeur.setOrigin({rectValeur.size.x, 0.f});
    valeur.setPosition({x + 340.f, y});
    cible.draw(valeur);

    sf::RectangleShape fondJauge({300.f, 4.f});
    fondJauge.setFillColor(sf::Color(50, 50, 50));
    fondJauge.setPosition({x + 40.f, y + 30.f});
    cible.draw(fondJauge);

    float ratio = std::min(quantite / 50.f, 1.f);
    sf::RectangleShape jauge({300.f * ratio, 4.f});
    jauge.setFillColor(couleur);
    jauge.setPosition({x + 40.f, y + 30.f});
    cible.draw(jauge);
}