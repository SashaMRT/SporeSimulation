# Spore Simulation - Simulation d'Écosystème Artificiel

![C++](https://img.shields.io/badge/C++-20-blue.svg?style=flat&logo=c%2B%2B)
![SFML](https://img.shields.io/badge/SFML-3.0-green.svg?style=flat&logo=sfml)
![Platform](https://img.shields.io/badge/Platform-Linux%20(Recommended)-orange)
![VS Code](https://img.shields.io/badge/IDE-VS%20Code-blueviolet.svg?logo=visual-studio-code)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

## 1. Présentation du Projet

### 📖 Le Contexte
Voici notre **Mini Projet** réalisé pour notre devoir de synthèse en C++.
On est partis sur une idée qui nous plaisait bien : essayer de recréer **une simulation du jeu vidéo "Spore"**. En gros, c'est une simulation graphique en 2D où des petites créatures évoluent et survivent dans un écosystème.

Le but, c'était de coder un moteur capable de faire émerger des **comportements complexes** (comme la prédation, la fuite ou l'équilibre naturel) juste avec des règles simples, tout en respectant les consignes du prof.

### 📜 Le Cahier des Charges
Pour valider le projet, on a dû respecter plusieurs contraintes techniques :
* **Architecture** : Tout coder en C++ avec la bibliothèque **SFML** pour l'affichage.
* **POO (Objet)** : Avoir une structure propre avec de l'héritage (`Entite` mère), du polymorphisme (méthodes virtuelles) et de l'encapsulation.
* **STL (Standard Template Library)** : On devait utiliser au moins 3 conteneurs différents de manière intelligente. On a choisi :
    * `std::vector` (pour stocker les entités).
    * `std::list` (pour gérer les particules).
    * `std::map` (pour les statistiques).
* **Simulation** : Le système devait être autonome, avec une vraie chaîne alimentaire et de la reproduction.

---

## 2. L'Équipe et les Crédits

### 👥 Qui a fait quoi ?
On a travaillé en binôme sur ce projet :

| Auteur | Rôle & Responsabilités | Contact |
| :--- | :--- | :--- |
| **Sasha Marie te Rehost** | **Moteur, Interface & Graphismes**<br>• J'ai géré le moteur principal du jeu (la boucle, la gestion du Monde).<br>• Je me suis occupé de la fenêtre et des événements.<br>• J'ai codé toute l'interface (HUD, Menus) et le système de stats (`std::map`).<br>• J'ai aussi fait les graphismes procéduraux et les particules. | [sasha.marieterehost@gmail.com](mailto:sasha.marieterehost@gmail.com) |
| **Gael Guinaliu** | **Gameplay & Simulation**<br>• Il a géré la logique de Gameplay (IA, Comportements).<br>• Il a mis en place le système d'évolution et de génétique.<br>• Il s'est occupé de la physique et des collisions. | [rodez.gael@gmail.com](mailto:rodez.gael@gmail.com) |

### 🏆 Merci à...
* **Pour la base** : Pour partir sur une structure propre (CMake compatible SFML 3), on a utilisé le template **cmake-sfml-project** disponible ici : [https://github.com/SFML/cmake-sfml-project](https://github.com/SFML/cmake-sfml-project).
* **SFML** : Merci à l'équipe de la SFML, c'est une librairie super agréable à utiliser (FAUX).

---

## 3. Comment ça marche ?

Le programme simule un "bocal" virtuel. Voici les règles du jeu :

### 🧬 Le Cycle de la Vie
1.  **Algues** : Elles apparaissent toutes seules. C'est la nourriture de base.
2.  **Bactéries** : Elles mangent les algues. Si elles ont assez d'énergie, elles **évoluent** (au hasard) en Herbivore ou en Carnivore.
3.  **Herbivores** : Ils mangent les algues, essaient de fuir les prédateurs et se reproduisent entre eux.
4.  **Carnivores** : Ils chassent les herbivores pour survivre.
5.  **Rochers** : Ce sont des obstacles, mais les petites créatures peuvent s'en servir pour se cacher.

### 🧠 Évolution et IA
Chaque créature a son propre "ADN" (Vitesse, Taille, Vue).
Quand elles se reproduisent :
* Le parent perd de l'énergie.
* L'enfant hérite des stats du parent, mais avec une petite **mutation aléatoire** (+/- 15%).
* **Résultat** : Les animaux trop lents se font manger, et ceux qui ne voient rien meurent de faim. C'est la sélection naturelle !

---

## 4. Architecture du Code

On a essayé de faire une architecture modulaire pour ne pas se perdre dans le code.

### 🏗 Structure (Classes)
* **`Entite` (Classe Mère)** : Elle définit ce qu'est une créature (position, énergie, méthode `update()`...).
    * `Algue`, `Bacterie`, `Herbivore`, `Carnivore`, `Rocher` héritent tous de `Entite`.
* **`Monde`** : C'est la classe principale qui contient tout le monde et gère les interactions.
* **`Rendu`** : Elle ne s'occupe que de l'affichage (l'interface, les textes, les jauges).

### 💾 Nos choix pour la STL
Comme demandé, on utilise 3 conteneurs différents pour des raisons précises :

1.  **`std::vector`** pour les Entités : C'est le plus rapide pour parcourir toute la liste à chaque image (ce qu'on fait tout le temps).
2.  **`std::list`** pour les Particules : Comme les explosions créent et détruisent plein de particules tout le temps, la liste est plus efficace pour supprimer des éléments au milieu sans tout décaler.
3.  **`std::map`** pour les Stats : Ça nous permet d'associer un type d'animal (Clé) à ses chiffres (Valeur) facilement pour l'affichage.

---

## 5. Guide d'Utilisation

### 🖱️ Contrôles
* **Clic Gauche** : Faire apparaître une Algue.
* **Clic Droit** : Faire apparaître une Bactérie.
* **Survol Souris** : Inspecter une créature (voir ses stats).
* **Touche P** : Pause.
* **Touche R** : Reset (tout recommencer).
* **Échap** : Quitter.

### 📊 L'Interface
Sur le côté, vous verrez :
* Les FPS et le temps.
* Des jauges colorées pour voir l'évolution des populations.
* Le détail des naissances et des morts.

---

## 6. Installation et Compilation (VS Code)

Voici comment lancer le projet. On recommande **Linux** (c'est plus simple) et **VS Code** comme demandé en cours.

> **⚠️ ATTENTION : SFML 3 REQUISE**
> Le projet est fait pour fonctionner avec la **SFML 3.0**. Faites attention à ne pas installer une vieille version (2.5) par erreur.

### 📦 Ce qu'il vous faut
1.  **VS Code** avec les extensions **C/C++** et **CMake Tools**.
2.  **CMake** installé sur votre ordi.

### 🐧 Sur Linux (Recommandé)

1.  Installez les outils de base :
    ```bash
    sudo apt-get update
    sudo apt-get install build-essential cmake libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev
    ```
2.  Ouvrez VS Code (`code .`).
3.  Acceptez les extensions si on vous le demande.
4.  Cliquez sur **CMake: Configure** puis **Build** en bas de la fenêtre.
5.  Lancez le jeu (Bouton Play).

### 🟦 Sur Windows

1.  Il vous faut un compilateur C++. On conseille **MinGW** ou **Visual Studio Community**.
2.  Ouvrez le dossier dans VS Code.
3.  VS Code va détecter le fichier `CMakeLists.txt`. S'il demande de configurer, dites **Oui**.
4.  Choisissez votre compilateur (Kit) quand il le demande (ex: `GCC` ou `Visual Studio`).
5.  Cliquez sur **Build** (ou `F7`) dans la barre bleue en bas.
    * *Attention : La première fois c'est long, car ça télécharge SFML tout seul.*
6.  Cliquez sur le bouton "Lecture" (Play) en bas.

### 🍎 Sur macOS

1.  Installez les outils : `xcode-select --install` et `brew install cmake`.
2.  Ouvrez dans VS Code, choisissez le Kit `Clang`.
3.  Build & Run via l'interface CMake en bas.

---

### 💻 En ligne de commande (Si besoin)

Si vous préférez le terminal classique :

```bash
mkdir build
cd build
cmake ..
cmake --build .
./bin/SporeSimulation