#include "avl.h"



static int maxi(int a, int b) {
    if (a >= b)
        return a;
    return b;
}

static int mini(int a, int b) {
    if (a <= b)
        return a;
    return b;
}

// Fonction pour trouver le maximum entre trois entiers
static int max_trois(int a, int b, int c) {
    int m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    return m;
}

// Fonction pour trouver le minimum entre trois entiers
static int min_trois(int a, int b, int c) {
    int m = a;
    if (b < m) m = b;
    if (c < m) m = c;
    return m;
}

Usine creer_usine(const char *id_source, double capacite) {
    Usine u;

    u.id = strdup(id_source);
    if (u.id == NULL) {
       printf(" Erreur d'allocation memoire pour l'ID de l'usine \n ");
        exit(1); 
    }
    
    u.capacite_max = capacite;
    u.volume_capte = 0.0;
    u.volume_reel = 0.0;

    return u;
}


AVL_Usine *creer_noeud_usine(Usine usine_donnees) {
    AVL_Usine *nouveau_noeud = (AVL_Usine *)malloc(sizeof(AVL_Usine));
    
    if (nouveau_noeud == NULL) { 
        printf("Echec d'allocation memoire pour le noeud AVL \n");
        free(usine_donnees.id);
        exit(1); 
    }

    nouveau_noeud->donnees = usine_donnees; 

    nouveau_noeud->gauche = NULL;
    nouveau_noeud->droite = NULL;
    nouveau_noeud->equilibre = 0; 
    
    return nouveau_noeud;
}


int hauteur_noeud(AVL_Usine *noeud) {
    if (noeud == NULL) {
        return 0;
    }

    int hg = hauteur_noeud(noeud->gauche);
    int hd = hauteur_noeud(noeud->droite);

    if (hg > hd)
    return 1 + hg;
    else
    return 1 + hd;
}


int get_facteur_equilibre(AVL_Usine *noeud) {
    if (noeud == NULL) {
        return 0;
    }
    return hauteur_noeud(noeud->gauche) - hauteur_noeud(noeud->droite);
}


AVL_Usine *rotation_gauche_usine(AVL_Usine *a) {
    if (a == NULL || a->droite == NULL) {
        return a;
    } 
    
    AVL_Usine *pivot;
    int eq_a, eq_p;

    pivot = a->droite;
    a->droite = pivot->gauche;
    pivot->gauche = a;

    eq_a = a->equilibre;
    eq_p = pivot->equilibre;

    a->equilibre = eq_a - maxi(eq_p, 0) - 1;
    pivot->equilibre = min_trois(eq_a - 2, eq_a + eq_p - 2, eq_p - 1);

    a = pivot;
    return a;
}


AVL_Usine *rotation_droite_usine(AVL_Usine *a) {
    if (a == NULL || a->gauche == NULL) {
        return a;
    }

    AVL_Usine *pivot;
    int eq_a, eq_p;

    pivot = a->gauche;
    a->gauche = pivot->droite;
    pivot->droite = a;

    eq_a = a->equilibre;
    eq_p = pivot->equilibre;

    a->equilibre = eq_a - mini(eq_p, 0) + 1;
    pivot->equilibre = max_trois(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);

    a = pivot;
    return a;
}


AVL_Usine *double_rotation_gauche_usine(AVL_Usine *a) {
    if (a == NULL || a->droite == NULL) {
        return a;
    }
    
    a->droite = rotation_droite_usine(a->droite);
    return rotation_gauche_usine(a);
}


AVL_Usine *double_rotation_droite_usine(AVL_Usine *a) {
    if (a == NULL || a->gauche == NULL) {
        return a;
    }
    
    a->gauche = rotation_gauche_usine(a->gauche);
    return rotation_droite_usine(a);
}


AVL_Usine *equilibrer_usine(AVL_Usine *noeud) {
    if (noeud == NULL) {
        return NULL;
    }
    
    if (noeud->equilibre >= 2 && noeud->droite) {
        if (noeud->droite->equilibre >=0) {
             return rotation_gauche_usine(noeud);
        } 
        else {
            return double_rotation_gauche_usine(noeud);
        }
    } 
    else if (noeud->equilibre <= -2) {
        if(noeud->gauche->equilibre <=0) {
             return rotation_droite_usine(noeud);
        }
        else {
             return double_rotation_droite_usine(noeud);
        }     
    }
    return noeud;
}  

  

