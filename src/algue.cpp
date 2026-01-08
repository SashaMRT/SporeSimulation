Algue::Algue(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::ALGUE, 12.f, 200.f) {
}

void Algue::update(float dt, Monde& monde) {
}

void Algue::dessiner(sf::RenderTarget& cible) const {
    static sf::Clock horloge;
    float temps = horloge.getElapsedTime().asSeconds();
    const int nbSegments = 10;
    const float epaisseurBase = 4.f;
    const float hauteurSegment = 4.f;
    
    for (int i = 0; i < nbSegments; ++i) {
        float ratio = static_cast<float>(i) / nbSegments;
        float taille = epaisseurBase * (1.0f - ratio * 0.7f);
        float oscillation = std::sin(temps * 1.5f + i * 0.5f) * (i * 0.6f);
        
        sf::CircleShape segment(taille);
        segment.setOrigin(sf::Vector2f(taille, taille));
        segment.setFillColor(sf::Color(34 + i*5, 120 + i*8, 50 + i*2, 255 - i*15));
        segment.setPosition(position + sf::Vector2f(oscillation, -i * hauteurSegment));
        cible.draw(segment);
    }
}
