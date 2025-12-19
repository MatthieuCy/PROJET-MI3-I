#!/bin/bash

# Mesure du temps d'exécution
start_time=$(date +%s%N)

# 1. Validation des arguments
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <fichier_donnees.csv> <commande: histo|leaks> [option/ID]"
    echo "Exemple: $0 c-wildwater_v3.dat histo max"
    exit 1
fi

DATA_FILE="$1"
COMMAND="$2"
PARAM="$3"
EXECUTABLE="./wildwater"

# Vérification de l'existence du fichier de données
if [ ! -f "$DATA_FILE" ]; then
    echo "Erreur : Le fichier $DATA_FILE est introuvable."
    exit 2
fi

# 2. Compilation via le Makefile
echo "--- Compilation ---"
make
if [ $? -ne 0 ]; then
    echo "Erreur de compilation. Vérifiez votre Makefile et votre code C."
    exit 3
fi

# 3. Création des dossiers de sortie
mkdir -p histo_outputs

# 4. Traitement des commandes
if [ "$COMMAND" == "histo" ]; then
    echo "--- Mode Histogramme ---"
    TEMP_RAW="histo_raw.dat"
    RESULT_PNG="histo_outputs/histogramme_${PARAM}.png"
    
    # Exécution du programme C
    $EXECUTABLE histo "$DATA_FILE" "$TEMP_RAW"
    
    if [ $? -eq 0 ] && [ -f "$TEMP_RAW" ]; then
        # Tri Shell : On ignore l'entête, on trie par capacité (colonne 2) numériquement
        # On prend les 15 plus grandes valeurs pour que le graphique soit lisible
        PROCESSED_DATA="histo_outputs/top_data.dat"
        head -n 1 "$TEMP_RAW" > "$PROCESSED_DATA" # Garder l'entête
        tail -n +2 "$TEMP_RAW" | sort -t ';' -k2 -nr | head -n 15 >> "$PROCESSED_DATA"
        
        echo "Génération du graphique avec Gnuplot..."
        # Appel du script Gnuplot avec ses 4 arguments
        gnuplot -c ./plot_histo.plt "$PROCESSED_DATA" "Top 15 - Capacités des Usines" "Capacité (M.m3/an)" "$RESULT_PNG"
        
        if [ $? -eq 0 ]; then
            echo "Succès ! Image générée : $RESULT_PNG"
        fi
        
        # Nettoyage du fichier temporaire brut
        rm "$TEMP_RAW"
    else
        echo "Erreur lors de l'exécution du programme C."
    fi

elif [ "$COMMAND" == "leaks" ]; then
    echo "--- Mode Fuites (Leaks) ---"
    if [ -z "$PARAM" ]; then
        echo "Erreur : Vous devez fournir un ID d'usine pour le mode leaks."
        exit 4
    fi
    # Exécution simple
    $EXECUTABLE leaks "$DATA_FILE" "$PARAM"
else
    echo "Commande inconnue : $COMMAND (utilisez 'histo' ou 'leaks')"
fi

# 5. Calcul de la durée
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))
echo "---"
echo "Exécution terminée en $duration ms."
