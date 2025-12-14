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
