#include "partie5.h"

int main(int argc, char const *argv[]) {

  //Q9.6

  //Création du bloc initial
  generate_random_data(1, 5);

  Block * b = (Block *) malloc(sizeof(Block));
  CellProtected * decl = read_public_protected("declarations.txt");
  b->author = str_to_key("(26d,87d)");
  b->votes = cellProtectedDup(decl);
  b->previous_hash = (unsigned char *) strdup("root");
  compute_proof_of_work(b, 2);

  //Création de la racine
  CellTree * root = create_node(b);

  //Création des citoyens, candidats et déclarations de votes
  generate_random_data(1000, 5);
  CellKey * citoyens = read_public_keys("keys.txt");
  CellKey * candidats = read_public_keys("candidates.txt");
  CellProtected * votes = read_public_protected("declarations.txt");

  CellProtected * tmp_votes = votes;
  CellKey * tmp_citoyens = citoyens;

  char nom_fic[1024];

  int cpt = 0;

  Key * author;

  while (tmp_votes) {

    author = tmp_citoyens->data;

    //Soumission des votes
    for (size_t i = 0; i < 10; i++) {
      submit_vote(tmp_votes->data);
      tmp_votes = tmp_votes->next;
      tmp_citoyens = tmp_citoyens->next;
    }

    sprintf(nom_fic, "block%d", cpt);

    //Création d'un bloc valide et ajout dans le répertoire
    create_block(root, author, 2);
    add_block(2, nom_fic);

    cpt++;
  }

  //Lecture et affichage de l'arbre obtenu
  CellTree * tree = read_tree();
  print_tree(tree);
  printf("\n");

  //Détermination du gagnant
  Key * winner = compute_winner_BT(tree, candidats, citoyens, 5, 1000);


  //Libération mémoire
  delete_list_key(&citoyens);
	delete_list_key(&candidats);
  delete_list_protected(&votes);
  delete_list_protected(&decl);

  delete_tree(tree);
  delete_tree(root);
  free(winner);

  return 0;
}
