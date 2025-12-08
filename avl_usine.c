#include "AVL_USINE.H"


Usine creer_usine(const char *id, int capacite){
Usine u;
u.id=id;
u.capacite_max=capacite;
u.volume_capte = 0;
u.volume_reel = 0;

return u;
}





AVL_Usine *avl_inserer(AVL_Usine *racine, Usine u) {
    if (racine == NULL) {
        /
        return creer_usine(u);
    }

    int comparaison = strcmp(u.id, racine->donnees.id);

    if (comparaison < 0) {
        racine->gauche = avl_inserer(racine->gauche, u);
    } else if (comparaison > 0) {
        racine->droite = avl_inserer(racine->droite, u);
    } else {
        // ID déjà existant : mise à jour des données (ou ignorer)
        // Libérer l'ID alloué dans u, car on utilise celui du nœud existant.
        free(u.id); 
        // Mise à jour des autres données si nécessaire (ici on ignore)
        return racine; 
    }

    // 2. Équilibrage de l'arbre
    return equilibrer(racine);
}

AVL_Usine *avl_rechercher(AVL_Usine *racine, const char *id) {
    if (racine == NULL) {
        return NULL; // Usine non trouvée
    }

    int comparaison = strcmp(id, racine->donnees.id);

    if (comparaison == 0) {
        return racine; // Usine trouvée
    } else if (comparaison < 0) {
        return avl_rechercher(racine->gauche, id); // Rechercher dans le sous-arbre gauche
    } else { // comparaison > 0
        return avl_rechercher(racine->droite, id); // Rechercher dans le sous-arbre droit
    }
}





}
void avl_supprimer(AVL_Usine *racine) {
    if (racine == NULL) {
        return;
    }

    // Parcours en postfixe : Gauche, Droite, Racine
    avl_supprimer(racine->gauche);
    avl_supprimer(racine->droite);

    // 1. Libérer l'ID de l'usine
    free(racine->donnees.id);
    
    // 2. Libérer le nœud AVL
    free(racine);
}
  
  