AVL_Usine *avl_inserer_usine(AVL_Usine *a, Usine u, int *h) {
    if (a == NULL) {
        *h = 1;
        return creer_noeud_usine(u);
    }

    int comparaison = strcmp(u.id, a->donnees.id);

    if (comparaison < 0) {
        a->gauche = avl_inserer_usine(a->gauche, u, h);
        *h = -(*h);
    } else if (comparaison > 0) {
        a->droite = avl_inserer_usine(a->droite, u, h);
    } else {
        if (u.capacite_max > a->donnees.capacite_max) {
             a->donnees.capacite_max = u.capacite_max;
        }       
        free(u.id);
        *h = 0;
        return a; 
    }
    if (*h !=0) {
        a->equilibre = a->equilibre + *h;
        a = equilibrer_usine(a);
        if (a->equilibre == 0) {
            *h = 0;
        }
        else {
             *h = 1;
        }
    }
    return a;
}


Voici la fonction mise à jour proprement. J'ai supprimé le bloc qui faisait doublon à la fin pour ne garder que la logique de comparaison basée sur le mode (max, src ou real).

C

// Insertion pour histogramme 
AVL_Usine* avl_inserer_par_valeur(AVL_Usine* a, Usine u, int* h, const char* mode) {
    if (a == NULL) {
        *h = 1;
        return creer_noeud_usine(u);
    }

    double val_nouvelle, val_actuelle;

    // choix valeur à comparer selon l'argument du script Shell
    if (strcmp(mode, "max") == 0) {
        val_nouvelle = u.capacite_max;
        val_actuelle = a->donnees.capacite_max;
    } else if (strcmp(mode, "src") == 0) {
        val_nouvelle = u.volume_capte;
        val_actuelle = a->donnees.volume_capte;
    } else { 
        val_nouvelle = u.volume_reel;
        val_actuelle = a->donnees.volume_reel;
    }


    if (val_nouvelle < val_actuelle) {
        a->gauche = avl_inserer_par_valeur(a->gauche, u, h, mode);
        *h = -(*h);
    } else {
        a->droite = avl_inserer_par_valeur(a->droite, u, h, mode);
        *h = *h;
    }

    // 3. Équilibrage de l'arbre
    if (*h != 0) {
        a->equilibre += *h;
        a = equilibrer_usine(a);
        if (a->equilibre == 0) {
            *h = 0;
        } else {
            *h = 1;
        }
    }
    return a;
}

AVL_Usine *avl_rechercher_usine(AVL_Usine *racine, const char *id) {
    if (racine == NULL) {
        return NULL; 
    }

    int comparaison = strcmp(id, racine->donnees.id);

    if (comparaison == 0) {
        return racine; // Usine trouvée
    } else if (comparaison < 0) {
        return avl_rechercher_usine(racine->gauche, id); 
    } else { 
        return avl_rechercher_usine(racine->droite, id); 
    }
}


void avl_supprimer(AVL_Usine *racine) {
    if (racine == NULL) {
        return;
    }
    avl_supprimer(racine->gauche);
    avl_supprimer(racine->droite);
    free(racine->donnees.id);  
    free(racine);
}


//Lit le fichier de données et construit un AVL contenant les usines.

AVL_Usine *lire_donnees_et_construire_avl(const char *nom_fichier) {

    // Ouverture du fichier de données en lecture
    FILE *fic = fopen(nom_fichier, "r");
    if (fic == NULL) {
        printf("ERREUR: Le fichier %s est introuvable \n", FICHIER_DONNEES);
        return NULL;
    }

    AVL_Usine *racine = NULL;
    char line[MAX_LINE_SIZE];

    //stocker les 5 champs séparés par ';'
    char c1[MAX_CHAMP_SIZE], c2[MAX_CHAMP_SIZE], c3[MAX_CHAMP_SIZE], c4[MAX_CHAMP_SIZE], c5[MAX_CHAMP_SIZE];

    // Saut de la ligne d'en-tête (noms des colonnes) pour eviter decalage et erreur
    if (fgets(line, sizeof(line), fic) == NULL) {
        fclose(fic);
        return NULL;
    }

    // Lecture du fichier ligne par ligne
    while (fgets(line, sizeof(line), fic)) {

        // Découpage de la ligne en 5 champs séparés par ';'
        if (sscanf(line,
                   "%99[^;];%99[^;];%99[^;];%99[^;];%99[^\n]", c1, c2, c3, c4, c5) != 5) {
            // Ligne mal formée : on l'ignore 
            continue;
        }

        //Cas 1 : Définition d'une usine  et sa capa max
        
        if (strcmp(c1, "-") == 0 &&  strcmp(c3, "-") == 0 &&  strcmp(c5, "-") == 0) {

            //  convertion de la capacite max  en nombre réel (double)
            double capacite = atof(c4);

            // Création de la structure Usine (ID + capacité max) puis  Insertion ou mise à jour de l'usine dans l'AVL
            Usine u = creer_usine(c2, capacite);
            int h = 0;
            racine = avl_inserer_usine(racine, u, &h);
        }

        // Cas 2 : source ->usine
        
        else if (strcmp(c1, "-") == 0 &&  strcmp(c4, "-") != 0 && strcmp(c5, "-") != 0) {

            // Recherche de l'usine concernée dans l'AVL
            AVL_Usine *noeud_usine = avl_rechercher_usine(racine, c3);

            // Si l'usine existe, on met à jour ses volumes
            if (noeud_usine != NULL) {

                // Conversion des valeurs numériques
                double volume = atof(c4);
                double fuite_pct = atof(c5);

                // Calcul du volume réellement reçu après pertes
                double volume_reel_traite =  volume * (1.0 - (fuite_pct / 100.0));

                // Accumulation des volumes dans l'usine
                noeud_usine->donnees.volume_capte += volume;
                noeud_usine->donnees.volume_reel += volume_reel_traite;
            }
        }
    }
    fclose(fic);
    

    return racine;
}

