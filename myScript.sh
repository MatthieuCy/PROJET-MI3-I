#!/bin/bash

# Mesure du temps d'exécution (début)
start_time=$(date +%s%N)

# verif argument
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <fichier_donnees.csv> <commande: histo|leaks> [option/ID]"
    echo "Exemples:"
    echo "  $0 c-wildwater_v3.dat histo max"
    echo "  $0 c-wildwater_v3.dat leaks 102"
    exit 1
fi

DATA_FILE="$1"
COMMAND="$2"
PARAM="$3"
EXECUTABLE="./wildwater"

# Vérif existence du fichier de données
if [ ! -f "$DATA_FILE" ]; then
    echo "Erreur : Le fichier $DATA_FILE est introuvable."
    exit 2
fi

# compil makefile
if [ ! -f "$EXECUTABLE" ]; then
    echo " Compilation "
    make
    if [ $? -ne 0 ]; then
        echo "Erreur de compilation. Vérifiez votre Makefile et votre code C."
        exit 3
    fi
fi

# Création du dossier pour les images s'il n'existe pas
mkdir -p histo_outputs

# traitement commande

# 1 : Histogramme
if [ "$COMMAND" == "histo" ]; then
    echo "Mode Histogramme : $PARAM "
    
    # Vérification 
    if [[ "$PARAM" != "max" && "$PARAM" != "src" && "$PARAM" != "real" ]]; then
        echo "Erreur : L'option doit être max, src ou real."
        exit 5
    fi

    TEMP_RAW="temp_c_output.dat"
    
    # Appel du programme C (Arguments réordonnés pour correspondre au main.c)
    $EXECUTABLE "$COMMAND" "$DATA_FILE" "$PARAM" "$TEMP_RAW"
    
    if [ $? -eq 0 ] && [ -f "$TEMP_RAW" ]; then
        # Définition des noms des fichiers de sortie
        DATA_MIN="histo_outputs/data_${PARAM}_min50.dat"
        DATA_MAX="histo_outputs/data_${PARAM}_max10.dat"
        IMG_MIN="histo_outputs/${PARAM}_min50.png"
        IMG_MAX="histo_outputs/${PARAM}_max10.png"

        echo "Découpage des données (50 Min et 10 Max)..."

        # Séparation des données :
    
        head -n 1 "$TEMP_RAW" > "$DATA_MIN"
        head -n 1 "$TEMP_RAW" > "$DATA_MAX"

        # Les 50 plus petites 
        tail -n +2 "$TEMP_RAW" | head -n 50 >> "$DATA_MIN"

        # Les 10 plus grandes 
        tail -n +2 "$TEMP_RAW" | tail -n 10 >> "$DATA_MAX"

        # Génération des deux graphiques avec Gnuplot
        echo "Lancement de Gnuplot..."
        gnuplot -c ./plot_histo.plt "$DATA_MIN" "50 plus petites usines ($PARAM)" "Volume(k.m3.year-1)" "$IMG_MIN"
        gnuplot -c ./plot_histo.plt "$DATA_MAX" "10 plus grandes usines ($PARAM)" "Volume(k.m3.year-1)" "$IMG_MAX"
        
        echo "Succès ! Images créées dans le dossier histo_outputs."
        rm "$TEMP_RAW"
    else
        echo "Erreur : Le programme C a échoué."
    fi

# 2 Leaks
elif [ "$COMMAND" == "leaks" ]; then
    echo " Mode Fuites (Leaks) "
    if [ -z "$PARAM" ]; then
        echo "Erreur : Vous devez fournir un ID d'usine."
        exit 4
    fi
    # Exécution du programme C pour les fuites (Arguments réordonnés)
    $EXECUTABLE "$COMMAND" "$DATA_FILE" "$PARAM" "leaks_history.dat"
    
    if [ $? -ne 0 ]; then
        echo "Erreur : Le programme C a échoué."
    fi

else
    echo "Commande inconnue : $COMMAND (utilisez 'histo' ou 'leaks')"
    exit 6
fi

# caclul duree totale(en ms)
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))
echo "---"
echo "Exécution terminée en $duration ms."
