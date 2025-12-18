#!/bin/bash
start_time=$(date +%s%N)

# Validation des arguments
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <fichier.csv> <commande> [option/ID]"
    exit 1
fi

DATA_FILE="$1"
COMMAND="$2"
PARAM="$3"
EXECUTABLE="./wildwater"

# 1. Compilation
make > /dev/null
if [ $? -ne 0 ]; then echo "Erreur de compilation"; exit 2; fi

# 2. Exécution selon le mode
if [ "$COMMAND" == "histo" ]; then
    TEMP_OUT="histo_data_${PARAM}.dat"
    $EXECUTABLE histo "$DATA_FILE" "$TEMP_OUT"
    
    if [ $? -eq 0 ]; then
        mkdir -p histo_outputs
        # Tri et filtrage via Shell (conforme si le C a filtré les colonnes)
        tail -n +2 "$TEMP_OUT" | sort -t ';' -k2 -n | head -n 50 > "histo_outputs/bottom50.dat"
        ./plot_histo.plt "histo_outputs/bottom50.dat" "Titre" "Y-Label" "histo_outputs/res.png"
        
        # Nettoyage des fichiers intermédiaires (Exigence de propreté)
        rm "$TEMP_OUT" "histo_outputs/bottom50.dat"
    fi

elif [ "$COMMAND" == "leaks" ]; then
    $EXECUTABLE leaks "$DATA_FILE" "$PARAM"
fi

# 3. Fin et Durée
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))
echo "Exécution terminée en $duration ms."
