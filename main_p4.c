#include "partie4.h"

int main(int argc, char const *argv[]) {

  //Tests Exercice 8

  //Test Q7.4 et fonction str_to_hash
  const char * s = "Rosetta code";
  unsigned char * d1 = SHA256((unsigned char *) s, strlen(s), 0);
  unsigned char * d2 = str_to_hash((char *) s);

  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    printf("%02x", d1[i]);

  }

  putchar('\n');

  for (int i = 0; i < strlen((char *)d2); i++) {
    printf("%02x", d2[i]);
  }

  putchar('\n');

  free(d2);


  //Test compute_proof_of_work/verify

  generate_random_data(10, 3);

  Block * b = (Block *) malloc(sizeof(Block));
  CellProtected * votes = read_public_protected("declarations.txt");
  Key * author = str_to_key("(26d,87d)");

  b->author = keydup(author);
  b->votes = cellProtectedDup(votes);
  b->previous_hash = (unsigned char *) strdup("previous_hash");

  compute_proof_of_work(b, 3);

  printf("Block to string :\n");
  char * block = block_to_str(b);
  printf("%s\n", block);

  unsigned char * hash = str_to_hash(block);
  printf("String Block to hash :\n");
  printf("%s\n", hash);


  if (verify_block(b,1) == 1) {
    printf("Le bloc est valide.\n");
  }
  else{
    printf("Le bloc n'est pas valide.\n");
  }

  //Test lecture et écriture dans un fichier
  char * block1 = block_to_str(b);
  printf("Avant écriture et lecture\n");
  printf("%s\n", block);

  printf("Écriture du block\n");
  write_block(b, "block.txt");

  printf("Lecture du block \n");
  Block * b_read = read_block("block.txt");

  printf("Affichage du block lu\n");
  char * block2 = block_to_str(b_read);
  printf("%s\n", block2);


	//Évaluation du temps de compute_proof_of_work en fonction de d

  clock_t ti;
	clock_t tf;
	double tcpu;

	FILE* c = fopen("sortie_compute_pow.txt", "w");
	if (! c) {
		printf("Erreur à l'ouverture du fichier\n");
		return -1;
	}

	for (int d = 1; d < 6; d++) {
		ti = clock();
		compute_proof_of_work(b, d);
		tf = clock();
		tcpu = ((double)(tf - ti))/CLOCKS_PER_SEC;
		fprintf(c, "%d %f\n", d, tcpu);
	}

	fclose(c);

  //Libération mémoire
  free(block);
  free(block1);
  free(block2);
  free(hash);
  free(author);

  delete_list_protected(&votes);

  delete_block(b);
  delete_block(b_read);

  //Tests exercice 8
  //Création des blocks
  Block * b1 = (Block *) malloc(sizeof(Block));
  CellProtected * votes1 = read_public_protected("declarations.txt");
  Key * author1 = str_to_key("(26d,87d)");
  b1->author = keydup(author1);
  b1->votes = cellProtectedDup(votes1);
  b1->previous_hash = (unsigned char *) strdup("helloworldhelloworldhelloworldhelloworldhelloworld");
  compute_proof_of_work(b1, 1);

  generate_random_data(4, 2);

  Block * b2 = (Block *) malloc(sizeof(Block));
  CellProtected * votes2 = read_public_protected("declarations.txt");
  Key * author2 = str_to_key("(36b,629)");
  b2->author = keydup(author2);
  b2->votes = cellProtectedDup(votes2);
  b2->previous_hash = (unsigned char *) strdup((char *) b1->hash);
  compute_proof_of_work(b2, 1);

  generate_random_data(4, 2);

  Block * b3 = (Block *) malloc(sizeof(Block));
  CellProtected * votes3 = read_public_protected("declarations.txt");
  Key * author3 = str_to_key("(55d,71b)");
  b3->author = keydup(author3);
  b3->votes = cellProtectedDup(votes3);
  b3->previous_hash = (unsigned char *) strdup((char *) b1->hash);
  compute_proof_of_work(b3, 1);

  generate_random_data(4, 2);

  Block * b4 = (Block *) malloc(sizeof(Block));
  CellProtected * votes4 = read_public_protected("declarations.txt");
  Key * author4 = str_to_key("(2b,125b)");
  b4->author = keydup(author4);
  b4->votes = cellProtectedDup(votes4);
  b4->previous_hash = (unsigned char *) strdup((char *) b3->hash);
  compute_proof_of_work(b4, 1);

  generate_random_data(4, 2);

  Block * b5 = (Block *) malloc(sizeof(Block));
  CellProtected * votes5 = read_public_protected("declarations.txt");
  Key * author5 = str_to_key("(b83,c4d)");
  b5->author = keydup(author5);
  b5->votes = cellProtectedDup(votes5);
  b5->previous_hash = (unsigned char *) strdup((char *) b1->hash);
  compute_proof_of_work(b5, 1);

  //Création de l'arbre
  CellTree * node = NULL;
  CellTree * node1 = create_node(b1);
  CellTree * node2 = create_node(b2);
  CellTree * node3 = create_node(b3);
  CellTree * node4 = create_node(b4);
  CellTree * node5 = create_node(b5);
  add_child(node, node1);
  add_child(node1, node2);
  add_child(node1, node3);
  add_child(node1, node5);
  add_child(node3, node4);

  //Affichage de l'arbre
  print_tree(node);

  //Test last_node
  CellTree * ct = last_node(node1);
  if (ct != NULL) {
    print_node(ct);
    printf("\n");
  }

  //Test fusion
  CellProtected * cp = protected_in_highest_child(node1);
  printf("Liste fusion\n");
  print_list_protected(cp);

  //Libération mémoire
  free(author1);
  free(author2);
  free(author3);
  free(author4);
  free(author5);
  delete_list_protected(&votes1);
  delete_list_protected(&votes2);
  delete_list_protected(&votes3);
  delete_list_protected(&votes4);
  delete_list_protected(&votes5);
  delete_list_protected(&cp);

  delete_tree(node1);

  return 0;
}
