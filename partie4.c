#include "partie4.h"

/* Exercice 7 - Structure d'un block et persistance */

//Q7.1
void write_block(Block * block, char * nom) {

	//Ouverture du fichier
  FILE * fic = fopen(nom, "w");
  if (fic == NULL){
    printf("Erreur à l'ouverture du fichier\n");
		return;
  }

	//Ecriture fichier
	char * author = key_to_str(block->author);
	CellProtected * votes = block->votes;

	fprintf(fic, "%s\n", author);
  fprintf(fic, "%s\n", block->hash);
	fprintf(fic, "%s\n", block->previous_hash);
	fprintf(fic, "%d\n", block->nonce);

	char * pr;

	while (votes) {
		pr = protected_to_str(votes->data);
		fprintf(fic, "%s\n", pr);
		free(pr);
    votes = votes->next;
	}

	//Libération mémoire
	free(author);

	//Fermeture fichier
	fclose(fic);
}

//Q7.2
Block * read_block (char * nom){

	//Ouverture du fichier
  FILE * fic = fopen(nom, "r");
  if (fic == NULL){
    printf("Erreur à l'ouverture du fichier\n");
		return NULL;
  }

	//Lecture fichier
	Block * block = (Block *) malloc(sizeof(Block));
  if (block == NULL){
    printf("Erreur malloc block\n");
    exit(EXIT_FAILURE);
  }

	char ligne[MAX_BUFFER_SIZE];
	Key * author = NULL;
	Protected * vote = NULL;
	CellProtected * list_votes = NULL;
  int last;

	if (fgets(ligne, MAX_BUFFER_SIZE, fic) != NULL) {
		author = str_to_key(ligne);
		if (author == NULL) {
			printf("Erreur lecture key\n");
		  exit(EXIT_FAILURE);
		}
		block->author = keydup(author);
		free(author);


	}
  else{
    printf("Format de ligne incorrect\n");
    exit(EXIT_FAILURE);
  }

  if (fgets(ligne, MAX_BUFFER_SIZE, fic) != NULL) {
		block->hash = (unsigned char *) strdup(ligne);
    last = strlen(ligne) - 1;
    block->hash[last] = '\0';
	}
  else{
    printf("Format de ligne incorrect\n");
    exit(EXIT_FAILURE);
  }

	if (fgets(ligne, MAX_BUFFER_SIZE, fic) != NULL) {
		block->previous_hash = (unsigned char *) strdup(ligne);
    last = strlen(ligne) - 1;
    block->previous_hash[last] = '\0';
	}
  else{
    printf("Format de ligne incorrect\n");
    exit(EXIT_FAILURE);
  }

	if (fgets(ligne, MAX_BUFFER_SIZE, fic) != NULL) {
		block->nonce = atoi(ligne);
	}
  else{
    printf("Format de ligne incorrect\n");
    exit(EXIT_FAILURE);
  }

  while (fgets(ligne, MAX_BUFFER_SIZE, fic) != NULL) {
    vote = str_to_protected(ligne);
    if (vote != NULL) {
      if (list_votes == NULL){
        list_votes = create_cell_protected(vote);
      }
      else{
        push_protected(&list_votes, vote);
      }
      free_protected(vote);
    }
  }

	block->votes = list_votes;

	//Fermeture fichier
	fclose(fic);

	return block;
}

//Q7.3
char * block_to_str(Block * block){

  //Création des chaînes author et previous_hash
	char * author = key_to_str(block->author);
	char * previous_hash = strdup((char *) block->previous_hash);


  //Détermination de la taille de la liste de déclarations
  char * vote;
  int tailleVotes = 0;

  CellProtected * cp = block->votes;
  while (cp != NULL) {
    vote = protected_to_str(cp->data);
    tailleVotes += strlen(vote) + 1;
    cp = cp->next;
    free(vote);
  }

  //Création de la chaine de déclarations
  char * votes = (char *) malloc (sizeof(char) * tailleVotes);
  votes[0] = '\0';
  cp = block->votes;
  while (cp != NULL) {
    vote = protected_to_str(cp->data);
    strcat(votes, vote);
    cp = cp->next;
    free(vote);
  }

  //Création de la chaîne retour
  int taille = strlen(author) + strlen(previous_hash) + tailleVotes + 256;
  char * res = (char *) malloc(sizeof(char) * taille);

  //Affectation à la chaîne de retour
  sprintf(res, "%s %s %s %d", author, previous_hash, votes, block->nonce);

	//Libération mémoire
  free(votes);
	free(author);
	free(previous_hash);

  return res;
}

//Q7.5
unsigned char * str_to_hash(char * str){
  char * result = (char *) malloc(sizeof(char) * 126);
  if (result == NULL) {
    printf("Erreur d'allocation (create_hashcell)\n");
    return NULL;
  }

  unsigned char * hashed = SHA256((unsigned char *) str, strlen(str), 0);

  sprintf(result, "%02x", hashed[0]);

  char buffer[8];
  for (int i = 1; i < SHA256_DIGEST_LENGTH; i++){
    sprintf(buffer, "%02x", hashed[i]);
    strcat(result, buffer);
  }

  return (unsigned char *) result;
}

//Q7.6
void compute_proof_of_work(Block * b, int d){

	int i=0;
  b->nonce = 0;
	unsigned char * hash;
  char * block;
  int trouve = 0;

  //On parcourt tant qu'on n'a pas trouvé d 0 au début
  while (trouve == 0){

		i = 0;

    //Valeur hachée du bloc
    block = block_to_str(b);
		hash = str_to_hash(block);

    //On parcourt tant qu'il y a des 0 devant
		while (hash[i] == '0' && i < d ) {
			i++;
		}

    //Si on trouve au moins d 0 au début
    if (i == d) {
      trouve = 1;
      break;
    }

    //Sinon on incrémente nonce et on continue
    b->nonce++;

    free(block);
    free(hash);
  }

  //On affecte à b->hash la valeur trouvée
	b->hash = (unsigned char *) strdup((char *) hash);
  free(block);
  free(hash);
}

