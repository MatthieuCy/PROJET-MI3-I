#include "avl.h"

int main(int argc, char *argv[]) {
    // Le script Shell doit envoyer les bons arguments
    // Exemple : ./wildwater <mode> <fichier_dat> <option/ID>
    if (argc < 4) {
        fprintf(stderr, "Erreur : Arguments insuffisants.\n");
        return 1; // Code erreur positif 
    }

    char *mode = argv[1];         // "histo" ou "leaks"
    char *fichier_donnees = argv[2];
    char *param = argv[3];        // option (max/src/real) ou ID usine

    if (strcmp(mode, "histo") == 0) {
        // Traitement Histogramme [cite: 412]
        AVL_Usine *racine = lire_donnees_et_construire_avl(fichier_donnees);
        if (!racine) return 2;
        
        // Le nom du fichier de sortie doit être unique selon l'option [cite: 431]
        char nom_sortie[100];
        sprintf(nom_sortie, "vol_%s.dat", param);
        
        generer_histogramme(racine, nom_sortie);
        liberer_avl_usine(racine); // Libération obligatoire [cite: 491, 534]
    } 
    else if (strcmp(mode, "leaks") == 0) {
        // Traitement Fuites [cite: 435]
        AVL_Usine *racine_usine = lire_donnees_et_construire_avl(fichier_donnees);
        Graphe_Global *graphe = construire_graphe_distribution(fichier_donnees);
        
        // calcul_rendement doit gérer le retour -1 si ID non trouvé [cite: 440]
        calculer_rendement_distribution(param, racine_usine, graphe, "leaks_history.dat");
        
        liberer_avl_usine(racine_usine);
        liberer_graphe_complet(graphe);
    }

    return 0; // Succès
}
