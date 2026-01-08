#include "herbivore.hpp"
#include "monde.hpp"
#include <cmath>
#include <cstdint>

Herbivore::Herbivore(sf::Vector2f pos, float vitesse, float taille, float vue) 
    : Entite(pos, TypeEntite::HERBIVORE, taille, 100.f),
      vitesseMax(vitesse), porteeVue(vue), invisible(false), 
      chronoCache(0.f), chronoRecharge(0.f), isRapide(vitesse > 120.f) 
{    
    float angle = (rand() % 360) * 3.14159f / 180.f;
    this->vitesse = sf::Vector2f(std::cos(angle), std::sin(angle)) * vitesseMax;
}

void Herbivore::update(float dt, Monde& monde) {
    if (chronoRecharge > 0.f) chronoRecharge -= dt;

    if (invisible) {
        chronoCache += dt;
        if (chronoCache >= 10.f) {
            invisible = false;
            chronoCache = 0.f;
            chronoRecharge = 20.f;
            monde.libererRocherProche(position);
        }
        return;
    }

    Entite* danger = monde.getPlusProche(position, TypeEntite::CARNIVORE);
    bool dangerProche = false;
    
    if (danger) {
        float distDanger = std::hypot(danger->getPosition().x - position.x, 
                                      danger->getPosition().y - position.y);
        dangerProche = (distDanger < porteeVue * 0.7f);
    }

    if (chronoRecharge <= 0.f && dangerProche && monde.estCache(position)) {
        invisible = true;
        return;
    }

    Entite* ami = monde.getPlusProche(position, TypeEntite::HERBIVORE);
    float vueEffective = porteeVue;
    
    if (ami) {
        float distAmi = std::hypot(ami->getPosition().x - position.x, ami->getPosition().y - position.y);
        if (distAmi > 0.1f && distAmi < 100.f) {
            vueEffective *= 2.0f;
        }
    }

    Entite* nourriture = monde.getPlusProche(position, TypeEntite::ALGUE);

    bool enFuite = false;

    if (danger && dangerProche) {
        sf::Vector2f fuite = position - danger->getPosition();
        float distDanger = std::hypot(fuite.x, fuite.y);

        if (distDanger < vueEffective) {
            if (distDanger > 0.001f) {
                sf::FloatRect limites = monde.getLimites();
                sf::Vector2f centreMonde = {limites.size.x / 2.f, limites.size.y / 2.f};
                sf::Vector2f versCentre = centreMonde - position;
                float distVersCentre = std::hypot(versCentre.x, versCentre.y);
                
                if (distVersCentre > 0.001f) {
                    versCentre /= distVersCentre;
                    
                    float angleFuite = std::atan2(fuite.y, fuite.x);
                    float angleVersCentre = std::atan2(versCentre.y, versCentre.x);
                    
                    float differenceAngle = angleVersCentre - angleFuite;
                    while (differenceAngle > 3.14159f) differenceAngle -= 2 * 3.14159f;
                    while (differenceAngle < -3.14159f) differenceAngle += 2 * 3.14159f;
                    
                    float poidsVersCentre = 0.3f;
                    float angleFinal = angleFuite + differenceAngle * poidsVersCentre;
                    
                    vitesse.x = std::cos(angleFinal);
                    vitesse.y = std::sin(angleFinal);
                    vitesse = vitesse * (vitesseMax * 1.2f);
                } else {
                    vitesse = (fuite / distDanger) * (vitesseMax * 1.2f);
                }
            } else {
                vitesse = {vitesseMax, 0.f};
            }
            enFuite = true;
            energie -= 5.f * dt;
        }
    }

    if (!enFuite && nourriture) {
        sf::Vector2f versNourriture = nourriture->getPosition() - position;
        float distNourriture = std::hypot(versNourriture.x, versNourriture.y);
        
        if (distNourriture < vueEffective) {
            if (distNourriture > 0.001f) {
                vitesse = (versNourriture / distNourriture) * vitesseMax;
            }
            
            if (distNourriture < rayon + nourriture->getRayon()) {
                nourriture->tuer();
                energie += 40.f;
            }
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

    float coutMetabolique = (rayon * 0.1f) + (vitesseMax * 0.02f);
    energie -= coutMetabolique * dt;
    
    if (energie <= 0) 
        tuer();
}

void Herbivore::dessiner(sf::RenderTarget& cible) const {
    if (invisible) return;

    float angle = 0.f;
    if (std::abs(vitesse.x) > 0.1f || std::abs(vitesse.y) > 0.1f) {
        angle = std::atan2(vitesse.y, vitesse.x);
    }

    std::uint8_t bleu = static_cast<std::uint8_t>(std::clamp(vitesseMax + 50.f, 0.f, 255.f));
    sf::Color couleurCorps(50, 200, bleu, 200);
    sf::Color couleurTentacules(50, 220, 150, 150);

    int nbTentacules = (vitesseMax > 120.f) ? 3 : 5;

    for (int i = 0; i < nbTentacules; ++i) {
        float angleDepart = angle + 3.14f + (i - nbTentacules / 2.f) * 0.4f;
        
        for (int k = 0; k < 6; ++k) {
            float animation = std::sin((position.x + position.y) * 0.05f + k * 0.5f) * 0.2f;
            float distance = rayon * (0.8f + k * 0.4f);
            
            sf::Vector2f posT;
            posT.x = position.x + std::cos(angleDepart + animation) * distance;
            posT.y = position.y + std::sin(angleDepart + animation) * distance;

            sf::CircleShape t(rayon * 0.2f);
            t.setOrigin({t.getRadius(), t.getRadius()});
            t.setPosition(posT);
            t.setFillColor(couleurTentacules);
            cible.draw(t);
        }
    }

    sf::CircleShape corps(rayon);
    corps.setOrigin({rayon, rayon});
    corps.setPosition(position);
    corps.setFillColor(couleurCorps);
    corps.setOutlineThickness(2.f);
    corps.setOutlineColor(sf::Color::White);
    cible.draw(corps);

    if (std::abs(vitesse.x) > 0.1f || std::abs(vitesse.y) > 0.1f) {
        sf::Vector2f posOeil = position + sf::Vector2f(std::cos(angle), std::sin(angle)) * (rayon * 0.6f);
        
        sf::CircleShape oeil(rayon * 0.3f);
        oeil.setOrigin({rayon * 0.3f, rayon * 0.3f});
        oeil.setPosition(posOeil);
        oeil.setFillColor(sf::Color::White);
        cible.draw(oeil);

        sf::CircleShape pupille(rayon * 0.15f);
        pupille.setOrigin({pupille.getRadius(), pupille.getRadius()});
        pupille.setPosition(posOeil + sf::Vector2f(std::cos(angle), std::sin(angle)) * (rayon * 0.15f));
        pupille.setFillColor(sf::Color::Black);
        cible.draw(pupille);
    }
}