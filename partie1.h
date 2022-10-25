#ifndef PARTIE1_H
#define PARTIE1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>

/* Exercice 1 - Resolution du problème de primalite */

//Q1.1 Teste si p est premier de maniere naive
int is_prime_naive(long p);

//Q1.3 retourne a^m mod n de maniere naive
long modpow_naive(long a, long m, long n);
//Q1.4 retourne a^m mod n de maniere recursive (et de meilleure complexite)
long modpow(long a, long m, long n);

//Q1.6
// Teste si a est un temoin de Miller pour p
int witness(long a, long b, long d, long p);
// Retourne un entier long genere aleatoirement entre low et up inclus
long rand_long(long low, long up);
// Realise le test de Miller-Tabin en generant k valeurs de a au hasard et en testant si chaque valeur de a est un temoin de Miller
int is_prime_miller(long p, int k);

//Q1.8 Retourne un entier premier de taille comprise entre low_size et up_size
long random_prime_number(int low_size, int up_size, int k);


/* Exercice 2 - Implementation du protocole RSA */

// Algorithme d'Euclide etendu pour calculer le pgcd
long extended_gcd(long s, long t, long *u, long *v);

//Q2.1 Gerere la cle publique et la cle secrete en suivant le protocole RSA
void generate_key_values(long p, long q, long *n, long *s, long *u);

//Q2.2 chiffre chaine avec la cle publique (s,n)
long *encrypt(char* chaine, long s, long n);

//Q2.3 dechiffre crypted avec la cle privee (u, n)
char *decrypt(long* crypted, int size, long u, long n);

//Libération (ajoutées)
void free_encrypt(long * encrypt);
void free_decrypt(char * decrypt);

#endif
