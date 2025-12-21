 # **Projet C-WildWater : Analyse de Réseaux de Distribution d’Eau**  
 ## Projet en langage C et Shell.  




 ### Description du projet

Ce projet a pour objectif d’analyser des données issues d’un système de distribution d’eau potable en France.  
Il permet notamment :

- la génération d’histogrammes sur les capacités et volumes des usines,
- le calcul des pertes d’eau  en aval d’une usine donnée,
- l’export des résultats sous forme de fichiers CSV et de graphiques PNG.

### Le projet repose sur :
- un script Shell (`myScript.sh`) pour l’orchestration,
- plusieurs programmes en C
- Gnuplot pour la visualisation graphique.



### Comment executer notre programme:

-Cloner le dossier sur votre pc avec git clone
-Faire cd dans le fichier créer
-Utiliser commande make
-Mettre les droits avec chmod +x (ne pas oublier de mettre les droits à plot_histo.plt)
- Puis executer / Liste de tous les exécutions possibles histogramme :
  
  ./myScript.sh c-wildwater_v3.dat histo max
  ./myScript.sh c-wildwater_v3.dat histo real
  ./myScript.sh c-wildwater_v3.dat histo max

   Pour leak :
  ./myScript.sh c-wildwater_v3.dat leak
  Si vous voulez tester avec une seule usine précise:
  ./wildwater leak c-wildwater_v3.dat ID_USINE

Structure du projet


├── myscipt.sh             # Script principal
   plot_histo.plt          #Génération des histogrammes
├── README.md              # Documentation
│   ├── Makefile
│   ├── avl.c / avl.h      # Arbres AVL et Histogrammes
│   ├── leaks.c            # Calcul des fuites
├── data/                  # Données d’entrée 
│   └── c-wildwater_v3.dat
