#include "avl.h"


static int max(int a, int b) {
    return (a > b) ? a : b;
}


static int min(int a, int b) {
    return (a < b) ? a : b;
}

static int maxi_trois(int a, int b, int c) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

// Fonction pour trouver le minimum entre trois entiers
static int mini_trois(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}


Noeud_AVL_Recherche* creer_noeud_avl(const char *id_acteur_key, Noeud_Acteur *adresse_noeud) {
    Noeud_AVL_Recherche *nouveau_noeud = (Noeud_AVL_Recherche*) malloc(sizeof(Noeud_AVL_Recherche));
    
    if (nouveau_noeud == NULL) {
        return NULL;
    }
    
    nouveau_noeud->id_acteur_key = strdup(id_acteur_key);
    if (nouveau_noeud->id_acteur_key == NULL) {
        free(nouveau_noeud);
        return NULL;
    }

    nouveau_noeud->adresse_noeud = adresse_noeud;
    nouveau_noeud->gauche = NULL;
    nouveau_noeud->droite = NULL;
    nouveau_noeud->equilibre = 0; 
    
    return nouveau_noeud;
}


Noeud_Acteur* rechercher_avl(Noeud_AVL_Recherche *racine, const char *id_acteur_key) {
    if (racine == NULL) {
        return NULL;
    }

    int comparaison = strcmp(id_acteur_key, racine->id_acteur_key);

    if (comparaison < 0) {
        return rechercher_avl(racine->gauche, id_acteur_key);
    } else if (comparaison > 0) {
        return rechercher_avl(racine->droite, id_acteur_key);
    } else {
        return racine->adresse_noeud;
    }
}


int avl_recherche_hauteur(Noeud_AVL_Recherche *n) {
    if (n == NULL)
        return 0;

    int hg = avl_recherche_hauteur(n->gauche);
    int hd = avl_recherche_hauteur(n->droite);

    if (hg > hd)
        return 1 + hg;
    else
        return 1 + hd;
}


int avl_recherche_facteur_equilibre(Noeud_AVL_Recherche *n) {
    if(n == NULL){
        return 0;
    }
    return avl_recherche_hauteur(n->gauche) - avl_recherche_hauteur(n->droite);
}


Noeud_AVL_Recherche* rotation_droite_graphe(Noeud_AVL_Recherche *a) {
    if (a == NULL || a->gauche == NULL) {
        return a;
    }

    Noeud_AVL_Recherche *pivot;
    int eq_a, eq_p;

    pivot = a->gauche;
    a->gauche = pivot->droite;
    pivot->droite = a;

    eq_a = a->equilibre;
    eq_p = pivot->equilibre;

    a->equilibre = eq_a - min(eq_p, 0) + 1;
    pivot->equilibre = maxi_trois(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);

    a = pivot;
    return a;
}


Noeud_AVL_Recherche* rotation_gauche_graphe(Noeud_AVL_Recherche *a) {
    if (a == NULL || a->droite == NULL) {
        return a;
    } 
    
    Noeud_AVL_Recherche *pivot;
    int eq_a, eq_p;

    pivot = a->droite;
    a->droite = pivot->gauche;
    pivot->gauche = a;

    eq_a = a->equilibre;
    eq_p = pivot->equilibre;

    a->equilibre = eq_a - max(eq_p, 0) - 1;
    pivot->equilibre = mini_trois(eq_a - 2, eq_a + eq_p - 2, eq_p - 1);

    a = pivot;
    return a;
}


Noeud_AVL_Recherche* double_rotation_gauche_graphe(Noeud_AVL_Recherche *a) {
    if (a == NULL || a->droite == NULL) {
        return a;
    }
    
    a->droite = rotation_droite_graphe(a->droite);
    return rotation_gauche_graphe(a);
}


