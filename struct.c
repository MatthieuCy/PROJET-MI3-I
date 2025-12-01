typedef struct Usine {
    char *id;            // identifiant usine
    int capacité_max;    // en milliers de m3/an
    long volume-capté;     // volume total capté (k.m3)
    long volume_réel;    // volume réellement traité
} Usine;


typedef struct ArbreAVL_Usine {
    Usine donnees;                 // Données de l'usine
    int hauteur;                   // Hauteur du nœud dans l'arbre AVL
    struct ArbreAVL_Usine *gauche; // Sous-arbre gauche
    struct ArbreAVL_Usine *droite; // Sous-arbre droit
} ArbreAVL_Usine; // 
