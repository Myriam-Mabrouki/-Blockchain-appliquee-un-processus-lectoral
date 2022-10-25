#ifndef PARTIE5_H
#define PARTIE5_H

#include "partie4.h"
#include <dirent.h>

/* Exercice 9 - Simulation du processus de vote */

//Q9.1
void submit_vote(Protected * p);
//Q9.2
void create_block(CellTree * tree, Key * author, int d);
//Q9.3
void add_block (int d, char * name);
//Q9.4
CellTree * read_tree();
//Q9.5
Key * compute_winner_BT(CellTree * tree, CellKey * candidates, CellKey * voters, int sizeC, int sizeV);

#endif
