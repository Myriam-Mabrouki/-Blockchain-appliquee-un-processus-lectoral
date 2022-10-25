#include "partie5.h"

/* Exercice 9 - Simulation du processus de vote */

//Q9.1
void submit_vote(Protected * p) {

  //Ouverture du fichier
  FILE * fic = fopen("Pending_votes.txt", "a");
  if (fic == NULL){
    printf("Erreur à l'ouverture du fichier\n");
		return;
  }

  //Ajout du vote dans le fichier
  char * pr = protected_to_str(p);
  fprintf(fic, "%s\n", pr);
  free(pr);

  //Fermeture du fichier
  fclose(fic);
}

//Q9.2
void create_block(CellTree * tree, Key * author, int d) {

  //Création du bloc b
  Block * b = (Block *) malloc(sizeof(Block));
  if (b == NULL){
    printf("Erreur malloc block\n");
    exit(EXIT_FAILURE);
  }

  b->author = keydup(author);
  b->votes = read_public_protected("Pending_votes.txt");

  CellTree * last = last_node(tree);
  char * block;

  //Affectation de previous_hash en fonction de tree
  if (last == NULL) {
    b->previous_hash = (unsigned char *) strdup("beginning");
  }
  else{
    block = block_to_str(last->block);
    b->previous_hash = (unsigned char *) str_to_hash(block);
    free(block);
  }

  compute_proof_of_work(b, d);

  //Suppression du fichier "Pending_votes.txt"
  remove("Pending_votes.txt");

  //Écriture du block
  write_block(b, "Pending_block.txt");

  //Création d'un noeud à partir de b
  CellTree * node = create_node(b);
  if (last == NULL) {
    tree = node;
  } else {
    //Ajout du noeud créé à tree
    add_child(last, node);
  }

}

//Q9.3
void add_block (int d, char * name){

  //Création du block
  Block * b = read_block("Pending_block.txt");
  if (b == NULL){
    printf("Erreur malloc block\n");
    exit(EXIT_FAILURE);
  }

  //Vérification de la validité du block
  if (verify_block(b, d) == 0) {
    printf("Block non valide\n");
    remove("Pending_block.txt");
    delete_block(b);
    exit(EXIT_FAILURE);
  }

  //Création du nom du fichier dans le répertoire Blockchain
  char nom_fic[256] = "./Blockchain/";
  strcat(nom_fic, name);

  //Écriture dans le fichier
  write_block(b, nom_fic);

  //Suppression de Pending_block.txt
  remove("Pending_block.txt");

  delete_block(b);
}

//Q9.4
CellTree * read_tree(){

  int size = - 2; //On initialise size à -2 pour ne pas compter le début et la fin du répertoire ("." et "..");

  //Détermination de la taille du répertoire
  DIR * dir = opendir("./Blockchain/");
  if (dir != NULL) {
    struct dirent * entry;
    while ((entry = readdir(dir)) != NULL) {
      size++;
    }
    closedir(dir);
  }

  //Création du tableau T contenant les noeuds
  CellTree ** T = (CellTree **) malloc(sizeof(CellTree *) * size);
  if (T == NULL){
    printf("Erreur malloc CellTree\n");
    exit(EXIT_FAILURE);
  }

  //Initialisation des cases de T à NULL
  int i;
  for (i = 0; i < size; i++) {
    T[i] = NULL;
  }

  Block * block;

  //Stockage des noeuds dans T
  const char nom_rep[14] = "./Blockchain/";
  char nom_fic[256];

  DIR * dir2 = opendir("./Blockchain/");
  if (dir2 != NULL) {
    struct dirent * entry2;
    i = 0;
    while ((entry2 = readdir(dir2)) != NULL) {

      if (strcmp(entry2->d_name, "." ) != 0 && strcmp(entry2->d_name, ".." ) != 0) {

        //Création d'une chaîne contenant le nom du fichier dans le répertoire
        strcpy(nom_fic, nom_rep);
        strcat(nom_fic, entry2->d_name);


        //Lecture du bloc et création du noeud dans T
        block = read_block(nom_fic);
        T[i] = create_node(block);
        i++;
      }

    }
    closedir(dir2);
  }

  //Recherche des fils
  i = 0;
  int j = 0;

  while (i < size) {
    j = 0;

    while (j < size) {
      if (strcmp( (char *) T[i]->block->hash, (char *) T[j]->block->previous_hash) == 0) {
        add_child(T[i], T[j]);
      }
      j++;
    }
    i++;
  }


  //Recherche du père
  i = 0;
  while (i < size && T[i]->father != NULL) {
    i++;
  }

  CellTree * root;

  //Si on a trouvé le père
  if (i < size) {
    root = T[i];
  }
  //Sinon, on ne renvoie rien
  else{
    root = NULL;
  }

  //Libération mémoire
  free(T);

  return root;
}

//Q9.5
Key * compute_winner_BT(CellTree * tree, CellKey * candidates, CellKey * voters, int sizeC, int sizeV){
  CellProtected * decl = protected_in_highest_child(tree);
  Key * winner = compute_winner(decl, candidates, voters, sizeC, sizeV);
  delete_list_protected(&decl);
  return winner;
}
