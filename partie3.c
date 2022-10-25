#include "partie3.h"

/* Exercice 5 - Lecture et stockage des données dans des listes chainées */

//Q5.1
CellKey * create_cell_key(Key * key){
  CellKey * ck = (CellKey *) malloc(sizeof(CellKey));
  if (ck == NULL){
    printf("Erreur malloc CellKey\n");
    exit(EXIT_FAILURE);
  }
  ck->data = keydup(key);
  ck->next = NULL;

  return ck;
}

//Q5.2
void push_key (CellKey ** ck, Key * key){
  CellKey * new_cell = create_cell_key(key);
  new_cell->next = *ck;
  *ck = new_cell;
}

//Q5.3
CellKey * read_public_keys (char * nom){

  //Ouverture du fichier
  FILE * fic = fopen(nom, "r");
  if (fic == NULL){
    printf("Erreur à l'ouverture du fichier\n");
		return NULL;
  }

  char ligne[MAX_BUFFER_SIZE];
  CellKey * lck = NULL;
  Key * k = NULL;

  //Lecture et création de la liste
  while (fgets(ligne, MAX_BUFFER_SIZE, fic) != NULL) {
    k = str_to_key(ligne);
    if (k != NULL) {
      if (lck == NULL){
        lck = create_cell_key(k);
      }
      else{
          push_key(&lck, k);
      }
    } else {
      printf("La clé n'a pas pu être lue\n");
    }
    free(k);
  }
  fclose(fic);

  return lck;
}

//Affichage d'une clé (ajoutée)
void print_key(Key * k){
  if (k != NULL) {
    printf("(%lx,%lx)", k->val, k->n);
  }
}

//Q5.4
void print_list_keys(CellKey * LCK){
  while (LCK != NULL){
    print_key(LCK->data);
    printf("\n");
    LCK = LCK->next;
  }
}

//Q5.5
void delete_cell_key(CellKey * c){
  if (c != NULL){
    free(c->data);
    free(c);
  }
}

void delete_list_key(CellKey ** LCK){
  if (LCK != NULL) {
    CellKey * tmp;
    while (*LCK){
      tmp = (*LCK)->next;
      delete_cell_key(*LCK);
      *LCK = tmp;
    }
  }
}

//Q5.6
CellProtected * create_cell_protected(Protected * pr) {
  CellProtected * cp = (CellProtected *) malloc(sizeof(CellProtected));
  if (cp == NULL){
    printf("Erreur malloc CellProtected\n");
    exit(EXIT_FAILURE);
  }
  cp->data = init_protected(pr->pKey, pr->mess, pr->sgn);
  cp->next = NULL;

  return cp;
}

//Q5.7
void push_protected (CellProtected ** cp, Protected * pr){
  CellProtected * new_cell = create_cell_protected(pr);
  new_cell->next = *cp;
  *cp = new_cell;
}

//Duplication d'une liste chainée de déclarations (ajoutée)
CellProtected * cellProtectedDup(CellProtected * cp) {
  if (cp != NULL) {
    CellProtected * res = create_cell_protected(cp->data);
    cp = cp->next;
    while (cp != NULL) {
      push_protected(&res, cp->data);
      cp = cp->next;
    }
    return res;
  }

  return NULL;
}

//Q5.8
CellProtected * read_public_protected (char * nom){

  //Ouverture du fichier
  FILE * fic = fopen(nom, "r");
  if (fic == NULL){
    printf("Erreur à l'ouverture du fichier\n");
		return NULL;
  }

  char ligne[MAX_BUFFER_SIZE];
  CellProtected * lcp = NULL;
  Protected * pr = NULL;

  //Lecture et création de la liste
  while (fgets(ligne, MAX_BUFFER_SIZE, fic) != NULL) {
    pr = str_to_protected(ligne);
    if (pr != NULL) {
      if (lcp == NULL){
        lcp = create_cell_protected(pr);
      }
      else{
          push_protected(&lcp, pr);
      }
    } else {
      printf("Le protected n'a pas pu être lu\n");
    }
    free_protected(pr);
  }
  fclose(fic);

  return lcp;
}

