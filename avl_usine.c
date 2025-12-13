#include "AVL_USINE.H"



void lecture_ecriture_csv1()
{
    const char* fichier = "c-wildwater_v3.dat";
    FILE *fic = fopen(fichier, "r");

    if (fic == NULL){
        perror("fichier introuvable ou erreur d'ouverture");
        exit(1);
    }

    FILE *fic2 = fopen("vol_max.dat", "w");
    if (fic2 == NULL){
        perror("fichier de sortie introuvable");
        exit(1);
    }

    fprintf(fic2, "identifiant_usine;volume_max (k.m3.year-1)\n");

    char line[800];
    
    char c1[50];      
    char usine[50];        
    char c3[50];        
    char volume_max[50];  
    char c5[50];        

    while (fgets(line, sizeof(line), fic))
    {
        sscanf(line, "%49[^;];%49[^;];%49[^;];%49[^;];%49[^\n]",
               c1, usine, c3, volume_max, c5);

        
        if (strcmp(c1, "-") == 0 && strcmp(c3, "-") == 0 && strcmp(c5, "-") == 0)
        {
            fprintf(fic2, "%s;%s\n", usine, volume_max);
        }
    }

    fclose(fic);
    fclose(fic2);
}
void lecture_ecriture_csv2(){
        
    FILE *fic = fopen("c-wildwater_v3.dat", "r");
    FILE *fic2 = fopen("vol_captation.txt", "w");

    if (fic == NULL || fic2 == NULL){
        perror("Erreur ouverture fichier");
        exit(1);
        }

    
    fprintf(fic2, "identifiant_usine;volume_source (k.m3.year-1)\n");

    char line[800];
    char c1[100], source[100], usine[100], volume[100], fuite[100];

    while (fgets(line, sizeof(line), fic)){
        sscanf(line, "%99[^;];%99[^;];%99[^;];%99[^;];%99[^\n]",
               c1, source, usine, volume, fuite);

        if (strcmp(c1, "-") == 0 && strcmp(volume, "-") != 0 && strcmp(fuite, "-") != 0){
            fprintf(fic2, "%s;%s\n", usine, volume);
        }
    }

    fclose(fic);
    fclose(fic2);
}

void lecture_ecriture_csv3()
{
    const char* fichier = "c-wildwater_v3.dat";
    FILE *fic = fopen(fichier, "r");

    if (fic == NULL){
        perror("fichier introuvable ou erreur d'ouverture");
        exit(1);
    }

    FILE *fic2 = fopen("vol_traitement.tmp", "w");
    if (fic2 == NULL){
        perror("fichier de sortie introuvable");
        exit(1);
    }

    fprintf(fic2, "identifiant_usine;volume_reel (k.m3.year-1)\n");

    char line[800];
    char c1[50], source[50], usine[50], volume[50], fuite[50];

    double v, f, v_reel;

    while (fgets(line, sizeof(line), fic))
    {
                
        sscanf(line, "%49[^;];%49[^;];%49[^;];%49[^;];%49[^\n]",
               c1, source, usine, volume, fuite);

      
        if (strcmp(c1, "-") == 0 && strcmp(volume, "-") != 0 && strcmp(fuite, "-") != 0)
        {
            v = atof(volume);  
            f = atof(fuite);    

            v_reel = v * (1 - (f / 100.0));

            fprintf(fic2, "%s;%.2f\n", usine, v_reel);
        }
    }

    fclose(fic);
    fclose(fic2);
}


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
