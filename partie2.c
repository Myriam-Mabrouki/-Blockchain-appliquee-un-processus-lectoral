#include "partie2.h"

/* Exercice 3 - Manipulation de structures sécurisées */

//Q3.2
void init_key(Key * key, long val, long n){
	key->val = val;
	key->n = n;
}

//Duplication d'une clé (ajoutée)
Key * keydup(Key * key){
	Key * dup = (Key *) malloc(sizeof(Key));
	if (dup == NULL){
    printf("Erreur malloc Key\n");
    exit(EXIT_FAILURE);
  }
	init_key(dup, key->val, key->n);
	return dup;
}

//Comparaison de deux clés (ajoutée)
int keycmp(Key * k1, Key * k2){
  if (k1 == NULL || k2 == NULL) {
    return 0;
  }
  return (k1->val == k2->val) && (k1->n == k2->n);
}

//Q3.3
void init_pair_keys(Key * pkey, Key * skey, long low_size, long up_size){

	long p = random_prime_number(low_size, up_size, 5000);
	long q = random_prime_number(low_size, up_size, 5000);


	while(p == q) {
		q = random_prime_number(low_size, up_size, 5000);
	}

	long n, s, u;
	generate_key_values(p, q, &n, &s, &u);

	//Pour avoir des cles positives :
	if (u < 0) {
		long t = (p - 1) * (q - 1);
		u = u + t; //On aura toujours s * u mod t = 1
	}

	init_key(pkey, s, n);
	init_key(skey, u, n);
}

//Q3.4
char * key_to_str(Key * key){
	char * str =(char *) malloc(sizeof(char) *(20));
	if (str == NULL){
    printf("Erreur malloc char\n");
    exit(EXIT_FAILURE);
  }
	sprintf(str, "(%lx,%lx)", key->val, key->n);
	return str;
}

Key * str_to_key(char * str){
	long val, n;
	if (sscanf(str, "(%lx,%lx)", &val, &n) == 2){
		Key * key =(Key *) malloc(sizeof(Key));
		if (key == NULL){
	    printf("Erreur malloc Key\n");
	    exit(EXIT_FAILURE);
	  }
		key->val = val;
		key->n = n;
		return key;
	}
	else {
		printf("Format de ligne incorrect\n");
		return NULL;
	}
}

//Q3.6
Signature * init_signature(long * content, int size){
	Signature * sign =(Signature *) malloc(sizeof(Signature));
	if (sign == NULL){
    printf("Erreur malloc Signature\n");
    exit(EXIT_FAILURE);
  }
	sign->content = content;
	sign->size = size;
	return sign;
}

//Duplication d'une signature (ajoutée)
Signature * sgndup (Signature * sgn){
	long * content = malloc (sizeof(long) * sgn->size);
	if (content == NULL){
    printf("Erreur malloc long\n");
    exit(EXIT_FAILURE);
  }
	for (size_t i = 0; i < sgn->size; i++) {
		content[i] = sgn->content[i];
	}
	return init_signature(content, sgn->size);
}

//Q3.7
Signature * sign(char * mess, Key * skey){
	long * tab = encrypt(mess, skey->val, skey->n);
	int size = strlen(mess);
	return init_signature(tab, size);
}

//Q3.8
char * signature_to_str(Signature * sgn) {
	char * result = malloc(10 * sgn->size * sizeof(char));
	if (result == NULL){
    printf("Erreur malloc char\n");
    exit(EXIT_FAILURE);
  }
	result[0]= '#';
	int pos = 1;
	char buffer [156];
	for(int i = 0; i < sgn->size; i ++) {
		sprintf(buffer, "%lx", sgn->content [i]);
		for(int j = 0; j < strlen(buffer); j ++) {
			result[pos] = buffer[j];
			pos = pos + 1;
		}
		result[pos] = '#';
		pos = pos + 1;
	}
	result[pos] = '\0';
	result = realloc(result,(pos + 1) * sizeof(char));
	if (result == NULL){
    printf("Erreur malloc char\n");
    exit(EXIT_FAILURE);
  }
	return result;
}

