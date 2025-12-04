Usine creer_usine(const char *id, int capacite){
Usine *u=malloc(sizeof(Usine));
if(u==NULL){
      printf("erreur d'allocation mémoire");
      exit(1);
}

u->id=id;
u->capacite_max=capacite;







AVL_Usine *avl_inserer(AVL_Usine *racine, Usine u){









AVL_Usine *avl_rechercher(AVL_Usine *racine, const char *id){






void avl_supprimer(AVL_Usine *racine){
  
  
