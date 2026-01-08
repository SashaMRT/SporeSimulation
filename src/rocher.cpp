/**
 * @file Rocher.cpp
 * @author Sasha Marie te Rehost (sasha.marieterehost@gmail.com)
 * @author Gael Guinaliu (rodez.gael@gmail.com)
 * @brief Implémentation de la classe Rocher.
 * @details Définit le comportement et l'apparence des obstacles rocheux dans le monde.
 * Les rochers servent d'obstacles physiques et de cachettes pour les herbivores.
 */

/**
 * @brief Constructeur de la classe Rocher.
 * * Initialise un rocher statique à une position donnée.
 * La taille du rocher varie légèrement d'une instance à l'autre pour créer de la diversité visuelle.
 * * @param pos Position (x, y) du centre du rocher dans le monde.
 */
Rocher::Rocher(sf::Vector2f pos) 
    : Entite(pos, TypeEntite::ROCHER, 
             // Le rayon est une base fixe + une variation aléatoire entre 0 et 19 pixels
             Constantes::ROCHER_RAYON_BASE + (rand() % 20), 
             Constantes::ROCHER_ENERGIE), 
      occupe(false) // Par défaut, personne n'est caché derrière le rocher
{
}

/**
 * @brief Met à jour l'état du rocher.
 * * @details Actuellement, le rocher est une entité statique qui n'évolue pas avec le temps.
 * Cette méthode est nécessaire car héritée de la classe abstraite Entite.
 * * @param dt Temps écoulé depuis la dernière image (Delta Time), non utilisé ici.
 * @param monde Référence vers le monde (pour d'éventuelles interactions futures).
 */
void Rocher::update(float dt, Monde& monde) {
    // Le rocher est inerte, il ne fait rien de particulier à chaque frame.
    // On pourrait ajouter ici une régénération de ressources ou une érosion.
}

/**
 * @brief Dessine le rocher sur la fenêtre.
 * * Crée une forme irrégulière (polygone convexe) pour imiter l'aspect d'une pierre.
 * Change de couleur si le rocher est utilisé comme cachette.
 * * @param cible La cible de rendu (la fenêtre SFML).
 */
void Rocher::dessiner(sf::RenderTarget& cible) const {
    // --- Création de la forme du rocher ---
    // On utilise un polygone convexe à 6 points pour éviter l'aspect "parfait" d'un cercle
    sf::ConvexShape forme(6);

    // Définition manuelle des points pour créer une forme patatoïde
    // Les coordonnées sont relatives au centre du rocher (0,0 local)
    forme.setPoint(0, {rayon, 0.f});                     // Point Est (droite)
    forme.setPoint(1, {rayon * 0.8f, rayon * 0.7f});     // Sud-Est
    forme.setPoint(2, {-rayon * 0.5f, rayon});           // Sud-Ouest
    forme.setPoint(3, {-rayon, 0.2f});                   // Ouest (gauche)
    forme.setPoint(4, {-rayon * 0.7f, -rayon * 0.8f});   // Nord-Ouest
    forme.setPoint(5, {0.3f, -rayon});                   // Nord

    // On place la forme à la position réelle de l'entité dans le monde
    forme.setPosition(position);
    
    // --- Gestion de la couleur (Feedback visuel) ---
    if (occupe) {
        // Si un herbivore est caché ici, le rocher devient plus sombre
        // Cela permet au joueur de savoir qu'il y a quelqu'un
        forme.setFillColor(sf::Color(40, 40, 45));
    } else {
        // Couleur normale : Gris pierre standard
        forme.setFillColor(sf::Color(80, 80, 90));
    }
    
    // Ajout d'un contour pour mieux distinguer le rocher du fond
    forme.setOutlineThickness(2.f);
    forme.setOutlineColor(sf::Color(50, 50, 55)); // Contour gris foncé
    
    // Dessin de la forme principale
    cible.draw(forme);

    // --- Ajout de détails (Reflet) ---
    // On ajoute un petit reflet seulement si le rocher n'est pas "occupé" (caché dans l'ombre)
    if (!occupe) {
        sf::CircleShape reflet(rayon * 0.3f);
        // Position décalée vers le haut-gauche pour simuler une source de lumière
        reflet.setPosition(position - sf::Vector2f(rayon*0.4f, rayon*0.4f));
        
        // Couleur gris clair avec transparence (alpha = 100)
        reflet.setFillColor(sf::Color(120, 120, 130, 100));
        
        cible.draw(reflet);
    }
}