Noeud_AVL_Recherche* double_rotation_droite_graphe(Noeud_AVL_Recherche *a) {
    if (a == NULL || a->gauche == NULL) {
        return a;
    }
    
    a->gauche = rotation_gauche_graphe(a->gauche);
    return rotation_droite_graphe(a);
}


Noeud_AVL_Recherche* equilibrer_graphe(Noeud_AVL_Recherche *noeud) {
    if (noeud == NULL) {
        return NULL;
    }
    
    if (noeud->equilibre >= 2) {
        if (noeud->droite->equilibre >=0) {
             return rotation_gauche_graphe(noeud);
        } 
        else {
            return double_rotation_gauche_graphe(noeud);
        }
    } 
    else if (noeud->equilibre <= -2) {
        if(noeud->gauche->equilibre <=0) {
             return rotation_droite_graphe(noeud);
        }
        else {
             return double_rotation_droite_graphe(noeud);
        }     
    }
    return noeud;
}  


Noeud_Acteur* creer_noeud_acteur(const char *id, const char *id_usine) {
    if (id == NULL || id_usine == NULL) {
    return NULL;
    }
    Noeud_Acteur *acteur = (Noeud_Acteur*) malloc(sizeof(Noeud_Acteur));
    if (acteur == NULL) {
        return NULL; 
    }
    acteur->id_acteur = strdup(id);
    acteur->id_usine_parent = strdup(id_usine);
    
    if (acteur->id_acteur == NULL || acteur->id_usine_parent == NULL) {
        free(acteur->id_acteur);
        free(acteur->id_usine_parent);
        free(acteur);
        return NULL;
    }

    acteur->volume_entrant = 0.0;
    acteur->volume_perdu_absolu = 0.0;
   
    acteur->parent = NULL; 
    acteur->troncons_aval = NULL; 
    acteur->nombre_enfants = 0;
    
    return acteur;
}


int ajouter_troncon_aval(Noeud_Acteur *parent, Noeud_Acteur *enfant, double fuite_pct) {
    Troncon_Enfant *nouveau_troncon = (Troncon_Enfant*) malloc(sizeof(Troncon_Enfant));
    if (nouveau_troncon == NULL) {
        return -1;
    }

    nouveau_troncon->fuite_pct = fuite_pct;
    nouveau_troncon->acteur_aval = enfant;
    
    if (enfant->parent != NULL) {
    }
    enfant->parent = parent;
    nouveau_troncon->suivant = parent->troncons_aval;
    parent->troncons_aval = nouveau_troncon;
    parent->nombre_enfants++;
    
    return 0; // Succès
}


Noeud_AVL_Recherche* avl_inserer_graphe(Noeud_AVL_Recherche *noeud, const char *id_acteur_key, Noeud_Acteur *adresse_noeud, int *h) {
    if (noeud == NULL) {
        *h = 1;
        return creer_noeud_avl(id_acteur_key, adresse_noeud);
    }
    
    int comparaison = strcmp(id_acteur_key, noeud->id_acteur_key);

    if (comparaison < 0) {
        noeud->gauche = avl_inserer_graphe(noeud->gauche, id_acteur_key, adresse_noeud, h);
        *h = -(*h);
    } else if (comparaison > 0) {
        noeud->droite = avl_inserer_graphe(noeud->droite, id_acteur_key, adresse_noeud, h);
    } else {
        *h = 0;
        return noeud;
    }

    if (*h != 0) {
        noeud->equilibre += *h;
        noeud = equilibrer_graphe(noeud);
        if (noeud->equilibre == 0) {
            *h = 0;
        }
        else {
            *h = 1;
        }
    }
    return noeud;
}


