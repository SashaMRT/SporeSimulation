Carnivore::Carnivore(sf::Vector2f pos, float vitesse, float taille, float vue) 
    : Entite(pos, TypeEntite::CARNIVORE, taille, 150.f), 
    vitesseMax(vitesse),  porteeVue(vue)
{    
    float angle = (rand() % 360) * 3.14159f / 180.f;
    this->vitesse = sf::Vector2f(std::cos(angle), std::sin(angle)) * vitesseMax;
}

void Carnivore::update(float dt, Monde& monde) {
    Entite* ami = monde.getPlusProche(position, TypeEntite::CARNIVORE);
    float bonusMeute = 1.0f;

    if (ami) {
        float distAmi = std::hypot(ami->getPosition().x - position.x, ami->getPosition().y - position.y);
        if (distAmi > 0.1f && distAmi < 150.f) {
            bonusMeute = 1.3f;
        }
    }

    Entite* proie = monde.getPlusProche(position, TypeEntite::HERBIVORE);
    bool chasseEnCours = false;

    if (proie) {
        Herbivore* herbivore = dynamic_cast<Herbivore*>(proie);
        float dist = std::hypot(proie->getPosition().x - position.x, proie->getPosition().y - position.y);
        
        if (herbivore && !herbivore->estInvisible() && dist < porteeVue) {
            chasseEnCours = true;
            sf::Vector2f direction = proie->getPosition() - position;
            
            if (dist > 0.001f) {
                float vitesseActuelle = vitesseMax * bonusMeute; 
                vitesse = (direction / dist) * vitesseActuelle;
            }

            if (dist < rayon + proie->getRayon()) {
                energie += 80.f; 
                proie->tuer();
            }
        }
    }

    if (!chasseEnCours) {
        if (rand() % 100 < 2) {
             float angle = (rand() % 360) * 3.14159f / 180.f;
             vitesse = sf::Vector2f(std::cos(angle), std::sin(angle)) * (vitesseMax * 0.5f * (bonusMeute > 1.f ? 1.2f : 1.f));
        }
    }

    position += vitesse * dt;

    sf::FloatRect limites = monde.getLimites();

    if (position.x < limites.position.x) { 
        position.x = limites.position.x; 
        vitesse.x = std::abs(vitesse.x); 
    }

    if (position.x > limites.size.x) { 
        position.x = limites.size.x; 
        vitesse.x = -std::abs(vitesse.x); 
    }

    if (position.y < limites.position.y) { 
        position.y = limites.position.y; 
        vitesse.y = std::abs(vitesse.y); 
    }

    if (position.y > limites.size.y) { 
        position.y = limites.size.y; 
        vitesse.y = -std::abs(vitesse.y); 
    }

    energie -= (rayon * 0.15f + vitesseMax * 0.01f) * dt;
    if (energie <= 0) 
        tuer();
}

void Carnivore::dessiner(sf::RenderTarget& cible) const {
    float angleRad = 0.f;
    if (std::abs(vitesse.x) > 0.1f || std::abs(vitesse.y) > 0.1f) {
        angleRad = std::atan2(vitesse.y, vitesse.x);
    }
    float angleDeg = angleRad * 180.f / 3.14159f;

    std::uint8_t rouge = static_cast<std::uint8_t>(std::clamp(255.f - (rayon * 2.f), 100.f, 255.f));
    sf::Color couleurPeau(rouge, 40, 40);
    sf::Color couleurContour = (vitesseMax > 130.f) ? sf::Color(255, 215, 0) : sf::Color(150, 20, 20);

    for (int i = 3; i >= 1; --i) {
        float tailleQueue = rayon * (0.7f - i * 0.1f);
        
        sf::ConvexShape queue(3);
        queue.setPoint(0, {tailleQueue * 1.5f, 0});
        queue.setPoint(1, {-tailleQueue, -tailleQueue * 0.7f});
        queue.setPoint(2, {-tailleQueue, tailleQueue * 0.7f});

        float oscillation = std::sin((position.x + position.y) * 0.1f + i) * 15.f;
        
        sf::Vector2f posQueue;
        posQueue.x = position.x - std::cos(angleRad) * (i * rayon * 0.7f);
        posQueue.y = position.y - std::sin(angleRad) * (i * rayon * 0.7f);

        queue.setPosition(posQueue);
        queue.setRotation(sf::degrees(angleDeg + oscillation));
        queue.setFillColor(sf::Color(rouge - 20, 30, 30));
        cible.draw(queue);
    }

    sf::ConvexShape corps(4);
    corps.setPoint(0, {rayon * 1.8f, 0}); 
    corps.setPoint(1, {0, -rayon});      
    corps.setPoint(2, {-rayon * 0.8f, 0});
    corps.setPoint(3, {0, rayon});        
    
    corps.setPosition(position);
    corps.setRotation(sf::degrees(angleDeg));
    corps.setFillColor(couleurPeau);
    
    corps.setOutlineThickness(2.f);
    corps.setOutlineColor(couleurContour);
    cible.draw(corps);

    sf::CircleShape oeil(rayon * 0.25f);
    oeil.setOrigin({oeil.getRadius(), oeil.getRadius()});
    
    sf::Vector2f posOeil;
    posOeil.x = position.x + std::cos(angleRad) * (rayon * 0.5f);
    posOeil.y = position.y + std::sin(angleRad) * (rayon * 0.5f);
    
    oeil.setPosition(posOeil);
    oeil.setFillColor((vitesseMax > 130.f) ? sf::Color::Red : sf::Color::Yellow);
    cible.draw(oeil);
}