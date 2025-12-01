typedef struct Usine {
    char *id;            // identifiant usine
    int capacité_max;    // en milliers de m3/an
    long volume-capté;     // volume total capté (k.m3)
    long volume_réel;    // volume réellement traité
} Usine;


typedef struct AVL_Usine {
    Usine donnees;                 // Données de l'usine
    int hauteur;                   // Hauteur du nœud dans l'arbre AVL
    struct AVL_Usine *gauche; // Sous-arbre gauche
    struct AVL_Usine *droite; // Sous-arbre droit
} AVL_Usine; // 
