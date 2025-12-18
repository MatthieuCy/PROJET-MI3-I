#ifndef AVL_USINE_H
#define AVL_USINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define MAX_CHAMP_SIZE 100
#define MAX_LINE_SIZE 1000
#define FICHIER_DONNEES "c-wildwater_v3.dat"
#define CONVERSION_KM3_TO_MM3 1000.0

typedef struct Usine {
    char *id;               // identifiant usine 
    double capacite_max;       // capacité maximale 
    double volume_capte;      // volume total capté 
    double volume_reel;       // volume réellement traité 
} Usine;

typedef struct AVL_Usine {
    Usine donnees;                // données associées à l’usine
    int equilibre;                // equilibre du nœud AVL
    struct AVL_Usine *gauche;    // sous-arbre gauche
    struct AVL_Usine *droite;    // sous-arbre droit
} AVL_Usine;

struct Arbre_Noeud;


typedef struct Chainon_Troncon {
    double fuite_pct;                 // Pourcentage de fuite sur ce tronçon
    struct Arbre_Noeud *cible;        // Pointeur vers le nœud aval
    struct Chainon_Troncon *suivant;  // Maillon suivant
} Chainon_Troncon;


typedef struct Arbre_Noeud {
    char *id;                          // ID de l'installation 
    Chainon_Troncon *troncons_aval;    // Liste des tronçons  sortants
} Arbre_Noeud;


typedef struct Arbre_Graphe_AVL {
    char *id;                          // ID de l'installation (clé de l'AVL)
    Arbre_Noeud *noeud;                // Pointeur vers le nœud réel (la donnée)
    int equilibre;
    struct Arbre_Graphe_AVL *gauche;
    struct Arbre_Graphe_AVL *droite;
} Arbre_Graphe_AVL;


struct Noeud_Acteur;


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
    int equilibre;                  
    struct Noeud_AVL_Recherche *gauche;
    struct Noeud_AVL_Recherche *droite;
} Noeud_AVL_Recherche;

typedef struct Graphe_Global {
    // La racine de l'AVL de recherche (pour trouver les noeuds par ID rapidement)
    Noeud_AVL_Recherche *racine_avl; 
    Noeud_Acteur *usine_cible;
} Graphe_Global;

// Fonctions principales 




// Fonctions de gestion des noeuds et tronçons (Graphe)
Arbre_Noeud *creer_arbre_noeud(char *id);
Chainon_Troncon *creer_chainon_troncon(double fuite_pct, Arbre_Noeud *cible);
int ajouter_troncon(Arbre_Noeud *source, Arbre_Graphe_AVL *racine_avl, char *cible_id, double fuite_pct);

// Fonctions de manipulation de l'AVL du Graphe
int avl_graphe_hauteur(Arbre_Graphe_AVL *n);
Arbre_Graphe_AVL *avl_graphe_nouveau_noeud(char *id, Arbre_Noeud *noeud);
Noeud_AVL_Recherche *rotation_droite_graphe(Noeud_AVL_Recherche *y);
Noeud_AVL_Recherche *rotation_gauche_graphe(Noeud_AVL_Recherche *x);
Noeud_AVL_Recherche *double_rotation_gauche_graphe(Noeud_AVL_Recherche *a);
Noeud_AVL_Recherche *double_rotation_droite_graphe(Noeud_AVL_Recherche *a);
int avl_facteur_equilibre_graphe(Arbre_Graphe_AVL *n);
Noeud_AVL_Recherche *equilibrer_graphe(Arbre_Graphe_AVL *n); 
Noeud_AVL_Recherche *avl_inserer_graphe(Noeud_AVL_Recherche *noeud, const char *id_acteur_key, Noeud_Acteur *adresse_noeud, int *h);
Arbre_Noeud *avl_rechercher_graphe(Arbre_Graphe_AVL *racine, char *id);
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
int hauteur_noeud_usine(AVL_Usine *noeud);
int avl_facteur_equilibre_usine(AVL_Usine *noeud);
AVL_Usine *rotation_gauche_usine(AVL_Usine *x); 
AVL_Usine *rotation_droite_usine(AVL_Usine *y); 
AVL_Usine *double_rotation_gauche_usine(AVL_Usine *a);
AVL_Usine *double_rotation_droite_usine(AVL_Usine *a);
AVL_Usine *equilibrer_usine(AVL_Usine *noeud);  
AVL_Usine *avl_inserer_usine(AVL_Usine *racine, Usine u, int *h);
AVL_Usine *avl_rechercher_usine(AVL_Usine *racine, const char *id);
void avl_supprimer_usine(AVL_Usine *racine); // Libération mémoire

// Fonctions de lecture et sortie
AVL_Usine *lire_donnees_et_construire_avl();
int generer_histogramme(AVL_Usine *racine, const char *nom_fichier_sortie);




double calculer_fuites(Arbre_Graphe_AVL *racine_graphe_avl, const char *id_usine);
int historique_fuites(const char *nom_fichier, const char *id_usine, double fuites_pct);
void lire_donnees_et_construire_graphe(Arbre_Graphe_AVL **racine_graphe_avl, AVL_Usine *racine_usine_avl);




#endif

