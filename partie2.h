#ifndef PARTIE2_H
#define PARTIE2_H

#include "partie1.h"

/* Exercice 3 - Manipulation de structures sécurisées */

//Q3.1
typedef struct key {
	long val;
	long n;
} Key;

//Q3.5
typedef struct signature {
	long * content;
	int size;
} Signature;

//Q3.9
typedef struct protected {
	Key * pKey;
	char * mess;
	Signature * sgn;
} Protected;

/* Exercice 4 - Création de données pour simuler le processus de vote */

//Structure Cel ajoutée, correspondant à un citoyen : couple clé publique/clé privée
typedef struct carte_electorale {
	Key * pKey;
	Key * sKey;
} Cel;

/* Exercice 3 - Manipulation de structures sécurisées */

//Q3.2
void init_key (Key * key, long val, long n);
//Duplication d'une clé (ajoutée)
Key * keydup(Key * key);
//Comparaison de deux clés (ajoutée)
int keycmp(Key * k1, Key * k2);
//Q3.3
void init_pair_keys(Key * pkey, Key * skey, long low_size, long up_size);
//Q3.4
char * key_to_str(Key * key);
Key * str_to_key(char * str);

//Q3.6
Signature * init_signature(long * content, int size);
//Duplication d'une signature (ajoutée)
Signature * sgndup (Signature * sgn);
//Q3.7
Signature * sign(char * mess, Key * skey);
//Q3.8
char * signature_to_str(Signature * sgn);
Signature * str_to_signature(char * str);

//Q3.10
Protected * init_protected(Key * pkey, char * mess, Signature * sign);
//Q3.11
int verify(Protected * pr);
//Q3.12
char * protected_to_str(Protected * pr);
Protected * str_to_protected(char * str);

//Libération de la mémoire (ajoutées)
void free_signature(Signature * sign);
void free_protected(Protected * pr);

/* Exercice 4 - Création de données pour simuler le processus de vote */

//Q4.1
void generate_random_data(int nv, int nc);

#endif
