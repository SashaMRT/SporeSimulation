#pragma once

namespace Constantes {

    constexpr float FENETRE_LARGEUR = 1280.f;
    constexpr float FENETRE_HAUTEUR = 720.f;
    constexpr unsigned int FPS_LIMITE = 60;
    
    constexpr int NB_ROCHERS_INIT = 6;
    constexpr int NB_ALGUES_INIT = 60;
    constexpr int NB_BACTERIES_INIT = 10;

    constexpr float MENU_LARGEUR = 380.f;
    const sf::Color COULEUR_FOND_MENU(25, 25, 30, 230);
    const sf::Color COULEUR_TITRE = sf::Color::White;
    const sf::Color COULEUR_TEXTE_GRIS(150, 150, 150);
    const sf::Color COULEUR_TEXTE_CLAIR(200, 200, 200);
    const sf::Color COULEUR_ZONE_JEU(10, 30, 50);

    constexpr float ALGUE_RAYON = 5.f;
    constexpr float ALGUE_ENERGIE = 50.f;
    const sf::Color ALGUE_COULEUR(34, 139, 34);

    constexpr float BACTERIE_RAYON = 8.f;
    constexpr float BACTERIE_ENERGIE = 100.f;
    constexpr float BACTERIE_SEUIL_EVOLUTION = 110.f; // Facile (1 algue suffit)
    const sf::Color BACTERIE_COULEUR(0, 255, 255);

    constexpr float HERBIVORE_ENERGIE_BASE = 150.f;
    constexpr float HERBIVORE_VITESSE_BASE = 90.f;
    constexpr float HERBIVORE_RAYON_BASE = 15.f;
    const sf::Color HERBIVORE_COULEUR_LENT(50, 220, 100);
    const sf::Color HERBIVORE_COULEUR_RAPIDE(0, 191, 255);

    constexpr float CARNIVORE_ENERGIE_BASE = 160.f; // On baisse un peu pour les forcer à chasser
    constexpr float CARNIVORE_VITESSE_BASE = 110.f;
    constexpr float CARNIVORE_RAYON_BASE = 20.f;
    const sf::Color CARNIVORE_COULEUR(255, 50, 50);

    constexpr float ROCHER_RAYON_BASE = 20.f;
    constexpr float ROCHER_ENERGIE = 1000.f;
    constexpr float ROCHER_DEGATS = 0.01f;
    const sf::Color ROCHER_COULEUR(100, 100, 100);

    constexpr float ENERGIE_REPRODUCTION = 190.f; 
    constexpr float COUT_REPRODUCTION = 70.f;    
    constexpr float TAUX_MUTATION = 0.15f;     

    constexpr float PARTICULE_VIE_DECAY = 0.8f;
    constexpr float PARTICULE_FRICTION = 0.98f;
    constexpr int PARTICULE_NB_EXPLOSION = 20;
}