//Q7.7
int verify_block (Block * b, int d){

  //Vérification
	for (int i = 0; i < d; i++) {

    //Si une case d'indice < d - 1 est différente de 0, alors le bloc n'est pas valide
		if (b->hash[i] != '0') {
			return 0;
		}
	}

  //Sinon le bloc est valide
  return 1;
}

//Q7.9
void delete_block(Block * b) {
  free(b->author);
  delete_list_protected(&(b->votes));
  free(b->hash);
  free(b->previous_hash);
  free(b);
}

/* Exercice 8 - Structure arborescente */

//Q8.1
CellTree * create_node(Block * b){

  //Création de la structure CellTree
  CellTree * ct = (CellTree *) malloc(sizeof(CellTree));
  if (ct == NULL){
    printf("Erreur malloc CellTree\n");
    exit(EXIT_FAILURE);
  }

  //Initialisation des membres de la structure
  ct->block = b;
  ct->father = NULL;
  ct->firstChild = NULL;
  ct->nextBro = NULL;
  ct->height = 0;
  return ct;
}

//Q8.2
int update_height(CellTree * father, CellTree * child){
  if (father->height >= (child->height + 1)) {
    return 0;
  }
  father->height = child->height + 1;
  return 1;
}

//Q8.3
void add_child(CellTree * father, CellTree * child){

  //Ajout d'un père au fils
  if (child != NULL) {
    child->father=father;
  }

  CellTree * tmp;

  if (father != NULL) {

    //Si father n'a pas de fils
    if (father->firstChild == NULL) {
      father->firstChild = child;
    }

    //Sinon on parcourt ses fils
    else{
      tmp = father->firstChild;
      while (tmp->nextBro != NULL) {
        tmp = tmp->nextBro;
      }
      tmp->nextBro=child;
    }

    //Mise à jour de la taille
    if (child != NULL) {
      tmp = child;
      while (tmp->father != NULL) {
        update_height(tmp->father, tmp);
        tmp = tmp->father;
      }
    }
  }

  else{
    father = child;
  }

}

//Affichage d'un noeud (ajoutée)
void print_node(CellTree * node){

  if (node != NULL) {
    char * block = block_to_str(node->block);
    unsigned char * hash = str_to_hash(block);
    printf("height : %d hash : %s ", node->height, hash);
    free(block);
    free(hash);
  }

}

//Q8.4
void print_tree(CellTree * tree){

  if (tree != NULL) {

    //Affichage du noeud
    print_node(tree);

    //Affichage des frères sur une même ligne
    print_tree(tree->nextBro);

    //Affichage des fils à la ligne suivante
    if (tree->firstChild != NULL) {
      printf("\n");
    }
    print_tree(tree->firstChild);
  }
}

//Q8.5
void delete_node(CellTree * node){

  //Si node a un père et que node est son fils, alors le 1e fils du père devient le frère de node
  if (node->father != NULL && node->father->firstChild == node) {
    node->father->firstChild = node->nextBro;
  }

  //Suppression
  delete_block(node->block);
  free(node);
}

void delete_tree(CellTree * tree){
  //On suppose qu'on a une racine en argument
  if (tree != NULL) {

    //Suppression des frères et des fils s'ils existent
    if (tree->nextBro != NULL && tree->firstChild != NULL) {
      delete_tree(tree->nextBro);
      delete_tree(tree->firstChild);
    }
    else if (tree->nextBro == NULL) {
      delete_tree(tree->firstChild);
    }
    else if (tree->firstChild == NULL) {
      delete_tree(tree->nextBro);
    }

    //Suppression du noeud
    delete_node(tree);
  }
}

//Q8.6
CellTree * highest_child(CellTree * cell){
  if (cell == NULL) {
    return NULL;
  }
  CellTree * res = cell->firstChild;
  CellTree * tmp = res;

  //Recherche du fils de cell avec la plus grande hauteur
  while (tmp != NULL) {
    if (tmp->height >= res->height) {
      res = tmp;
    }
    tmp = tmp->nextBro;
  }

  return res;
}

//Q8.7
CellTree * last_node(CellTree * tree){
  CellTree * res = tree;
  if (res == NULL) {
    return tree;
  }

  //Parcours de la plus longue chaîne jusqu'à arriver au dernier fils
  while (res->firstChild != NULL) {
    res = highest_child(res);
  }

  return res;
}

//Q8.8
void fusion_declarations(CellProtected ** cp1, CellProtected * cp2){

  //Si aucun des arguments n'est NULL, on parcourt la première liste et on pointe le dernier élément vers la tête de la seconde
  if (*cp1 != NULL && cp2 != NULL){
    CellProtected * tmp = *cp1;
    while (tmp->next) {
      tmp = tmp->next;
    }
    tmp->next = cellProtectedDup(cp2);
  }

  //Si le premier argument est NULL, on lui affecte le second
  if (*cp1 == NULL) {
    *cp1 = cellProtectedDup(cp2);
  }
}

//Q8.9
CellProtected * protected_in_highest_child(CellTree * tree){

  CellProtected * res = NULL;
  CellTree * tmp_node = tree;
  CellProtected * tmp_votes;

  //Fusion des déclarations de la plus longue chaîne de l'arbre
  while (tmp_node) {
    tmp_votes = tmp_node->block->votes;
    fusion_declarations(&res, tmp_votes);
    tmp_node = highest_child(tmp_node);
  }

  return res;
}