Graphe_Global* construire_graphe_distribution() {
    Graphe_Global *graphe = (Graphe_Global*) malloc(sizeof(Graphe_Global));
    if (graphe == NULL) {
        printf("erreur echec d'allocution memoire \n");
        return NULL;
    }
    graphe->racine_avl = NULL;
    graphe->usine_cible = NULL;

    FILE *fic = fopen(FICHIER_DONNEES, "r");
    if (fic == NULL) {
        fprintf(stderr, "ERREUR: Impossible d'ouvrir le fichier de données %s.\n", FICHIER_DONNEES);
        free(graphe);
        return NULL;
    }

    char line[MAX_LINE_SIZE];
    char c1[MAX_CHAMP_SIZE], c2[MAX_CHAMP_SIZE], c3[MAX_CHAMP_SIZE], c4[MAX_CHAMP_SIZE], c5[MAX_CHAMP_SIZE];
    int h = 0;
 
    if (fgets(line, sizeof(line), fic) == NULL) {
        fclose(fic);
        return graphe;
    }

    //  Lecture ligne par ligne et construction du graphe
    while (fgets(line, sizeof(line), fic)) {
        // Lire 5 champs séparés par des ';'
        if (sscanf(line, "%99[^;];%99[^;];%99[^;];%99[^;];%99[^\n]", c1, c2, c3, c4, c5) != 5) {
            continue; 
        }
        

        if (strcmp(c1, "-") == 0 && strcmp(c5, "-") == 0) {
            continue;
        }

        // Cas 1 : Usine -> Stockage 
        else if (strcmp(c1, "-") != 0 && strcmp(c2, "Facility complex") == 0 && strcmp(c5, "-") != 0) {
            
            Noeud_Acteur *stockage = creer_noeud_acteur(c3, c1); 
            if (stockage == NULL) break;
            
            h = 0;
            graphe->racine_avl = avl_inserer_graphe(graphe->racine_avl, c3, stockage, &h);
            if (graphe->racine_avl == NULL) break;
        
        }
        
        // Cas 2 : Autres tronçons 
        else if (strcmp(c1, "-") != 0 && strcmp(c5, "-") != 0) {
        
            Noeud_Acteur *parent_acteur = rechercher_avl(graphe->racine_avl, c2);
            
            if (parent_acteur == NULL) { 
                continue;
            }


            Noeud_Acteur *enfant_acteur = creer_noeud_acteur(c3, c1);
            if (enfant_acteur == NULL) break;

            h = 0;
            graphe->racine_avl = avl_inserer_graphe(graphe->racine_avl, c3, enfant_acteur, &h);
            if (graphe->racine_avl == NULL) break;

        
            double fuite_pct = atof(c5);
            if (ajouter_troncon_aval(parent_acteur, enfant_acteur, fuite_pct) != 0) break;
        } 
        
   
    }
    
    fclose(fic);
    return graphe;
}


int compter_stockages(Noeud_AVL_Recherche *avl_noeud_recherche, const char *id_usine_cible) {
    if (avl_noeud_recherche == NULL) {
        return 0;
    }

    int count = 0;
    Noeud_Acteur *acteur = avl_noeud_recherche->adresse_noeud;
    
    // si stockage et appartient à l'usine 
    if (strncmp(acteur->id_acteur, "Storage", 7) == 0 && strcmp(acteur->id_usine_parent, id_usine_cible) == 0) {
        count = 1;
    }

    // parcours infixe
    count += compter_stockages(avl_noeud_recherche->gauche, id_usine_cible);
    count += compter_stockages(avl_noeud_recherche->droite, id_usine_cible);

    return count;
}



void propager_volume_et_calculer_pertes(Noeud_Acteur *noeud_courant, double volume_entrant_total, double *total_pertes) {

    if (noeud_courant == NULL || noeud_courant->nombre_enfants == 0) {
        return; 
    }

    // répartition equitable du volume entre les enfants
    double volume_par_troncon = volume_entrant_total / (double)noeud_courant->nombre_enfants;

    Troncon_Enfant *troncon = noeud_courant->troncons_aval;

    while (troncon != NULL) {

        double fuite_pct = troncon->fuite_pct;
        double volume_perdu_troncon = volume_par_troncon * (fuite_pct / 100.0);
        double volume_transmis_troncon = volume_par_troncon - volume_perdu_troncon;

        *total_pertes += volume_perdu_troncon;

        propager_volume_et_calculer_pertes(troncon->acteur_aval, volume_transmis_troncon, total_pertes);

        troncon = troncon->suivant;
    }
}


