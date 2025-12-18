#!/usr/bin/gnuplot

# Variables passées par le script Shell
# $1 : Fichier de données (ex: histo_outputs/bottom50.dat)
# $2 : Titre du graphique
# $3 : Label de l'axe Y
# $4 : Nom du fichier PNG de sortie

# Configuration de la sortie
set terminal png size 1200,800 font "arial,10"
set output ARG4

# Configuration du format
set datafile separator ";"
set style data histogram
set style fill solid border -1
set boxwidth 0.9

# Titres et labels
set title ARG2
set xlabel "Identifiant Usine"
set ylabel ARG3

# Esthétique de l'axe X
set xtics rotate by -45
set grid y

# Tracé
# On utilise ARG1 (le premier argument)
# using 2:xticlabels(1) signifie : 
#   - colonne 2 pour la hauteur des barres
#   - colonne 1 pour les noms sous les barres
plot ARG1 using 2:xticlabels(1) notitle linecolor rgb "blue"
