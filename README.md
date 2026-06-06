# Modélisation Géométrique

Projet de visualisation et de manipulation de maillages reposant sur une structure de
données en demi-arêtes (half-edge). Le code se trouve dans `TP1/MeshViewerCMake/`.

## Compilation et exécution

```bash
cd TP1/MeshViewerCMake
cmake -S . -B build
cmake --build build
./build/MeshViewer
```

Un clic droit dans la fenêtre ouvre le menu : triangulation, Catmull-Clark,
simplification, révolution, affichage des normales et de la silhouette, ouverture d'un
fichier `.obj`, etc.

## Travail réalisé

- **readFile** : lecture des fichiers `.obj` et construction de la structure en
  demi-arêtes (sommets, demi-arêtes, faces, et appariement des twins).
- **Normales** : normale par face à partir du produit vectoriel, et normale par sommet
  obtenue en moyennant les normales des faces adjacentes, en tenant compte des bords.
- **Silhouette** : une arête appartient à la silhouette lorsque l'une de ses faces est
  orientée vers la caméra et l'autre à l'opposé, détecté par le changement de signe du
  produit scalaire entre la direction de vue et les normales.
- **Triangulation** : algorithme d'ear clipping, fonctionnant aussi bien sur les faces
  convexes que concaves. Les polygones à trous (variante optionnelle) n'ont pas été traités.
- **Tests de la structure** : `checkMesh()` vérifie les principaux invariants — twins,
  cohérence des liens next/prev, faces, et champ `originof` des sommets.
- **Surface de révolution** : génération d'un maillage par rotation d'un profil autour
  d'un axe, avec reconstruction de la structure en demi-arêtes.
- **Simplification** : réduction du maillage par effondrement (edge collapse) des arêtes
  les plus courtes.
- **Subdivision de Catmull-Clark** : calcul des points de face et d'arête, repositionnement
  des sommets (règle intérieure et règle de bord), puis reconstruction en quadrilatères.

Une petite suite de tests accompagne le projet :

```bash
cmake --build build --target MeshViewerTests
./build/MeshViewerTests
```

## Compatibilité macOS

Je développe à la fois sur MacBook M1 et sur Windows, et j'ai choisi de travailler
principalement sous macOS. Le code de base étant prévu pour Windows et FreeGLUT, j'ai
rencontré quelques problèmes de compatibilité avec OpenGL. Je me suis appuyé sur une IA
pour les diagnostiquer et les corriger ; il s'agissait essentiellement de trois points :

- Sous macOS, il faut utiliser le GLUT natif (`<GLUT/glut.h>`) plutôt que FreeGLUT, et les
  Vertex Array Objects n'y sont exposés que sous leur nom suffixé `*APPLE`
  (`glGenVertexArraysAPPLE`, etc.). D'où le bloc `#if defined(__APPLE__)` en tête de
  `main.cpp`, qui bascule vers FreeGLUT sur les autres systèmes.
- Côté CMake, il a fallu pointer vers Homebrew (`/opt/homebrew`) pour GLEW et lier le
  framework natif via `-framework GLUT`, tout en conservant FreeGLUT pour Windows.
- L'ouverture d'un fichier passe par un appel AppleScript (`osascript`) afin d'utiliser le
  sélecteur de fichiers natif du système.

Ces ajustements ne concernent que la portabilité : ils ne modifient pas les algorithmes et
permettent d'exécuter le même code sur les deux systèmes.

## Utilisation de l'IA

Pour la partie algorithmique, j'ai utilisé l'IA comme un outil d'accompagnement, à la
manière d'un professeur particulier : comprendre le fonctionnement de certains algorithmes
(Catmull-Clark, ear clipping, edge collapse), clarifier des points théoriques et vérifier
ma compréhension. L'implémentation des exercices reste mon travail.
