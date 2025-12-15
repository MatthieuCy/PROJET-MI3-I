typedef struct Troncon_Enfant {
    double fuite_pct;      // Pourcentage fuites du tronçon (colonne 5)
    struct Noeud_Acteur *acteur_aval; 
    struct Troncon_Enfant *suivant;
} Troncon_Enfant;

typedef struct Noeud_Acteur {
    char *id_acteur;            
    char *id_usine_parent;      
  
    double volume_entrant;      
    double volume_perdu_absolu; 
    
    struct Noeud_Acteur *parent; 
    Troncon_Enfant *troncons_aval; 
    int nombre_enfants;               
} Noeud_Acteur;

typedef struct Noeud_AVL_Recherche {
    char *id_acteur_key;          
    Noeud_Acteur *adresse_noeud;  
    
    int hauteur;                  
    struct Noeud_AVL_Recherche *gauche;
    struct Noeud_AVL_Recherche *droite;
} Noeud_AVL_Recherche;

typedef struct Graphe_Global {
    // La racine de l'AVL de recherche (pour trouver les noeuds par ID rapidement)
    Noeud_AVL_Recherche *racine_avl; 
    Noeud_Acteur *usine_cible;
} Graphe_Global;


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
    nouveau_noeud->hauteur = 1; 
    
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

int max(int a, int b) {
    return (a > b) ? a : b;
}

int avl_recherche_hauteur(Noeud_AVL_Recherche *n) {
    return n ? n->hauteur : 0;
}

Noeud_AVL_Recherche* rotation_droite(Noeud_AVL_Recherche *y) {
    Noeud_AVL_Recherche *x = y->gauche;
    Noeud_AVL_Recherche *T2 = x->droite;
    x->droite = y;
    y->gauche = T2;
    y->hauteur = 1 + max(avl_recherche_hauteur(y->gauche), avl_recherche_hauteur(y->droite));
    x->hauteur = 1 + max(avl_recherche_hauteur(x->gauche), avl_recherche_hauteur(x->droite));
    
    return x;
}

Noeud_AVL_Recherche* rotation_gauche(Noeud_AVL_Recherche *x) {
    Noeud_AVL_Recherche *y = x->droite;
    Noeud_AVL_Recherche *T2 = y->gauche;
    y->gauche = x;
    x->droite = T2;
    x->hauteur = 1 + max(avl_recherche_hauteur(x->gauche), avl_recherche_hauteur(x->droite));
    y->hauteur = 1 + max(avl_recherche_hauteur(y->gauche), avl_recherche_hauteur(y->droite));
    
    return y;
}

int avl_recherche_facteur_equilibre(Noeud_AVL_Recherche *n) {
    if(n == NULL){
        return 0;
    }
    return avl_recherche_hauteur(n->gauche) - avl_recherche_hauteur(n->droite);
}

Noeud_AVL_Recherche* equilibrer_noeud(Noeud_AVL_Recherche *n) {
    if (!n) return n;

    n->hauteur = 1 + max(avl_recherche_hauteur(n->gauche), avl_recherche_hauteur(n->droite));
    int eq = avl_recherche_facteur_equilibre(n);

    if (eq > 1 && avl_recherche_facteur_equilibre(n->gauche) >= 0)
        return rotation_droite(n);
    if (eq > 1 && avl_recherche_facteur_equilibre(n->gauche) < 0) {
        n->gauche = rotation_gauche(n->gauche);
        return rotation_droite(n);
    }
    if (eq < -1 && avl_recherche_facteur_equilibre(n->droite) <= 0)
        return rotation_gauche(n);
    if (eq < -1 && avl_recherche_facteur_equilibre(n->droite) > 0) {
        n->droite = rotation_droite(n->droite);
        return rotation_gauche(n);
    }
    return n;
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

Noeud_AVL_Recherche* inserer_avl(Noeud_AVL_Recherche *noeud, const char *id_acteur_key, Noeud_Acteur *adresse_noeud) {
    if (noeud == NULL) {
        return creer_noeud_avl(id_acteur_key, adresse_noeud);
    }
    
    int comparaison = strcmp(id_acteur_key, noeud->id_acteur_key);

    if (comparaison < 0) {
        noeud->gauche = inserer_avl(noeud->gauche, id_acteur_key, adresse_noeud);
    } else if (comparaison > 0) {
        noeud->droite = inserer_avl(noeud->droite, id_acteur_key, adresse_noeud);
    } else {
        return noeud;
    }

    return equilibrer_noeud(noeud);
}

Graphe_Global* construire_graphe_distribution() {

    Graphe_Global *graphe = (Graphe_Global*) malloc(sizeof(Graphe_Global));
    if (graphe == NULL) {
        printf("ERREUR: Allocation memoire pour Graphe_Global.\n");
        return NULL;
    }
    graphe->racine_avl = NULL;
    graphe->usine_cible = NULL;

    FILE *fic = fopen(FICHIER_DONNEES, "r");
    if (fic == NULL) {
        printf("ERREUR: Le fichier %s est introuvable.\n", FICHIER_DONNEES);
        free(graphe);
        return NULL;
    }

    char line[MAX_LINE_SIZE];
    char c1[MAX_CHAMP_SIZE], c2[MAX_CHAMP_SIZE], c3[MAX_CHAMP_SIZE], c4[MAX_CHAMP_SIZE], c5[MAX_CHAMP_SIZE];


    if (fgets(line, sizeof(line), fic) == NULL) {
        fclose(fic);
        return graphe;
    }

    while (fgets(line, sizeof(line), fic)) {
        if (sscanf(line, "%99[^;];%99[^;];%99[^;];%99[^;];%99[^\n]", c1, c2, c3, c4, c5) != 5) {
            continue; 
        }
        
        
        if (strcmp(c1, "-") != 0 && strcmp(c5, "-") != 0) {
            Noeud_Acteur *parent_acteur = rechercher_avl(graphe->racine_avl, c2);
            if (parent_acteur == NULL) {
                parent_acteur = creer_noeud_acteur(c2, c1);
                if (parent_acteur == NULL) { 
                    break; 
                }
                graphe->racine_avl = inserer_avl(graphe->racine_avl, c2, parent_acteur);
                if (graphe->racine_avl == NULL) { 
                    break;
                }
            }
         }

            graphe->racine_avl = inserer_avl(graphe->racine_avl, c3, enfant_acteur);
            if (graphe->racine_avl == NULL) { 
                break;
            }
            double fuite_pct = atof(c5);
            if (ajouter_troncon_aval(parent_acteur, enfant_acteur, fuite_pct) != 0) {
                break;
            }
        } 
    }

    fclose(fic);
    return graphe;
}
