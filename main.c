#include "avl.h"

int main(int argc, char *argv[]) {
    // Shell appelle : ./wildwater <mode> <fichier_entree> <option_ou_id> <fichier_sortie> donc 5 pour  (le nom du prog + 4 paramètres)
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <mode> <fichier_entree> <option_ou_id> <fichier_sortie>\n", argv[0]);
        return 1; 
    }

    char *mode = argv[1];
    char *fichier_entree = argv[2];
    char *param = argv[3];         // Pour histo : "max", "src" ou "real"  et Pour leaks : "ID_station"
    char *fichier_sortie = argv[4]; // Le nom du fichier .dat à créer

    // hisotgramme
    if (strcmp(mode, "histo") == 0) {
        
        // construit l'AVL des usines et on calcule les volumes (src ou max)
        AVL_Usine *racine = lire_donnees_et_construire_avl(fichier_entree);
        if (!racine) return 2;

        // Si le mode est 'real'
        if (strcmp(param, "real") == 0) {
            Graphe_Global *graphe = construire_graphe_distribution(fichier_entree);
            if (graphe) {
                //  parcourt usines AVL + calcule volume réel via le graphe
                calculer_tous_les_volumes_reels(racine, graphe);
                liberer_graphe_complet(graphe);
            }
        }

        // Génération de l'histogramme 
        if (generer_histogramme(racine, fichier_sortie, param) != 0) {
            liberer_avl_usine(racine);
            return 3;
        }
        
        liberer_avl_usine(racine);
    } 
    
    // leaks
    else if (strcmp(mode, "leaks") == 0) {
        // Chargement  usines pour  volumes de base (src)
        AVL_Usine *racine_usine = lire_donnees_et_construire_avl(fichier_entree);
        if (!racine_usine) return 2;

        // Construction du graphe de distribution pour le calcul des pertes
        Graphe_Global *graphe = construire_graphe_distribution(fichier_entree);
        if (!graphe) {
            liberer_avl_usine(racine_usine);
            return 3;
        }
    
        // Calcule  rendement d'une station précise et écrit dans le fichier
        double res = calculer_rendement_distribution(param, racine_usine, graphe, fichier_sortie);
        
        liberer_avl_usine(racine_usine);
        liberer_graphe_complet(graphe);
        
        if (res < 0) return 4; 
    } 
    
    // autre
    else {
        fprintf(stderr, "Erreur : Mode '%s' non reconnu.\n", mode);
        return 5;
    }

    return 0; // Succès
}

