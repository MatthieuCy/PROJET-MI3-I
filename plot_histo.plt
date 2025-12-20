#!/usr/bin/gnuplot -persist

# ARG1 : Fichier .dat | ARG2 : Titre | ARG3 : Label Y | ARG4 : Sortie .png

set terminal pngcairo size 1400,900 font "arial,10"
set output ARG4

#  Para  style et couleur
set datafile separator ";"
set style data histograms
set style fill solid 0.8 border -1
set boxwidth 0.75
set grid y lc rgb "#E5E5E5"

#  Titres et marges
set title ARG2 font "arial,18,bold"
set xlabel "Identifiants des Usines" font "arial,12"
set ylabel ARG3 font "arial,12"
set bmargin 12 # Espace pour les IDs inclinés
set tmargin 5  # Espace pour les valeurs en haut

# Échelle +(espace de 15% en haut pour que les chiffres ne soient pas coupés)
set offsets 0, 0, graph 0.15, 0 
set yrange [0:*] 
set format y "%.0f"

# Rotation des IDs
set xtics rotate by -45 font "arial,9" scale 0

#  Détection colonne (max=2, src=3, real=4)
col = (strstrt(ARG2, "max") > 0) ? 2 : (strstrt(ARG2, "src") > 0 ? 3 : 4)

#  Dessin 
plot ARG1 using col:xticlabels(1) title "Valeur" linecolor rgb "#4682B4", \
     '' using 0:col:col with labels font "arial,9,bold" offset 0,0.8 notitle
