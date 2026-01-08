void initialiserMonde(Monde& monde, sf::Vector2f tailleFenetre) {
    monde.reset();

    int larg = std::max(1, (int)tailleFenetre.x);
    int haut = std::max(1, (int)tailleFenetre.y);

    for (int i = 0; i < 6; ++i) 
        monde.spawnRocher({static_cast<float>(rand() % larg), static_cast<float>(rand() % haut)});
    
    for (int i = 0; i < 50; ++i) 
        monde.spawnAlgue({static_cast<float>(rand() % larg), static_cast<float>(rand() % haut)});
    
    for (int i = 0; i < 5; ++i) 
        monde.spawnBacterie({static_cast<float>(rand() % larg), static_cast<float>(rand() % haut)});
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    sf::RenderWindow window(sf::VideoMode({1280u, 720u}), "Spore Simulation");
    window.setFramerateLimit(60);

    sf::Vector2f tailleInitiale(1280.f, 720.f);
    Monde monde(sf::FloatRect({0.f, 0.f}, tailleInitiale));    

    Rendu rendu;
    if (!rendu.init("arial.ttf")) {
        std::cerr << "Erreur: Police introuvable" << std::endl;
    }

    initialiserMonde(monde, tailleInitiale);

    sf::Clock clock;
    bool enPause = false;

    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        if (dt > 0.1f) dt = 0.1f;

        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) 
                window.close();

            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                sf::Vector2f nouvelleTaille(static_cast<float>(resized->size.x), static_cast<float>(resized->size.y));
                sf::FloatRect aireVisible({0.f, 0.f}, nouvelleTaille); 
                window.setView(sf::View(aireVisible));
                monde.setTaille(nouvelleTaille);
            }
            
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Escape)
                    window.close();
                
                if (keyPressed->code == sf::Keyboard::Key::P)
                    enPause = !enPause;
                
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    sf::Vector2f tailleActuelle(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
                    initialiserMonde(monde, tailleActuelle);
                    enPause = false; 
                }
            }

            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f clicPos = window.mapPixelToCoords(mouseEvent->position);
                float menuX = window.getView().getSize().x - 380.f;
                if (clicPos.x < menuX) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) 
                        monde.spawnAlgue(clicPos);
                    else if (mouseEvent->button == sf::Mouse::Button::Right) 
                        monde.spawnBacterie(clicPos);
                }
            }
        }

        if (!enPause) {
            monde.update(dt);
        }

        window.clear(sf::Color::Black);
        
        sf::RectangleShape zoneJeu(monde.getLimites().size);
        zoneJeu.setFillColor(sf::Color(10, 30, 50));
        zoneJeu.setOutlineColor(sf::Color::White);
        zoneJeu.setOutlineThickness(2.f);
        window.draw(zoneJeu);
        
        monde.dessinerFond(window);
        monde.dessiner(window);

        sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
        Entite* entiteSurvolee = monde.getEntiteSousSouris(worldPos);

        if (entiteSurvolee) {
            sf::CircleShape curseur(25.f);
            curseur.setOrigin({25.f, 25.f});
            curseur.setPosition(entiteSurvolee->getPosition());
            curseur.setFillColor(sf::Color::Transparent);
            curseur.setOutlineThickness(2.f);
            curseur.setOutlineColor(sf::Color::Yellow);
            window.draw(curseur);
        }
        
        rendu.menu(monde, window, enPause, entiteSurvolee);
        
        window.display();
    }
    return 0;
}