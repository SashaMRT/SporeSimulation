/**
 * @file main.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Point d'entrée de l'application Spore Simulation.
 * @details Contient la boucle principale du jeu (Game Loop), la gestion des événements
 * (clavier, souris, fenêtre) et l'orchestration entre le modèle (Monde) et la vue (Rendu).
 */

/**
 * @brief Initialise ou réinitialise le monde avec une population de départ.
 * * Vide le monde de ses entités actuelles puis génère aléatoirement
 * des rochers, des algues et des bactéries selon les constantes définies.
 * * @param monde Référence vers l'instance du monde à peupler.
 * @param tailleFenetre Dimensions de la fenêtre pour répartir les entités.
 */
void initialiserMonde(Monde& monde, sf::Vector2f tailleFenetre) {
    monde.reset(); // Nettoyage complet du monde

    // Sécurité pour éviter des modulos par 0 si la fenêtre est trop petite
    int larg = std::max(1, (int)tailleFenetre.x);
    int haut = std::max(1, (int)tailleFenetre.y);

    // Génération des Rochers (Obstacles et cachettes)
    for (int i = 0; i < Constantes::NB_ROCHERS_INIT; ++i) 
        monde.spawnRocher({static_cast<float>(rand() % larg), static_cast<float>(rand() % haut)});
    
    // Génération des Algues (Nourriture de base)
    for (int i = 0; i < Constantes::NB_ALGUES_INIT; ++i) 
        monde.spawnAlgue({static_cast<float>(rand() % larg), static_cast<float>(rand() % haut)});
    
    // Génération des Bactéries (Première forme de vie)
    for (int i = 0; i < Constantes::NB_BACTERIES_INIT; ++i) 
        monde.spawnBacterie({static_cast<float>(rand() % larg), static_cast<float>(rand() % haut)});
}

/**
 * @brief Fonction principale du programme.
 * * Configure la fenêtre SFML, initialise les systèmes et lance la boucle de jeu.
 * @return 0 si le programme se termine correctement.
 */
int main() {
    // Initialisation de la graine aléatoire basée sur le temps actuel
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Création de la fenêtre principale
    sf::RenderWindow window(sf::VideoMode({
        static_cast<unsigned int>(Constantes::FENETRE_LARGEUR), 
        static_cast<unsigned int>(Constantes::FENETRE_HAUTEUR)
    }), "Spore Simulation");
    
    // Limite de framerate pour éviter une surconsommation CPU/GPU
    window.setFramerateLimit(Constantes::FPS_LIMITE);

    // Création du monde physique
    sf::Vector2f tailleInitiale(Constantes::FENETRE_LARGEUR, Constantes::FENETRE_HAUTEUR);
    Monde monde(sf::FloatRect({0.f, 0.f}, tailleInitiale));    

    // Initialisation du système de rendu (HUD)
    Rendu rendu;
    if (!rendu.init("arial.ttf")) {
        std::cerr << "Erreur: Police introuvable" << std::endl;
        // Le programme continue mais sans texte affiché correctement
    }

    // Premier peuplement du monde
    initialiserMonde(monde, tailleInitiale);

    sf::Clock clock;          // Horloge pour le Delta Time (dt)
    sf::Clock horlogeGlobale; // Horloge pour le temps total de simulation
    bool enPause = false;     // État de pause

    // --- BOUCLE PRINCIPALE (Game Loop) ---
    while (window.isOpen()) {
        // Calcul du temps écoulé depuis la dernière frame
        float dt = clock.restart().asSeconds();

        // Calcul des FPS pour l'affichage
        float fps = (dt > 0.f) ? 1.0f / dt : 0.f;
        float tempsTotal = horlogeGlobale.getElapsedTime().asSeconds();

        // Plafonnement du dt pour éviter les bugs physiques en cas de lag
        if (dt > 0.1f) dt = 0.1f;

        // --- GESTION DES ÉVÉNEMENTS ---
        while (const auto event = window.pollEvent()) {
            // Fermeture de la fenêtre (croix)
            if (event->is<sf::Event::Closed>()) 
                window.close();

            // Redimensionnement de la fenêtre
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                sf::Vector2f nouvelleTaille(static_cast<float>(resized->size.x), static_cast<float>(resized->size.y));
                
                // Mise à jour de la vue pour garder l'échelle 1:1
                sf::FloatRect aireVisible({0.f, 0.f}, nouvelleTaille); 
                window.setView(sf::View(aireVisible));
                
                // On informe le monde de sa nouvelle taille
                monde.setTaille(nouvelleTaille);
            }
            
            // Gestion du clavier
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                    window.close(); // Echap pour quitter
                
                if (keyPressed->code == sf::Keyboard::Key::P)
                    enPause = !enPause; // P pour Pause
                
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    // R pour Reset : on relance la simulation
                    sf::Vector2f tailleActuelle(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
                    initialiserMonde(monde, tailleActuelle);
                    enPause = false; 
                    horlogeGlobale.restart();
                }
            }

            // Gestion de la souris (Ajout manuel d'entités)
            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f clicPos = window.mapPixelToCoords(mouseEvent->position);
                
                // Vérification pour ne pas cliquer "sous" le menu latéral
                float menuX = window.getView().getSize().x - Constantes::MENU_LARGEUR;
                if (clicPos.x < menuX) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) 
                        monde.spawnAlgue(clicPos);     // Clic Gauche : Algue
                    else if (mouseEvent->button == sf::Mouse::Button::Right) 
                        monde.spawnBacterie(clicPos);  // Clic Droit : Bactérie
                }
            }
        }

        // --- MISE À JOUR (UPDATE) ---
        if (!enPause) {
            monde.update(dt);
        }

        // --- RENDU (DRAW) ---
        window.clear(sf::Color::Black); // Effacer l'écran
        
        // Dessin de la bordure de la zone de jeu
        sf::RectangleShape zoneJeu(monde.getLimites().size);
        zoneJeu.setFillColor(Constantes::COULEUR_ZONE_JEU);
        zoneJeu.setOutlineColor(sf::Color::White);
        zoneJeu.setOutlineThickness(2.f);
        window.draw(zoneJeu);
        
        // Dessin du fond et des entités
        monde.dessinerFond(window);
        monde.dessiner(window);

        // Gestion du curseur de sélection (survol)
        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        Entite* entiteSurvolee = monde.getEntiteSousSouris(worldPos);

        if (entiteSurvolee) {
            // Affichage d'un cercle jaune autour de l'entité survolée
            sf::CircleShape curseur(25.f);
            curseur.setOrigin({25.f, 25.f});
            curseur.setPosition(entiteSurvolee->getPosition());
            curseur.setFillColor(sf::Color::Transparent);
            curseur.setOutlineThickness(2.f);
            curseur.setOutlineColor(sf::Color::Yellow);
            window.draw(curseur);
        }
        
        // Affichage de l'interface utilisateur (HUD)
        rendu.menu(monde, window, enPause, fps, tempsTotal, entiteSurvolee);
        
        window.display(); // Affichage final
    }
    return 0;
}