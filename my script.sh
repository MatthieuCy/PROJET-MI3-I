#!/bin/bash


start_time=$(date +%s%N)

# Noms des fichiers/répertoires
EXECUTABLE="./wildwater"

FICHIER_DONNEES_C="c-wildwater_v3.dat" 
HISTOGRAM_OUT_DIR="histo_outputs"
LEAKS_OUT_FILE="leaks_history.dat"
PLOT_SCRIPT_FILE="plot_histo.plt"

# Arguments utilisateur
USER_DATA_FILE="$1"
COMMANDE="$2"
OPTION="$3"

# Fonction pour afficher la durée et sortir
cleanup_and_exit() {
    local exit_code=$1
    
    # Nettoyage du lien symbolique
    rm -f "$FICHIER_DONNEES_C"
    
    # Calcul et affichage de la durée totale
    end_time=$(date +%s%N)
    duration_ns=$(( end_time - start_time ))
    duration_ms=$(( duration_ns / 1000000 ))
    
    echo "Duree totale d'execution: ${duration_ms} ms"
    exit "$exit_code"
}


# 2. VALIDATION DES ARGUMENTS DE BASE


if [ -z "$COMMANDE" ]; then
    echo "ERREUR: Commande manquante."
    echo "Usage: $0 <fichier_donnees_csv> <commande> [option/ID]"
    echo "Commandes supportees: histo, leaks"
    cleanup_and_exit 1
fi

if [ ! -f "$USER_DATA_FILE" ]; then
    echo "ERREUR: Fichier de donnees '$USER_DATA_FILE' introuvable."
    cleanup_and_exit 1
fi


# 3. COMPILATION


echo "-> Tentative de compilation (via Makefile)..."
make

if [ $? -ne 0 ]; then
    echo "ERREUR: Echec de la compilation. Code de retour du makefile : $?."
    cleanup_and_exit 2
fi


ln -sf "$USER_DATA_FILE" "$FICHIER_DONNEES_C"

chmod +x "$PLOT_SCRIPT_FILE"

# 4. TRAITEMENT DE LA COMMANDE 'histo'


if [ "$COMMANDE" == "histo" ]; then
    echo "-> Execution de la commande 'histo'..."
    
    # Validation de l'option
    if [ -z "$OPTION" ] || ([ "$OPTION" != "max" ] && [ "$OPTION" != "src" ] && [ "$OPTION" != "real" ]); then
        echo "ERREUR: Option invalide ou manquante pour 'histo'. Options supportees: max, src, real."
        cleanup_and_exit 3
    fi

    # Nom du fichier de données CSV généré par le programme C
    C_OUTPUT_FILE="histo_data_${OPTION}.dat"

    # Appel du programme C pour générer le fichier de données
    echo "   Appel du programme C: $EXECUTABLE histo $C_OUTPUT_FILE"
    "$EXECUTABLE" histo "$C_OUTPUT_FILE"

    if [ $? -ne 0 ]; then
        echo "ERREUR: Le programme C a echoue lors de la generation de l'histogramme."
        cleanup_and_exit 4
    fi

    # Création du répertoire de sortie
    mkdir -p "$HISTOGRAM_OUT_DIR"
    
    # Définition de la colonne à utiliser pour le tri et le titre du graphique
    case "$OPTION" in
        max)
            COLUMN=2
            TITLE_Y="Capacite Maximale (M.m3.year-1)"
            ;;
        src)
            COLUMN=3
            TITLE_Y="Volume Capte (M.m3.year-1)"
            ;;
        real)
            COLUMN=4
            TITLE_Y="Volume Reel Traite (M.m3.year-1)"
            ;;
    esac

    # Retirer l'en-tête (ligne 1), trier par la colonne choisie

    
    # GESTION DES 50 PLUS PETITES USINES
    
    echo "   Generation du graphique pour les 50 plus petites usines..."
    
    # Tri: numérique sur la colonne (-n) puis prendre les 50 premières (les plus petites)
    tail -n +2 "$C_OUTPUT_FILE" | sort -t ';' -k $COLUMN -n | head -n 50 > "${HISTOGRAM_OUT_DIR}/bottom50_${OPTION}.dat"

    # Appel du script GnuPlot (il génère l'image .png)
    ./"$PLOT_SCRIPT_FILE" "${HISTOGRAM_OUT_DIR}/bottom50_${OPTION}.dat" \
        "50 plus petites usines - ${TITLE_Y}" \
        "$TITLE_Y" \
        "${HISTOGRAM_OUT_DIR}/bottom50_${OPTION}.png" 
        
    if [ $? -ne 0 ]; then cleanup_and_exit 7; fi
-
    # GESTION DES 10 PLUS GRANDES USINES
   
    echo "   Generation du graphique pour les 10 plus grandes usines..."
    
    # Tri: numérique en ordre inverse  puis prendre les 10 premières 
    tail -n +2 "$C_OUTPUT_FILE" | sort -t ';' -k $COLUMN -n -r | head -n 10 > "${HISTOGRAM_OUT_DIR}/top10_${OPTION}.dat"

    # Appel du script GnuPlot 
    ./"$PLOT_SCRIPT_FILE" "${HISTOGRAM_OUT_DIR}/top10_${OPTION}.dat" \
        "10 plus grandes usines - ${TITLE_Y}" \
        "$TITLE_Y" \
        "${HISTOGRAM_OUT_DIR}/top10_${OPTION}.png" 

    if [ $? -ne 0 ]; then cleanup_and_exit 7; fi
    
    echo "   Histogrammes generes dans le repertoire '$HISTOGRAM_OUT_DIR'."

# 5. TRAITEMENT DE LA COMMANDE 'leaks'


elif [ "$COMMANDE" == "leaks" ]; then
    echo "-> Execution de la commande 'leaks'..."

    # Validation de l'ID d'usine
    if [ -z "$OPTION" ]; then
        echo "ERREUR: Identifiant d'usine (ID) manquant pour la commande 'leaks'."
        cleanup_and_exit 5
    fi

    FACTORY_ID="$OPTION"
    
    echo "   Calcul du volume de pertes pour l'usine ID: $FACTORY_ID"

    # Appel du programme C pour calculer les fuites et mettre à jour le fichier d'historique
    "$EXECUTABLE" leaks "$LEAKS_OUT_FILE" "$FACTORY_ID"

    if [ $? -ne 0 ]; then
        echo "ATTENTION: Le programme C a retourne un code de retour non nul. Verifiez le fichier '$LEAKS_OUT_FILE'."
    fi

    echo "   Resultat ajoute/mis a jour dans le fichier d'historique: $LEAKS_OUT_FILE"


# 6. COMMANDE INCONNUE


else
    echo "ERREUR: Commande '$COMMANDE' inconnue."
    cleanup_and_exit 6
fi

# Fin du script et affichage de la durée
cleanup_and_exit 0
