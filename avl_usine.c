#define MAX_CHAMP_SIZE 100
#define MAX_LINE_SIZE 1000
#define FICHIER_DONNEES "c-wildwater_v3.dat"
#define CONVERSION_KM3_TO_MM3 1000.0

int maxi(int a, int b){
    if(a>=b)    return a;
    return b;
}

int mini(int a, int b){
    if(a<=b)    return a;
    return b;
}


Usine creer_usine(const char *id_source, double capacite) {
    Usine u;

    u.id = strdup(id_source);
    if (u.id == NULL) {
       printf(" Erreur d'allocation memoire  \n ");
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
        printf("Echec d'allocation memoire \n");
        free(usine_donnees.id);
        exit(1); 
    }

    nouveau_noeud->donnees = usine_donnees; 

    nouveau_noeud->gauche = NULL;
    nouveau_noeud->droite = NULL;
    nouveau_noeud->hauteur = 1; 
    
    return nouveau_noeud;
}

int get_facteur_equilibre(AVL_Usine *noeud) {
    if (noeud == NULL) {
        return 0;
    }
    return hauteur_noeud(noeud->gauche) - hauteur_noeud(noeud->droite);
}

AVL_Usine *avl_rechercher(AVL_Usine *racine, const char *id) {
    if (racine == NULL) {
        return NULL; 
    }

    int comparaison = strcmp(id, racine->donnees.id);

    if (comparaison == 0) {
        return racine; // Usine trouvée
    } else if (comparaison < 0) {
        return avl_rechercher(racine->gauche, id); 
    } else { 
        return avl_rechercher(racine->droite, id); 
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

int hauteur_noeud(AVL_Usine *noeud) {
    if (noeud == NULL) {
        return 0;
    }
    return noeud->hauteur;
}


AVL_Usine *rotation_gauche(AVL_Usine *x) {
    if (x == NULL || x->droite == NULL) {
        return x; 
    }

    AVL_Usine *y = x->droite; 
    AVL_Usine *T2 = y->gauche;

    y->gauche = x; 
    x->droite = T2;

    x->hauteur = maxi(hauteur_noeud(x->gauche), hauteur_noeud(x->droite)) + 1;
    y->hauteur = maxi(hauteur_noeud(y->gauche), hauteur_noeud(y->droite)) + 1;
    return y;
}


AVL_Usine *rotation_droite(AVL_Usine *y) {
    if (y == NULL || y->gauche == NULL) {
        return y;
    }
    
    AVL_Usine *x = y->gauche;
    AVL_Usine *T2 = x->droite;

    y->gauche = T2;
    x->droite = y;

    y->hauteur = maxi(hauteur_noeud(y->gauche), hauteur_noeud(y->droite)) + 1;
    x->hauteur = maxi(hauteur_noeud(x->gauche), hauteur_noeud(x->droite)) + 1;
    
    return x;
}

AVL_Usine *equilibrer(AVL_Usine *noeud) {
    if (noeud == NULL) {
        return NULL;
    }

    noeud->hauteur = maxi(hauteur_noeud(noeud->gauche), hauteur_noeud(noeud->droite)) + 1;

    int equilibre = get_facteur_equilibre(noeud);
    
    if (equilibre > 1 && get_facteur_equilibre(noeud->gauche) >= 0) {
        return rotation_droite(noeud);
    }

    if (equilibre > 1 && get_facteur_equilibre(noeud->gauche) < 0) {
        noeud->gauche = rotation_gauche(noeud->gauche);
        return rotation_droite(noeud);
    }

    if (equilibre < -1 && get_facteur_equilibre(noeud->droite) <= 0) {
        return rotation_gauche(noeud);
    }

    if (equilibre < -1 && get_facteur_equilibre(noeud->droite) > 0) {
        noeud->droite = rotation_droite(noeud->droite);
        return rotation_gauche(noeud);
    }

    return noeud;
}
  


AVL_Usine *avl_inserer(AVL_Usine *racine, Usine u) {
    if (racine == NULL) {
        return creer_noeud_usine(u);
    }

    int comparaison = strcmp(u.id, racine->donnees.id);

    if (comparaison < 0) {
        racine->gauche = avl_inserer(racine->gauche, u);
    } else if (comparaison > 0) {
        racine->droite = avl_inserer(racine->droite, u);
    } else {
        if (u.capacite_max > racine->donnees.capacite_max) {
             racine->donnees.capacite_max = u.capacite_max;
             free(u.id);
        }        
        return racine; 
    }
    return equilibrer(racine);
}

//Lit le fichier de données et construit un AVL contenant les usines.

AVL_Usine *lire_donnees_et_construire_avl() {

    // Ouverture du fichier de données en lecture
    FILE *fic = fopen(FICHIER_DONNEES, "r");
    if (fic == NULL) {
        printf("ERREUR: Le fichier %s est introuvable \n", FICHIER_DONNEES);
        return NULL;
    }

    AVL_Usine *racine = NULL;
    char line[MAX_LINE_SIZE];

    //stocker les 5 champs séparés par ';'
    char c1[MAX_CHAMP_SIZE], c2[MAX_CHAMP_SIZE],
         c3[MAX_CHAMP_SIZE], c4[MAX_CHAMP_SIZE],
         c5[MAX_CHAMP_SIZE];

    // Saut de la ligne d'en-tête (noms des colonnes) pour eviter decalage et erreur
    if (fgets(line, sizeof(line), fic) == NULL) {
        fclose(fic);
        return NULL;
    }

    // Lecture du fichier ligne par ligne
    while (fgets(line, sizeof(line), fic)) {

        // Découpage de la ligne en 5 champs séparés par ';'
        if (sscanf(line,
                   "%99[^;];%99[^;];%99[^;];%99[^;];%99[^\n]",
                   c1, c2, c3, c4, c5) != 5) {
            // Ligne mal formée : on l'ignore 
            continue;
        }

        //CAS 1 : Définition d'une usine  et sa capa max
        
        if (strcmp(c1, "-") == 0 &&
            strcmp(c3, "-") == 0 &&
            strcmp(c5, "-") == 0) {

            //  convertion de la capacite max  en nombre réel (double)
            double capacite = atof(c4);

            // Création de la structure Usine (ID + capacité max) puis  Insertion ou mise à jour de l'usine dans l'AVL
            Usine u_payload = creer_usine(c2, capacite);
            racine = avl_inserer(racine, u_payload);
        }

        // CAS 2 : SOURCE → USINE
        
        else if (strcmp(c1, "-") == 0 &&
                 strcmp(c4, "-") != 0 &&
                 strcmp(c5, "-") != 0) {

            // Recherche de l'usine concernée dans l'AVL
            AVL_Usine *noeud_usine = avl_rechercher(racine, c3);

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

// détermine comment  écrire la ligne (conversion et formatage correct).
void ecrire_ligne_usine(FILE *fic, AVL_Usine *noeud) {
    if (noeud == NULL) return;

    // le sujet nous demande la Conversion en Millions de m³ (M.m³)
    double capacite_mm3 = noeud->donnees.capacite_max / CONVERSION_KM3_TO_MM3;
    double capte_mm3 = noeud->donnees.volume_capte / CONVERSION_KM3_TO_MM3;
    double reel_mm3 = noeud->donnees.volume_reel / CONVERSION_KM3_TO_MM3;

    // Écriture au format CSV avec deux décimales
    fprintf(fic, "%s;%.2f;%.2f;%.2f\n", noeud->donnees.id, capacite_mm3,capte_mm3,  reel_mm3);
}


//parcours infixe inverse pour avoir un tri alphabetique inverse
void parcourir_et_ecrire_inverse(AVL_Usine *noeud, FILE *fic) {
    if (noeud == NULL) {
        return;
    }
    parcourir_et_ecrire_inverse(noeud->droite, fic);
    ecrire_ligne_usine(fic, noeud);
    parcourir_et_ecrire_inverse(noeud->gauche, fic);
}


// fonction  qui génère un fichier CSV à partir d’un AVL d’usines pourr tracer un histogramme des capacités et volumes traités.
int generer_histogramme(AVL_Usine *racine, const char *nom_fichier_sortie) {
    if (racine == NULL) {
        printf("AVL vide, aucun histogramme genere.\n");
        return 0;
    }

    // Ouverture du fichier de sortie en mode écriture
    FILE *fic_out = fopen(nom_fichier_sortie, "w");
    if (fic_out == NULL) {
        printf("ERREUR: Impossible d'ouvrir le fichier de sortie %s.\n", nom_fichier_sortie);
        return 2;
    }

    // Écriture de l'en-tête : le contenu de chaque colonne
    fprintf(fic_out, "identifiant_usine;capacite_max (M.m3.year-1);volume_capte (M.m3.year-1);volume_reel (M.m3.year-1)\n");

    // Lancement du parcours pour remplir le fichier
    parcourir_et_ecrire_inverse(racine, fic_out);

    fclose(fic_out);
    return 0;
}


