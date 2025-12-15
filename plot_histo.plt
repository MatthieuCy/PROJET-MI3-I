#!/bin/bash
# Script GnuPlot : plot_histo.plt

# Variables passées par myScript.sh
INPUT_DATA="$1"      
TITLE="$2"           
Y_LABEL="$3"         
OUTPUT_PNG="$4"      

# Vérification minimale
if [ ! -f "$INPUT_DATA" ]; then
    echo "ERREUR (GnuPlot): Fichier de donnees '$INPUT_DATA' introuvable."
    exit 1
fi

# Le script principal de GnuPlot, généré dynamiquement
cat << EOF | gnuplot

    # Configuration de la sortie et du type de graphique
    set terminal png size 1200,800 font "arial,10"
    set output "$OUTPUT_PNG"

    # Configuration du format des données d'entrée
    set datafile separator ";"
    
    # Configuration de l'histogramme
    set style data histogram
    set style fill solid border -1
    set boxwidth 0.9

    # Mise en forme du titre et des axes
    set title "$TITLE"
    set xlabel "Identifiant Usine"
    set ylabel "$Y_LABEL"

    # Ajustement des tics sur l'axe X pour le nom des usines
    set xtics rotate by -45
    set auto x
    set auto y

    # Tracé : Colonne 2 pour la valeur, Colonne 1 pour le label X.
    plot "$INPUT_DATA" using 2:xticlabels(1) notitle linecolor rgb "blue"

EOF

# Retourne le code de sortie de gnuplot
exit $?
