typedef struct Chainon_Troncon {
    double fuite_pct;                
    struct Arbre_Noeud *cible;          
    struct Chainon_Troncon *suivant;    
} Chainon_Troncon;


typedef struct Arbre_Noeud {
    char *id;                           
    Chainon_Troncon *troncons_aval;    
} Arbre_Noeud;


typedef struct Arbre_Graphe_AVL {
    char *id;                           
    Arbre_Noeud *noeud;                 
    int hauteur;
    struct Arbre_Graphe_AVL *gauche;
    struct Arbre_Graphe_AVL *droite;
} Arbre_Graphe_AVL;

int maxi(int a, int b){
    if(a>=b)    return a;
    return b;
}


int avl_graphe_hauteur(Arbre_Graphe_AVL *n) {
    return n ? n->hauteur : 0;
}

Arbre_Graphe_AVL *avl_graphe_nouveau_noeud(char *id, Arbre_Noeud *noeud) {
    Arbre_Graphe_AVL *n = malloc(sizeof(*n));
    if (!n) return NULL;

    n->id = strdup(id);
    n->noeud = noeud;
    n->gauche = n->droite = NULL;
    n->hauteur = 1;
    return n;
}

Arbre_Graphe_AVL *rotation_droite(Arbre_Graphe_AVL *y) {
    Arbre_Graphe_AVL *x = y->gauche;
    Arbre_Graphe_AVL *T2 = x->droite;
    x->droite = y;
    y->gauche = T2;
    y->hauteur = 1 + maxi(avl_graphe_hauteur(y->gauche),avl_graphe_hauteur(y->droite));
    x->hauteur = 1 + maxi(avl_graphe_hauteur(x->gauche),avl_graphe_hauteur(x->droite))
    return x;
}

Arbre_Graphe_AVL *rotation_gauche(Arbre_Graphe_AVL *x) {
    Arbre_Graphe_AVL *y = x->droite;
    Arbre_Graphe_AVL *T2 = y->gauche;
    y->gauche = x;
    x->droite = T2;
    x->hauteur = 1 + maxi(avl_graphe_hauteur(x->gauche), avl_graphe_hauteur(x->droite));
    y->hauteur = 1 + maxi(avl_graphe_hauteur(y->gauche), avl_graphe_hauteur(y->droite));
    return y;
}

int avl_graphe_facteur_equilibre(Arbre_Graphe_AVL *n) {
    if(n == NULL){
       return 0;
    }
    return  avl_graphe_hauteur(n->gauche)   - avl_graphe_hauteur(n->droite);
}

Arbre_Graphe_AVL *equilibrer(Arbre_Graphe_AVL *n) {
    if (!n) return n;

    n->hauteur = 1 + maxi(avl_graphe_hauteur(n->gauche),avl_graphe_hauteur(n->droite));
    int eq = avl_graphe_facteur_equilibre(n);

    if (eq > 1 && avl_graphe_facteur_equilibre(n->gauche) >= 0)
        return rotation_droite(n);

    if (eq > 1 && avl_graphe_facteur_equilibre(n->gauche) < 0) {
        n->gauche = rotation_gauche(n->gauche);
        return rotation_droite(n);
    }

    if (eq < -1 && avl_graphe_facteur_equilibre(n->droite) <= 0)
        return rotation_gauche(n);

    if (eq < -1 && avl_graphe_facteur_equilibre(n->droite) > 0) {
        n->droite = rotation_droite(n->droite);
        return rotation_gauche(n);
    }

    return n;
}

Arbre_Graphe_AVL *avl_graphe_insertion(Arbre_Graphe_AVL *racine, char *id, Arbre_Noeud *noeud) {
    if (!racine)
        return avl_graphe_nouveau_noeud(id, noeud);

    int cmp = strcmp(id, racine->id);

    if (cmp < 0)
        racine->gauche = avl_graphe_insertion(racine->gauche, id, noeud);
    else if (cmp > 0)
        racine->droite = avl_graphe_insertion(racine->droite, id, noeud);
    else
        return racine;

    return equilibrer(racine);
}