Signature * str_to_signature(char * str) {
	int len = strlen(str);
	long * content =(long *) malloc(sizeof(long) * len);
	if (content == NULL){
    printf("Erreur malloc long\n");
    exit(EXIT_FAILURE);
  }
	int num = 0;
	char buffer[256];
	int pos = 0;
		for(int i = 0; i < len; i ++) {
		if(str[i] != '#') {
			buffer[pos] = str[i];
			pos = pos + 1;
		} else {
			if(pos != 0) {
				buffer [pos] = '\0';
				if (sscanf(buffer, "%lx", &(content[num])) == 1){
					num = num + 1;
					pos = 0;
				}
				else{
					printf("Format de ligne incorrect\n");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	content = realloc(content, num * sizeof(long));
	if (content == NULL){
    printf("Erreur malloc long\n");
    exit(EXIT_FAILURE);
  }
	return init_signature(content, num);
}

//Q3.10
Protected * init_protected(Key * pkey, char * mess, Signature * sign){
	Protected * pr = (Protected *) malloc(sizeof(Protected));
	if (pr == NULL){
    printf("Erreur malloc Protected\n");
    exit(EXIT_FAILURE);
  }
	pr->pKey = keydup(pkey);
	pr->mess = strdup(mess);
	pr->sgn = sgndup(sign);
	return pr;
}

//Q3.11 Vérifie que la signature décryptée avec la clé publique correspond au message
int verify(Protected * pr){
	if(pr == NULL) {
		return 0;
	}

	char * decoded = decrypt(pr->sgn->content, pr->sgn->size, pr->pKey->val, pr->pKey->n);

	int res = strcmp(pr->mess, decoded);
	free_decrypt(decoded);

	if (res== 0) {
		return 1;
	}
	return  0;
}

//Q3.12
char * protected_to_str(Protected * pr){
	char * str_key = key_to_str(pr->pKey);
	char * str_sgn = signature_to_str(pr->sgn);
	char * str =(char *) malloc(sizeof(char) *(strlen(str_key) + strlen(str_sgn) + strlen(pr->mess) + 5));
	if (str == NULL){
    printf("Erreur malloc char\n");
    exit(EXIT_FAILURE);
  }
	sprintf(str, "%s %s %s", str_key, pr->mess, str_sgn);
	free(str_key);
	free(str_sgn);
	return str;
}

Protected * str_to_protected(char * str){
	char str_pkey[20], mess[256], str_sign[256];
	if (sscanf(str, "%s %s %s", str_pkey, mess, str_sign) == 3){
		Key * pkey = str_to_key(str_pkey);
		Signature * sign = str_to_signature(str_sign);
		Protected * pr = init_protected(pkey, mess, sign);
		free(pkey);
		free_signature(sign);
		return pr;
	}
	else{
		printf("Format de ligne incorrect\n");
		return NULL;
	}
}

//Libération de la mémoire (ajoutées)

void free_signature(Signature * sign){
	if (sign != NULL) {
		free(sign->content);
		free(sign);
	}
}

void free_protected(Protected * pr){
	if (pr != NULL) {
		free(pr->pKey);
		free(pr->mess);
		free_signature(pr->sgn);
		free(pr);
	}
}



/* Exercice 4 - Création de données pour simuler le processus de vote */

//Q4.1 Génère nv couples de clés différents, les écrit dans le fichier keys.txt, sélectionne nc candidats dans le fichier candidates.txt, et fait voter tout le monde dans declarations.txt
void generate_random_data(int nv, int nc) {
	int tmp;
	char *mess;
	srand(time(NULL));
	//Initialisation du tableau de nv couples de cles
	Key *pKey, *sKey;
	Cel * tkeys = (Cel*)malloc(nv * sizeof(Cel)); //Structure Cel correspondant à un citoyen : couple clé publique/clé privée
	if (tkeys == NULL){
    printf("Erreur malloc Cel\n");
    exit(EXIT_FAILURE);
  }

	//Ouverture du fichier keys.txt
	FILE* k = fopen("keys.txt", "w");
	if (! k) {
		printf("Erreur à l'ouverture du fichier keys.txt\n");
		return;
	}

	//Generation des nv couples de cles et ecriture dans le fichier
	char * tmp1, * tmp2;
	for (int i = 0; i < nv; i++) {
		pKey = (Key *)malloc(sizeof(Key));
		sKey = (Key *)malloc(sizeof(Key));
		if (pKey == NULL || sKey == NULL){
	    printf("Erreur malloc Key\n");
	    exit(EXIT_FAILURE);
	  }
		init_pair_keys(pKey, sKey, 3, 7);
		tkeys[i].pKey = keydup(pKey);
		tkeys[i].sKey = keydup(sKey);
		tmp1 = key_to_str(pKey);
		tmp2 = key_to_str(sKey);
		fprintf(k, "%s, %s\n", tmp1, tmp2);
		free(tmp1);
		free(tmp2);
		free(pKey);
		free(sKey);
	}

	//Initialisation du tableau de nc cle de candidats
	Key ** tcand = (Key **)malloc(sizeof(Key) * nc);
	if (tcand == NULL){
    printf("Erreur malloc Key\n");
    exit(EXIT_FAILURE);
  }
	Signature * sgn;

	//Ouverture du fichier candidates.txt
	FILE* c = fopen("candidates.txt", "w");
	if (! c) {
		printf("Erreur à l'ouverture du fichier candidates.txt\n");
		return;
	}

	//Generation des nc cles candidates et ecriture dans le fichier
	char * tmp3;
	for (int i = 0; i < nc; i++) {
		tmp = rand() % nv;
		tcand[i] = tkeys[tmp].pKey;
		tmp3 = key_to_str(tcand[i]);
		fprintf(c, "%s\n", tmp3);
		free(tmp3);
	}

	//Initialisation du tableau de declarations
	Protected ** decl = (Protected **)malloc(sizeof(Protected) * nv);
	if (decl == NULL){
    printf("Erreur malloc Protected\n");
    exit(EXIT_FAILURE);
  }

	//Ouverture du fichier declarations.txt
	FILE* d = fopen("declarations.txt", "w");
	if (! d) {
		printf("Erreur à l'ouverture du fichier declarations.txt\n");
		return;
	}

	//Generation des declarations et ecriture dans le fichier
	//init_protected(Key * pkey, char * mess, Signature * sign)
	for (int i = 0; i < nv; i++) {
		tmp = rand() % nc;
		mess = key_to_str(tcand[tmp]);
		sgn = sign(mess, tkeys[i].sKey);
		decl[i] = init_protected(tkeys[i].pKey, mess, sgn);
		tmp1 = protected_to_str(decl[i]);
		fprintf(d, "%s\n", tmp1);
		free(tmp1);
		free(mess);
		free_signature(sgn);
	}

	//Fermeture des fichiers
	fclose(k);
	fclose(c);
	fclose(d);

	//Libération de la mémoire
	for (int i = 0; i < nv; i++) {
		free(tkeys[i].pKey);
		free(tkeys[i].sKey);
		free_protected(decl[i]);
	}

	free(tkeys);
	free(tcand);
	free(decl);

}
