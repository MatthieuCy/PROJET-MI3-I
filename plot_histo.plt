#!/usr/bin/gnuplot -persist

# ARG1 : Fichier de données (.dat)
# ARG2 : Titre du graphique (Texte)
# ARG3 : Label de l'axe Y (Texte)
# ARG4 : Fichier de sortie (.png)

# 1. Configuration de la sortie
set terminal png size 1200,800 font "arial,11"
set output ARG4

# 2. Formatage des données
set datafile separator ";"
set style data histograms
set style fill solid 0.6 border -1
set boxwidth 0.8

# 3. Décoration et titres
set title ARG2 font "arial,16"
set xlabel "Identifiants des Usines" font "arial,12"
set ylabel ARG3 font "arial,12"

# 4. Grille et axes
set grid y
set yrange [0:*] # L'axe Y commence toujours à 0
set xtics rotate by -45 # Rotation pour éviter que les IDs se chevauchent

# 5. Dessin
# On utilise la colonne 2 (Capacité) et la colonne 1 pour les labels (ID)
plot ARG1 using 2:xticlabels(1) notitle linecolor rgb "#1E90FF"
