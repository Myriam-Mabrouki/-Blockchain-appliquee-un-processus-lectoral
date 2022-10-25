#include "partie3.h"

int main(int argc, char const *argv[]) {
	srand(time(NULL));

  //Tests Exercices 5 et 6

	//Simulation élection
	CellKey * voters = read_public_keys("keys.txt");
	CellKey * candidates = read_public_keys("candidates.txt");
	CellProtected * decl = read_public_protected("declarations.txt");
	Key * winner = compute_winner(decl, candidates, voters, 24, 2000);

	//Libération mémoire
	delete_list_key(&voters);
	delete_list_key(&candidates);
	delete_list_protected(&decl);
  if (winner != NULL) {
    free(winner);
  }

  return 0;
}
