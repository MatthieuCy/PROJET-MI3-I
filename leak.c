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