//Affichage d'une signature (ajoutée)
void print_signature(Signature * sgn) {
  if (sgn != NULL) {
    char * str = signature_to_str(sgn);
    printf("%s\n", str);
    free(str);
  }
}

//Affichage d'une déclaration (ajoutée)
void print_protected(Protected * pr){
  if (pr != NULL) {
    print_key(pr->pKey);
    printf(" %s ", pr->mess);
    print_signature(pr->sgn);
  }
}

//Q5.9
void print_list_protected(CellProtected * LCP){
  while (LCP != NULL){
    print_protected(LCP->data);
    LCP = LCP->next;
  }
}

//Q5.10
void delete_cell_protected(CellProtected * c){
  if (c != NULL){
    free_protected(c->data);
    free(c);
  }
}

void delete_list_protected(CellProtected ** LCP){
  if (LCP != NULL){
    CellProtected * tmp;
    while (*LCP){
      tmp = (*LCP)->next;
      delete_cell_protected(*LCP);
      *LCP = tmp;
    }
  }
}

/* Exercice 6 - Détermination du gagnant de l'élection */

//Q6.1
void delete_invalid_protected(CellProtected ** LCP) {
  if (LCP != NULL) {
    if (verify((*LCP)->data) == 0) {
      CellProtected * cp = (*LCP)->next;
      delete_cell_protected((*LCP));
      (*LCP) = cp;
      cp = NULL;
    }
    CellProtected * tmp = (*LCP);
    CellProtected * suiv = tmp->next;
    while (suiv) {
      if (verify(suiv->data) == 0) {
        tmp->next = suiv->next;
        delete_cell_protected(suiv);
        suiv = tmp->next;
      }
      else{
        tmp = tmp->next;
  			suiv = suiv->next;
      }
    }
    tmp = NULL;
  }
}

//Q6.2
HashCell  * create_hashcell(Key  * key){
  HashCell * hc = (HashCell *) malloc (sizeof(HashCell));
  if (hc == NULL){
    printf("Erreur malloc HashCell\n");
    exit(EXIT_FAILURE);
  }

  hc->key = keydup(key);
  hc->val = 0;
  return hc;
}

//Affichage d'une cellule de hachage sous la forme "Clé : Key, Valeur : int" (ajoutée)
void print_hashcell(HashCell * hc) {
  printf("Clé : ");
  print_key(hc->key);
  printf(", Valeur : %d\n", hc->val);
}

//Affichage d'une table de hachage (ajoutée)
void print_hashtable(HashTable * t) {
  for (int i = 0; i < t->size; i++) {
    printf("%d : ", i);
    if (t->tab[i] != NULL && t->tab[i]->key != NULL) {
      print_hashcell(t->tab[i]);
    } else {
      printf("vide\n");
    }
  }
}

//Q6.3
int hash_function(Key * key, int size){
  if (key != NULL) {
    return (key->val+key->n)%size;
  }
  return 0;
}

//Q6.4
int find_position(HashTable * t, Key * key){
  int size = t->size;
  int pos = hash_function(key, size);

  //Si la case n'est pas vide et qu'il ne s'agit pas de la bonne clé
  if (t->tab[pos] != NULL && keycmp(t->tab[pos]->key, key) == 0) {

    int pos_depart = pos;
    pos = (pos + 1) % size;

    //Parcours de la table de hachage jusqu'à trouver la bonne clé
    while (pos != pos_depart) {
			if (t->tab[pos] != NULL && keycmp(t->tab[pos]->key, key) == 1) {
				break;
			}
      pos = (pos + 1) % size;
    }

    //Si on ne trouve pas la clé
    if (pos == pos_depart) {
      pos = (pos + 1) % size;

      //Parcours de la table de hachage jusqu'à trouver une case vide
      while (t->tab[pos] != NULL && pos != pos_depart) {
        pos = (pos + 1) % size;
      }

      //S'il n'y pas plus de place dans la table de hachage
      if (pos == pos_depart) {
        return -1;
      }
    }
  }
  return pos;
}

