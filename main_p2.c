#include "partie2.h"

void print_long_vector(long *result, int size) {
	printf("[");
	for (int i = 0; i < size - 1; i++) {
		printf("%lx\t", result[i]);
	}
	printf("%lx]\n", result[size - 1]);
}

int main(int argc, char const *argv[]) {
	srand(time(NULL));

	//Test Exercice 3

	//Testing Init Keys
	Key * pKey = malloc(sizeof(Key));
	Key * sKey = malloc(sizeof(Key));
	init_pair_keys(pKey, sKey, 3, 7);
	printf("pKey : %lx, %lx \n", pKey->val, pKey->n);
	printf("sKey : %lx, %lx \n", sKey->val, sKey->n);

	//Testing Key Serialization
	char * chaine = key_to_str(pKey);
	printf("key_to_str : %s \n", chaine);
	Key * k = str_to_key(chaine);
	printf("str_to_key : %lx, %lx \n", k->val, k->n);

	//Testing signature
	//Candidate keys:
	Key * pKeyC = malloc(sizeof(Key));
	Key * sKeyC = malloc(sizeof(Key));
	init_pair_keys(pKeyC, sKeyC, 3, 7);

	//Declaration:
	char * mess = key_to_str(pKeyC);
	char * tmp;
	printf("%s vote pour %s \n", tmp = key_to_str(pKey), mess);
	Signature * sgn = sign(mess, sKey);
	printf("Signature : ");
	print_long_vector(sgn->content, sgn->size);
	free(chaine);
	chaine = signature_to_str(sgn);
	printf("signature_to_str : %s \n", chaine);
	free_signature(sgn);
	sgn = str_to_signature(chaine);
	printf("str_to_signature : ");
	print_long_vector(sgn->content, sgn->size);

	//Testing protected:
	Protected * pr = init_protected(pKey, mess, sgn);

	//Verification:
	if(verify(pr)) {
		printf("Signature valide\n");
	} else {
		printf("Signature non valide\n");
	}
	char * tmp1, * tmp2;
	free(chaine);
	chaine = protected_to_str(pr);
	printf("protected_to_str : %s \n", chaine);
	free_protected(pr);
	pr = str_to_protected(chaine);
	printf("str_to_protected : %s %s %s \n", tmp1 = key_to_str(pr->pKey), pr->mess, tmp2 = signature_to_str(pr->sgn));

	//Libération mémoire
	free(pKey);
	free(sKey);
	free(pKeyC);
	free(sKeyC);
	free(k);
	free(chaine);
	free(tmp);
	free(mess);
	free_signature(sgn);
	free(tmp1);
	free(tmp2);
	free_protected(pr);

	//Test Exercice 4
	generate_random_data(1000, 5);
	return 0;
}