Arbre_Noeud *avl_graphe_recherche(Arbre_Graphe_AVL *racine, char *id) {
    if (!racine) return NULL;

    int cmp = strcmp(id, racine->id);
    if (cmp == 0) return racine->noeud;
    if (cmp < 0)  return avl_graphe_recherche(racine->gauche, id);
    return avl_graphe_recherche(racine->droite, id);
}

Arbre_Noeud *creer_arbre_noeud(char *id) {
    Arbre_Noeud *n = (Arbre_Noeud *)malloc(sizeof(Arbre_Noeud));
    if (n== NULL){
        return NULL;
    } 
    n->id = strdup(id);
    n->troncons_aval = NULL; 
    return n;
}

Arbre_Graphe_AVL *creer_et_inserer_noeud(Arbre_Graphe_AVL *racine_avl, char *id_noeud) {
    Arbre_Noeud *existant = avl_graphe_recherche(racine_avl, id_noeud);
    if (existant) {
        return racine_avl;  // déjà présent
    }

    Arbre_Noeud *nouveau = creer_arbre_noeud(id_noeud);
    if (!nouveau) return racine_avl;

    return avl_graphe_insertion(racine_avl, id_noeud, nouveau);
}


Chainon_Troncon *creer_chainon_troncon(double fuite_pct, Arbre_Noeud *cible) {
    Chainon_Troncon *troncon = (Chainon_Troncon *)malloc(sizeof(Chainon_Troncon));
    if (troncon == NULL){
        return NULL; 
    }
    troncon->fuite_pct = fuite_pct;
    troncon->cible = cible; 
    troncon->suivant = NULL;
    return troncon;
}


int ajouter_troncon(Arbre_Noeud *source, Arbre_Graphe_AVL *racine_avl, char *cible_id, double fuite_pct) {
    if (!source || !racine_avl) {
        return -1; 
    }
    
    Arbre_Noeud *cible = avl_graphe_recherche(racine_avl, cible_id);
    
    if (cible == NULL) {
        printf("Erreur : Nœud cible '%s' introuvable dans l'index AVL.\n", cible_id);
        return -1;
    }

    Chainon_Troncon *nouveau_troncon = creer_chainon_troncon(fuite_pct, cible);
    if (!nouveau_troncon) {
        return -1; 
    }
   
    nouveau_troncon->suivant = source->troncons_aval;
    source->troncons_aval = nouveau_troncon;

    return 0; 
}


void supprimer_troncons(Chainon_Troncon *tete) {
    Chainon_Troncon *courant = tete;
    Chainon_Troncon *suivant = NULL;
    
    while (courant != NULL) {
        suivant = courant->suivant;
        free(courant);
        courant = suivant;
    }
}

void supprimer_arbre_noeud(Arbre_Noeud *n) {
    if (n == NULL) return;

    supprimer_troncons(n->troncons_aval);
    if (n->id != NULL) {
        free(n->id);
    }
    free(n);
}

void avl_graphe_suppression(Arbre_Graphe_AVL *racine) {
    if (racine == NULL) return;

    avl_graphe_suppression(racine->gauche);
    avl_graphe_suppression(racine->droite);

    if (racine->id != NULL) {
        free(racine->id);
    }

    free(racine);
}

void supprimer_noeuds_depuis_avl(Arbre_Graphe_AVL *racine) {
    if (racine == NULL) return;

    supprimer_noeuds_depuis_avl(racine->gauche);
    supprimer_noeuds_depuis_avl(racine->droite);

    supprimer_arbre_noeud(racine->noeud);
}


void supprimer_graphe_complet(Arbre_Graphe_AVL *racine_avl) {
    if (racine_avl == NULL) return;

    supprimer_noeuds_depuis_avl(racine_avl);
    avl_graphe_suppression(racine_avl);
}
