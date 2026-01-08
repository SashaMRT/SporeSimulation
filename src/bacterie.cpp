Bacterie::Bacterie(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::BACTERIE, Constantes::BACTERIE_RAYON, Constantes::BACTERIE_ENERGIE) {
}

void Bacterie::update(float dt, Monde& monde) {
    Entite* cible = monde.getPlusProche(position, TypeEntite::ALGUE);
    
    if (cible) {
        sf::Vector2f diff = cible->getPosition() - position;
        float dist = std::hypot(diff.x, diff.y);
        
        if (dist > 0.001f) {
            sf::Vector2f dir = diff / dist;
            position += dir * 40.f * dt; 
        }

        if (dist < rayon + cible->getRayon()) {
            // CORRECTION : On utilise la constante (50) au lieu de 15
            energie += Constantes::ALGUE_ENERGIE; 
            cible->tuer();
        }
    }
    
    energie -= 5.f * dt;
    if (energie <= 0) tuer();
}

void Bacterie::dessiner(sf::RenderTarget& cible) const {
    sf::CircleShape membrane(rayon);
    membrane.setOrigin({rayon, rayon});
    membrane.setPosition(position);
    membrane.setFillColor(sf::Color(0, 255, 255, 40));
    membrane.setOutlineThickness(2.f);
    membrane.setOutlineColor(sf::Color(255, 255, 255, 150));
    cible.draw(membrane);

    sf::CircleShape patte(rayon * 0.15f);
    patte.setFillColor(sf::Color(200, 255, 255, 180));
    patte.setOrigin({rayon * 0.15f, rayon * 0.15f});
    for (int i = 0; i < 8; ++i) {
        float a = i * 45.f * 3.14159f / 180.f;
        patte.setPosition(position + sf::Vector2f(std::cos(a), std::sin(a)) * rayon);
        cible.draw(patte);
    }

    sf::CircleShape noyauexterne(rayon);
    noyauexterne.setOutlineThickness(0);
    noyauexterne.setRadius(rayon * 0.4f);
    noyauexterne.setOrigin({rayon * 0.4f, rayon * 0.4f});
    noyauexterne.setFillColor(sf::Color(0, 180, 255));
    cible.draw(noyauexterne);

    sf::CircleShape noyauinterne(rayon);
    noyauinterne.setRadius(rayon * 0.2f);
    noyauinterne.setOrigin({rayon * 0.2f, rayon * 0.2f});
    noyauinterne.setFillColor(sf::Color(255, 255, 255, 200));
    noyauinterne.setPosition(position - sf::Vector2f(rayon * 0.1f, rayon * 0.1f));
    cible.draw(noyauinterne);
}