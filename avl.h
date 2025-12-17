#ifndef AVL_USINE_H
#define AVL_USINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

typedef struct Usine {
    char *id;               // identifiant usine 
    double capacite_max;       // capacité maximale 
    double volume_capte;      // volume total capté 
    double volume_reel;       // volume réellement traité 
} Usine;

typedef struct AVL_Usine {
    Usine donnees;                // données associées à l’usine
    int hauteur;                  // hauteur du nœud AVL
    struct AVL_Usine *gauche;    // sous-arbre gauche
    struct AVL_Usine *droite;    // sous-arbre droit
} AVL_Usine;

struct Arbre_Graphe_AVL;



typedef struct Chainon_Troncon {
    double fuite_pct;                 // Pourcentage de fuite sur ce tronçon
    struct Arbre_Noeud *cible;        // Pointeur vers le nœud aval
    struct Chainon_Troncon *suivant;  // Maillon suivant
} Chainon_Troncon;

// 2. Noeud du Graphe (Sommet du graphe)

typedef struct Arbre_Noeud {
    char *id;                          // ID de l'installation 
    Chainon_Troncon *troncons_aval;    // Liste des tronçons  sortants
} Arbre_Noeud;

// 3. Structure AVL pour indexer les Nœuds du Graphe

typedef struct Arbre_Graphe_AVL {
    char *id;                          // ID de l'installation (clé de l'AVL)
    Arbre_Noeud *noeud;                // Pointeur vers le nœud réel (la donnée)
    int hauteur;
    struct Arbre_Graphe_AVL *gauche;
    struct Arbre_Graphe_AVL *droite;
} Arbre_Graphe_AVL;

// Fonctions principales 




// Fonctions de gestion des noeuds et tronçons (Graphe)
Arbre_Noeud *creer_arbre_noeud(char *id);
Chainon_Troncon *creer_chainon_troncon(double fuite_pct, Arbre_Noeud *cible);
int ajouter_troncon(Arbre_Noeud *source, Arbre_Graphe_AVL *racine_avl, char *cible_id, double fuite_pct);

// Fonctions de manipulation de l'AVL du Graphe
int avl_graphe_hauteur(Arbre_Graphe_AVL *n);
Arbre_Graphe_AVL *avl_graphe_nouveau_noeud(char *id, Arbre_Noeud *noeud);
Arbre_Graphe_AVL *rotation_droite(Arbre_Graphe_AVL *y);
Arbre_Graphe_AVL *rotation_gauche(Arbre_Graphe_AVL *x);
int avl_graphe_facteur_equilibre(Arbre_Graphe_AVL *n);
Arbre_Graphe_AVL *equilibrer_graphe(Arbre_Graphe_AVL *n); // Renommée pour éviter conflit
Arbre_Graphe_AVL *avl_graphe_insertion(Arbre_Graphe_AVL *racine, char *id, Arbre_Noeud *noeud);
Arbre_Noeud *avl_graphe_recherche(Arbre_Graphe_AVL *racine, char *id);
Arbre_Graphe_AVL *creer_et_inserer_noeud(Arbre_Graphe_AVL *racine_avl, char *id_noeud);

// Fonctions de libération de mémoire du Graphe
void supprimer_troncons(Chainon_Troncon *tete);
void supprimer_arbre_noeud(Arbre_Noeud *n);
void avl_graphe_suppression(Arbre_Graphe_AVL *racine);
void supprimer_noeuds_depuis_avl(Arbre_Graphe_AVL *racine);
void supprimer_graphe_complet(Arbre_Graphe_AVL *racine_avl);


// PROTOTYPES - AVL_USINE.C 

// Fonctions de gestion de l'AVL des Usines
Usine creer_usine(const char *id_source, double capacite);
AVL_Usine *creer_noeud_usine(Usine usine_donnees);
int hauteur_noeud(AVL_Usine *noeud);
int get_facteur_equilibre(AVL_Usine *noeud);
AVL_Usine *rotation_gauche_usine(AVL_Usine *x); 
AVL_Usine *rotation_droite_usine(AVL_Usine *y); 
AVL_Usine *equilibrer_usine(AVL_Usine *noeud);  
AVL_Usine *avl_inserer(AVL_Usine *racine, Usine u);
AVL_Usine *avl_rechercher(AVL_Usine *racine, const char *id);
void avl_supprimer(AVL_Usine *racine); // Libération mémoire

// Fonctions de lecture et sortie
AVL_Usine *lire_donnees_et_construire_avl();
int generer_histogramme(AVL_Usine *racine, const char *nom_fichier_sortie);




double calculer_fuites(Arbre_Graphe_AVL *racine_graphe_avl, const char *id_usine);
int historique_fuites(const char *nom_fichier, const char *id_usine, double fuites_pct);
void lire_donnees_et_construire_graphe(Arbre_Graphe_AVL **racine_graphe_avl, AVL_Usine *racine_usine_avl);




#endif

