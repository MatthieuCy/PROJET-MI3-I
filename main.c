#include "avl.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage: %s <mode> <fichier_entree> <option_ou_id>\n", argv[0]);
        return 1; // Code erreur positif requis par le PDF
    }

    char *mode = argv[1];
    char *fichier_entree = argv[2]; // Devient dynamique
    char *param = argv[3];

    if (strcmp(mode, "histo") == 0) {
        // On passe le nom du fichier à la fonction de lecture
        AVL_Usine *racine = lire_donnees_et_construire_avl(fichier_entree);
        if (!racine) return 2;

        if (generer_histogramme(racine, param) != 0) {
            liberer_avl_usine(racine);
            return 3;
        }
        liberer_avl_usine(racine);
    } 
    else if (strcmp(mode, "leaks") == 0) {
        AVL_Usine *racine_usine = lire_donnees_et_construire_avl(fichier_entree);
        // On passe aussi le fichier au graphe
        Graphe_Global *graphe = construire_graphe_distribution(fichier_entree);
        
        // calculer_rendement_distribution doit écrire "ID;-1.00" si ID non trouvé
        double res = calculer_rendement_distribution(param, racine_usine, graphe, "leaks_history.dat");
        
        liberer_avl_usine(racine_usine);
        liberer_graphe_complet(graphe);
        
        if (res < 0) return 4; // Erreur si l'ID n'existe pas
    } else {
        return 5;
    }

    return 0; // Succès
}
