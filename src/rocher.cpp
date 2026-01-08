Rocher::Rocher(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::ROCHER, 
             Constantes::ROCHER_RAYON_BASE + (rand() % 20), 
             Constantes::ROCHER_ENERGIE), 
      occupe(false) {}

void Rocher::update(float dt, Monde& monde) {
}

void Rocher::dessiner(sf::RenderTarget& cible) const {
    sf::ConvexShape forme(6);
    forme.setPoint(0, {rayon, 0.f});
    forme.setPoint(1, {rayon * 0.8f, rayon * 0.7f});
    forme.setPoint(2, {-rayon * 0.5f, rayon});
    forme.setPoint(3, {-rayon, 0.2f});
    forme.setPoint(4, {-rayon * 0.7f, -rayon * 0.8f});
    forme.setPoint(5, {0.3f, -rayon});

    forme.setPosition(position);
    
    if (occupe) {
        forme.setFillColor(sf::Color(40, 40, 45));
    } else {
        forme.setFillColor(sf::Color(80, 80, 90));
    }
    
    forme.setOutlineThickness(2.f);
    forme.setOutlineColor(sf::Color(50, 50, 55));
    
    cible.draw(forme);

    if (!occupe) {
        sf::CircleShape reflet(rayon * 0.3f);
        reflet.setPosition(position - sf::Vector2f(rayon*0.4f, rayon*0.4f));
        reflet.setFillColor(sf::Color(120, 120, 130, 100));
        cible.draw(reflet);
    }
}