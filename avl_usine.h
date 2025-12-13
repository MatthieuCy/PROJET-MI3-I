#ifndef AVL_USINE_H
#define AVL_USINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Fonctions principales 
void lecture_ecriture_csv1();

void lecture_ecriture_csv2();

void lecture_ecriture_csv3();

int hauteur(AVL_Usine *n);
int facteur_equilibre(AVL_Usine *n);
AVL_Usine *rotation_droite(AVL_Usine *y);
AVL_Usine *rotation_gauche(AVL_Usine *x);
AVL_Usine *equilibrer(AVL_Usine *racine); 



Usine creer_usine(const char *id, int capacite);


AVL_Usine *avl_inserer(AVL_Usine *racine, Usine u);zqd


AVL_Usine *avl_rechercher(AVL_Usine *racine, const char *id);

void avl_supprimer(AVL_Usine *racine);

#endif

