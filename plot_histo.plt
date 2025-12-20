#!/usr/bin/gnuplot -persist

# ARG1 : Fichier de données (.dat)
# ARG2 : Titre du graphique
# ARG3 : Label Y
# ARG4 : Fichier de sortie (.png)

set terminal png size 1200,800 font "arial,11"
set output ARG4

set datafile separator ";"
set style data histograms
set style fill solid 0.7 border -1
set boxwidth 0.8

set title ARG2 font "arial,16"
set xlabel "Identifiants des Usines" font "arial,12"
set ylabel ARG3 font "arial,12"

set grid y
set yrange [0:*]
set xtics rotate by -45 scale 0

# --- LOGIQUE DE SÉLECTION DE COLONNE ---
# Si le titre contient "max", on trace la colonne 2
# Si le titre contient "src", on trace la colonne 3
# Si le titre contient "real", on trace la colonne 4
col = (strstrt(ARG2, "max") > 0) ? 2 : (strstrt(ARG2, "src") > 0 ? 3 : 4)

plot ARG1 using col:xticlabels(1) title "Valeur Mesurée" linecolor rgb "#1E90FF"
