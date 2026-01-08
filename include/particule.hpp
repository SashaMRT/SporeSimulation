class Particule {
public:
    sf::Vector2f position;
    sf::Vector2f vitesse;
    sf::Color couleur;
    float vie;
    float tailleInitiale;
    float angle;
    float rotation;

    Particule(sf::Vector2f pos, sf::Vector2f vit, sf::Color col)
        : position(pos), vitesse(vit), couleur(col), vie(1.0f),
          tailleInitiale(2.f + (rand() % 100) / 100.f * 3.f),
          angle((rand() % 360) * 3.14159f / 180.f),
          rotation((rand() % 100) / 100.f * 180.f - 90.f) {}

    void update(float dt) {
        position += vitesse * dt;
        vitesse *= 0.98f; // Légère friction
        angle += rotation * dt;
        vie -= dt * 0.8f; // Vie plus longue
    }

    void dessiner(sf::RenderTarget& cible) const {
        if (vie <= 0) return;
        
        float taille = tailleInitiale * (0.5f + vie * 0.5f); // Rétrécit avec la vie
        
        sf::RectangleShape forme({taille, taille});
        forme.setOrigin({taille / 2.f, taille / 2.f});
        forme.setPosition(position);
        forme.setRotation(sf::degrees(angle));
        
        sf::Color c = couleur;
        c.a = static_cast<std::uint8_t>(std::max(0.f, vie) * 200); // Transparence
        
        forme.setFillColor(c);
        cible.draw(forme);
    }
};