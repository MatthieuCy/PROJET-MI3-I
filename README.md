 # **Projet C-WildWater : Analyse de Réseaux de Distribution d’Eau**  
 ## Projet en C et Shell.  




 ### Description du projet

Ce projet a pour objectif d’analyser des données issues d’un système de distribution d’eau potable en France.

Ce que le programme permet de faire :

1. Statistiques des Usines (Mode histo).
On génère des histogrammes basés sur trois types de données :

Capacité (max) : Le débit maximum que l'usine peut traiter.

Entrée (src) : Le volume total capté depuis les sources.

Sortie (real) : Le volume final traité (réel).

2. Calcul des Rendements (Mode leaks).
Le programme suit le réseau d'une usine vers l'aval pour identifier les fuites.

Enregistrement : Les résultats sont ajoutés dans leaks.dat (Format : ID_Usine; Valeur).

Sécurité : Si l'usine est introuvable, le système renvoie -1 et affiche Facility Complex #INEXISTANT.

### Le projet repose sur :
- images : Contient tous les graphiques (PNG) générés.

- main.c : Script principal pour lancer le programme.

- Makefile : Pour la compilation automatique.

- plot_histo.plt : Script Gnuplot pour créer les images.

- avl.h / avl_usine.c : Gestion des structures de données (Arbres AVL).

- main.c / leak.c : Logique de calcul et moteur du programme.

- c-wildwater_v0.dat : Fichier de données pour les tests.

- .gitignore : Pour ne pas envoyer les fichiers inutiles sur GitHub.


### Comment éxecuter notre programme:

- On compile le programme avec la commande make.
  
- Pour donner les droits d'accès aux scripts on tape chmod +x main.sh plot_histo.plt.
  
- Pour l'histogramme , utilisez la commande suivante en choisissant l'une des trois options (max, real ou src) :
  ./myScript.sh c-wildwater_v3.dat histo max
  ./myScript.sh c-wildwater_v3.dat histo real
  ./myScript.sh c-wildwater_v3.dat histo max

-Pour calculer les pertes d'une usine précise :
  utilisez la commande suivante avec l'identifiant de l'usine entre guillemets : ./myScript.sh c-wildwater_v0.dat leaks "NOM_DE_L_USINE" 



│   └── c-wildwater_v3.dat
