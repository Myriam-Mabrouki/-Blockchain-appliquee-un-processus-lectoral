#ifndef PARTIE4_H
#define PARTIE4_H

#include "partie3.h"

/* Exercice 7 - Structure d'un block et persistance */

typedef struct block {
	Key * author;
	CellProtected * votes;
	unsigned char * hash;
	unsigned char * previous_hash;
	int nonce;
} Block;

/* Exercice 8 - Structure arborescente */

typedef struct block_tree_cell {
  Block * block;
  struct block_tree_cell * father;
  struct block_tree_cell * firstChild;
  struct block_tree_cell * nextBro;
  int height;
} CellTree;

/* Exercice 7 - Structure d'un block et persistance */

//Q7.1
void write_block(Block * block, char * nom);
//Q7.2
Block * read_block (char * nom);
//Q7.3
char * block_to_str(Block * block);
//Q7.5
unsigned char * str_to_hash (char * str);
//Q7.6
void compute_proof_of_work(Block * b, int d);
//Q7.7
int verify_block (Block * b, int d);
//Q7.9
void delete_block(Block * b);

/* Exercice 8 - Structure arborescente */

//Q8.1
CellTree * create_node(Block * b);
//Q8.2
int update_height(CellTree * father, CellTree * child);
//Q8.3
void add_child(CellTree * father, CellTree * child);
//Affichage d'un noeud (ajoutée)
void print_node(CellTree * node);
//Q8.4
void print_tree(CellTree * ct);
//Q8.5
void delete_node(CellTree * node);
void delete_tree(CellTree * tree);
//Q8.6
CellTree * highest_child(CellTree * cell);
//Q8.7
CellTree * last_node(CellTree * tree);
//Q8.8
void fusion_declarations(CellProtected ** cp1, CellProtected * cp2);
//Q8.9
CellProtected * protected_in_highest_child(CellTree * tree);

#endif