void parcourir_stockages_et_propager(Noeud_AVL_Recherche *avl_noeud_recherche, const char *id_usine_cible, double volume_par_stockage, double *total_pertes) {
    if (avl_noeud_recherche == NULL) {
        return;
    }

    parcourir_stockages_et_propager(avl_noeud_recherche->gauche, id_usine_cible, volume_par_stockage, total_pertes);

    Noeud_Acteur *acteur = avl_noeud_recherche->adresse_noeud;

    // "Storage" a 7 caractères , strncmp compare les N premiers caractères de deux chaînes
    if (strncmp(acteur->id_acteur, "Storage", 7) == 0 && strcmp(acteur->id_usine_parent, id_usine_cible) == 0) {
        propager_volume_et_calculer_pertes(acteur, volume_par_stockage, total_pertes);
    }

    parcourir_stockages_et_propager(avl_noeud_recherche->droite, id_usine_cible, volume_par_stockage, total_pertes);
}



double calculer_rendement_distribution(const char *id_usine, AVL_Usine *racine_usine_avl, Graphe_Global *graphe, const char *nom_fichier_sortie) {
    
    AVL_Usine *noeud_usine = avl_rechercher_usine(racine_usine_avl, id_usine);
    double volume_depart_km3 = 0.0;
    double total_pertes_km3 = 0.0; // Accumulation en k.m³ (milliers de m³)

    if (noeud_usine != NULL) {
        volume_depart_km3 = noeud_usine->donnees.volume_reel;
        
        int nb_stockages = compter_stockages(graphe->racine_avl, id_usine);

        if (nb_stockages > 0) {
            double volume_par_stockage = volume_depart_km3 / (double)nb_stockages;
            parcourir_stockages_et_propager(graphe->racine_avl, id_usine, volume_par_stockage, &total_pertes_km3);
        }
    }

    FILE *fic_out = fopen(nom_fichier_sortie, "a+");
    if (fic_out == NULL) { 
        printf("ERREUR: Impossible d'ouvrir le fichier de sortie %s.\n", nom_fichier_sortie);
        return -2.0; 
    }

    fseek(fic_out, 0, SEEK_END);
    if (ftell(fic_out) == 0) {
        fprintf(fic_out, "identifiant;Volume_perdu (M.m3.year-1)\n");
    }
    
    double volume_perdu_mm3 = total_pertes_km3 / CONVERSION_KM3_TO_MM3;

    if (noeud_usine == NULL) {
        fprintf(fic_out, "%s;-1.00\n", id_usine);
        fclose(fic_out);
        return -1.0;
    } else {
        fprintf(fic_out, "%s;%.2f\n", id_usine, volume_perdu_mm3); 
    }
    
    fclose(fic_out);

    return volume_perdu_mm3;
}


void liberer_avl_recherche(Noeud_AVL_Recherche *n) {
    if (n == NULL) return;
    liberer_avl_recherche(n->gauche);
    liberer_avl_recherche(n->droite);

    if (n->adresse_noeud != NULL) {
        Troncon_Enfant *courant = n->adresse_noeud->troncons_aval;
        while (courant != NULL) {
            Troncon_Enfant *temp = courant;
            courant = courant->suivant;
            free(temp);
        }
        free(n->adresse_noeud->id_acteur);
        free(n->adresse_noeud->id_usine_parent);
        free(n->adresse_noeud);
    }
    free(n->id_acteur_key);
    free(n);
}


void liberer_graphe_complet(Graphe_Global *graphe) {
    if (graphe == NULL) return;
    liberer_avl_recherche(graphe->racine_avl);
    free(graphe);
}