// Détermine comment  écrire la ligne (conversion et formatage correct).
void ecrire_ligne_usine(FILE *fic, AVL_Usine *noeud) {
    if (noeud == NULL) return;

    // le sujet nous demande la conversion en millions de m³
    double capacite_mm3 = noeud->donnees.capacite_max / CONVERSION_KM3_TO_MM3;
    double capte_mm3 = noeud->donnees.volume_capte / CONVERSION_KM3_TO_MM3;
    double reel_mm3 = noeud->donnees.volume_reel / CONVERSION_KM3_TO_MM3;

    // Écriture au format CSV avec deux décimales
    fprintf(fic, "%s;%.2f;%.2f;%.2f\n", noeud->donnees.id, capacite_mm3,capte_mm3,  reel_mm3);
}


// Parcours infixe 
void parcourir_et_ecrire_simple(AVL_Usine *noeud, FILE *fic) {
    if (noeud == NULL) return;
    parcourir_et_ecrire_simple(noeud->gauche, fic);
    ecrire_ligne_usine(fic, noeud);
    parcourir_et_ecrire_simple(noeud->droite, fic);
}


// Fonction qui prend l'arbre ID et remplit l'arbre Valeur
void transvaser_vers_tri_valeur(AVL_Usine* origine, AVL_Usine** nouvelle_racine, const char* mode) {
    if (origine == NULL) return;
    int h = 0;
    
    // copie
    Usine copie = origine->donnees;
    copie.id = strdup(origine->donnees.id); 
    
    // transmet le mode
    *nouvelle_racine = avl_inserer_par_valeur(*nouvelle_racine, copie, &h, mode);

    transvaser_vers_tri_valeur(origine->gauche, nouvelle_racine, mode);
    transvaser_vers_tri_valeur(origine->droite, nouvelle_racine, mode);
}


void liberer_avl_usine(AVL_Usine *racine) {
    if (racine == NULL) return;
    liberer_avl_usine(racine->gauche);
    liberer_avl_usine(racine->droite);
    if (racine->donnees.id != NULL) {
        free(racine->donnees.id);
    }
    free(racine);
}



// Fonction  qui génère un fichier CSV à partir d’un AVL d’usines pourr tracer un histogramme des capacités et volumes traités.
int generer_histogramme(AVL_Usine *racine_id, const char *nom_fichier_sortie, const char *mode) {
    if (racine_id == NULL) {
        printf("AVL vide.\n");
        return 0;
    }
    
    AVL_Usine* racine_triee = NULL;
    // transvasement mode choisi (max, src ou real)
    transvaser_vers_tri_valeur(racine_id, &racine_triee, mode);

    FILE *fic_out = fopen(nom_fichier_sortie, "w");
    if (fic_out == NULL) {
        printf("ERREUR: Impossible d'ouvrir le fichier de sortie %s.\n", nom_fichier_sortie);
        liberer_avl_usine(racine_triee);
        return 2;
    }
    
    // En-tête du fichier
    fprintf(fic_out, "identifiant_usine;capacite_max (M.m3.year-1);volume_capte (M.m3.year-1);volume_reel (M.m3.year-1)\n");
    
    // Écriture triée
    parcourir_et_ecrire_simple(racine_triee, fic_out);
    
    fclose(fic_out);
    liberer_avl_usine(racine_triee);
    return 0;
}


void liberer_troncons(Chainon_Troncon *debut) {
    Chainon_Troncon *courant = debut;
    while (courant != NULL) {
        Chainon_Troncon *a_supprimer = courant;
        courant = courant->suivant;
        free(a_supprimer);
    }
}

