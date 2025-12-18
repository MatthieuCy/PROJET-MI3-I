 Projet Distribution d’Eau  
Analyse de données d’un système de distribution d’eau en France  
(Shell + C)



  📌 Objectif du projet

Ce projet a pour objectif d’analyser des données issues d’un système de distribution d’eau potable en France.  
Il permet notamment :

- la génération d’histogrammes sur les capacités et volumes des usines,
- le calcul des pertes d’eau (fuites) en aval d’une usine donnée,
- l’export des résultats sous forme de fichiers CSV et de graphiques PNG.

Le projet repose sur :
- un script Shell (`myscript.sh`) pour l’orchestration,
- plusieurs programmes en C
- Gnuplot pour la visualisation graphique.



   Structure du projet




├── myscipt.sh             # Script principal
   plot_histo.plt          #Génération des histogrammes
├── README.md              # Documentation
│   ├── Makefile
│   ├── avl.c / avl.h      # Arbres AVL et Histogrammes
│   ├── leaks.c            # Calcul des fuites
├── data/                  # Données d’entrée 
│   └── c-wildwater_v3.dat
