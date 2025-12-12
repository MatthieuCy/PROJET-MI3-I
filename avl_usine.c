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


Usine creer_usine(const char *id, int capacite){
Usine u;
u.id=id;
u.capacite_max=capacite;
u.volume_capte = 0;
u.volume_reel = 0;

return u;
}





AVL_Usine *avl_inserer(AVL_Usine *racine, Usine u) {
    if (racine == NULL) {
        /
        return creer_usine(u);
    }

    int comparaison = strcmp(u.id, racine->donnees.id);

    if (comparaison < 0) {
        racine->gauche = avl_inserer(racine->gauche, u);
    } else if (comparaison > 0) {
        racine->droite = avl_inserer(racine->droite, u);
    } else {
        // ID déjà existant : mise à jour des données (ou ignorer)
        // Libérer l'ID alloué dans u, car on utilise celui du nœud existant.
        free(u.id); 
        // Mise à jour des autres données si nécessaire (ici on ignore)
        return racine; 
    }

    // 2. Équilibrage de l'arbre
    return equilibrer(racine);
}

AVL_Usine *avl_rechercher(AVL_Usine *racine, const char *id) {
    if (racine == NULL) {
        return NULL; // Usine non trouvée
    }

    int comparaison = strcmp(id, racine->donnees.id);

    if (comparaison == 0) {
        return racine; // Usine trouvée
    } else if (comparaison < 0) {
        return avl_rechercher(racine->gauche, id); //    Rechercher dans le sous-arbre gauche
    } else { // comparaison > 0
        return avl_rechercher(racine->droite, id); //   Rechercher dans le sous-arbre droit
    }
}





}
void avl_supprimer(AVL_Usine *racine) {
    if (racine == NULL) {
        return;
    }

    // Parcours en postfixe : Gauche, Droite, Racine
    avl_supprimer(racine->gauche);
    avl_supprimer(racine->droite);

    // 1. Libérer l'ID de l'usine
    free(racine->donnees.id);
    
    // 2. Libérer le nœud AVL
    free(racine);
}

AVL_Usine *rotation_droite(AVL_Usine *y){
    AVL_Usine * pivot;
    int eq_y, eq_p;

    pivot = y->fg;
    y->fg = pivot->fd;
    pivot->fd = y;

    eq_y = y->equilibre;
    eq_p = pivot->equilibre;

    y->equilibre = eq_y - min(eq_p, 0) +1;
    pivot->equilibre = max(eq_y+2, eq_y+eq_p+2, eq_p+1);
    y = pivot; 

    return y;
}
  
