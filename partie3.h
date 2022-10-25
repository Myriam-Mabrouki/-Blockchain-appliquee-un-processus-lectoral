#ifndef PARTIE3_H
#define PARTIE3_H

#include "partie2.h"

#define MAX_BUFFER_SIZE 1000

/* Exercice 5 - Lecture et stockage des données dans des listes chainées */

typedef struct cellKey {
  Key * data;
  struct cellKey * next;
} CellKey;

typedef struct cellProtected {
  Protected * data ;
  struct cellProtected * next;
} CellProtected;

/* Exercice 6 - Détermination du gagnant de l'élection */

typedef struct hashcell {
  Key * key;
  int val;
} HashCell;

typedef struct hashtable {
  HashCell ** tab;
  int size;
} HashTable;

/* Exercice 5 - Lecture et stockage des données dans des listes chainées */

//Q5.1
CellKey * create_cell_key(Key * key);
//Q5.2
void push_key (CellKey ** ck, Key * key);
//Q5.3
CellKey * read_public_keys (char * nom);
//Affichage d'une clé (ajoutée)
void print_key(Key * k);
//Q5.4
void print_list_keys(CellKey * LCK);
//Q5.5
void delete_cell_key(CellKey * c);
void delete_list_key(CellKey ** LCK);

//Q5.6
CellProtected * create_cell_protected(Protected * pr);
//Q5.7
void push_protected (CellProtected ** cp, Protected * pr);
//Duplication d'une liste chainée de déclarations (ajoutée)
CellProtected * cellProtectedDup(CellProtected * cp);
//Q5.8
CellProtected * read_public_protected (char * nom);
//Affichage d'une signature (ajoutée)
void print_signature(Signature * sgn);
//Affichage d'une déclaration (ajoutée)
void print_protected(Protected * pr);
//Q5.9
void print_list_protected(CellProtected * LCP);
//Q5.10
void delete_cell_protected(CellProtected * c);
void delete_list_protected(CellProtected ** LCP);

/* Exercice 6 - Détermination du gagnant de l'élection */

//Q6.1
void delete_invalid_protected(CellProtected ** LCP);
//Q6.2
HashCell  * create_hashcell(Key  * key);
//Affichage d'une HashCell sous la forme "Clé : Key, Valeur : int" (ajoutée)
void print_hashcell(HashCell * hc);
//Affichage d'une table de hachage (ajoutée)
void print_hashtable(HashTable * t);
//Q6.3
int hash_function(Key  * key, int size);
//Q6.4
int find_position(HashTable * t, Key * key);
//Q6.5
HashTable * create_hashtable(CellKey * keys, int size);
//Suppression d'une cellule de hachage (ajoutée)
void delete_hashcell(HashCell * c);
//Q6.6
void delete_hashtable(HashTable * t);
//Q6.7
Key * compute_winner(CellProtected * decl, CellKey * candidates, CellKey * voters, int sizeC, int sizeV);


#endif
