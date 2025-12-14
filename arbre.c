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
