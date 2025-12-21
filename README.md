 # **Projet C-WildWater :**
 # **Analyse de Réseaux de Distribution d’Eau**  
 ## Projet en C et Shell.  




 ### Description du projet

Ce projet a pour objectif d’analyser des données issues d’un système de distribution d’eau potable en France.

Ce que le programme permet de faire :

1. Statistiques des Usines (Mode histo).
   - On génère des histogrammes basés sur trois types de données :
      - Max : Le débit maximum que l'usine peut traiter.
      - SRC : Le volume total capté depuis les sources.
      - REAL : Le volume final traité (réel).


2. Calcul des Rendements (Mode leaks).
   - Le mode leaks calcule le volume d'eau perdu entre une usine et les consommateurs en additionnant les fuites de chaque tronçon du réseau. Le résultat est enregistré dans leaks.dat et affiche -1 si l'identifiant de l'usine n'existe pas.

     

### Le projet repose sur :
- images : Contient tous les graphiques (PNG) générés.

- myScript.sh : Script qui gère les erreurs, lance la compilation et pilote les traitements.

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
  - ./myScript.sh c-wildwater_v3.dat histo max
  - ./myScript.sh c-wildwater_v3.dat histo real
  - ./myScript.sh c-wildwater_v3.dat histo src

- Pour calculer les pertes d'une usine précise :
   - utilisez la commande suivante avec l'identifiant de l'usine entre guillemets :
       - ./myScript.sh c-wildwater_v3.dat cat leaks_history.dat "NOM_DE_L_USINE" 