//Q6.5
HashTable * create_hashtable(CellKey * keys, int size){

  //Création de la table de hachage
  HashTable * ht = (HashTable *) malloc(sizeof(HashTable));
  if (ht == NULL){
    printf("Erreur malloc HashCell\n");
    exit(EXIT_FAILURE);
  }

  //Création du tableau de la table de hachage
  ht->size = size;
  ht->tab = (HashCell **) malloc (sizeof(HashCell *) * size);
  if (ht->tab == NULL){
    printf("Erreur malloc HashCell\n");
    exit(EXIT_FAILURE);
  }

  //Initialisation du tableau de la table de hachage à NULL
  int i;
  for (i = 0; i < size; i++) {
    ht->tab[i] = NULL;
  }

  //Parcours des clés à insérer dans la table de hachage
  int pos;
  while (keys != NULL) {

    pos = find_position(ht, keys->data);

    //S'il n'y a plus de place dans la table de hachage
    if (pos == -1) {
			printf("Plus de place dans la table\n");
      break;
    }

    ht->tab[pos] = create_hashcell(keys->data);
    keys = keys->next;
  }
  return ht;
}

//Suppression d'une cellule de hachage (ajoutée)
void delete_hashcell(HashCell * c){
  if (c != NULL) {
    free(c->key);
    free(c);
  }
}

//Q6.6
void delete_hashtable(HashTable * t){
  if (t != NULL) {
    for (size_t i = 0; i < t->size; i++) {
      delete_hashcell(t->tab[i]);
    }
    free(t->tab);
    free(t);
  }
}

//Q6.7
Key * compute_winner(CellProtected * decl, CellKey * candidates, CellKey * voters, int sizeC, int sizeV){

  //Création des tables de hachage des candidats et des votants
  HashTable * Hc = create_hashtable(candidates, sizeC);
  HashTable * Hv = create_hashtable(voters, sizeV);

  //Suppression des fausses déclarations
  delete_invalid_protected(&decl);

  //Vote
  while (decl != NULL) {

    int posV = find_position(Hv, decl->data->pKey);
    Key * tmp1 = str_to_key(decl->data->mess);
    int posC = find_position(Hc, tmp1);

    //Vérification (la personne peut voter, n'a pas déjà voté et son vote porte sur un candidat)
    if (Hv->tab[posV] != NULL && Hv->tab[posV]->val == 0 && Hc->tab[posC] != NULL) {
      Hv->tab[posV]->val = 1;
      Hc->tab[posC]->val = Hc->tab[posC]->val + 1;
    }

    decl = decl->next;
    free(tmp1);
  }

  //Détermination du gagnant
  HashCell * tmp2 = NULL;
  Key * winner = NULL;
  int pos = 0;
  int nb_votes_total = 0;
  int nb_votes_gagnant = 0;

  while (pos != sizeC) {

    if (Hc->tab[pos] != NULL) {
      if (tmp2 == NULL || tmp2->val < Hc->tab[pos]->val) {
        tmp2 = Hc->tab[pos];
        nb_votes_gagnant = tmp2->val;
      }
      nb_votes_total += Hc->tab[pos]->val;
    }

    pos++;
  }
  if (tmp2 != NULL) {
    winner = keydup(tmp2->key);
    printf("Nombre de votes total : %d\n", nb_votes_total);

    if (winner != NULL) {
      printf("Gagnant déterminé : ");
      print_key(winner);
      printf(" avec %d votes.\n", nb_votes_gagnant);
    }
  }

  //Libération mémoire
  delete_hashtable(Hc);
  delete_hashtable(Hv);

  return winner;
}
