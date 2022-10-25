#include "partie1.h"

/* Exercice 1 - Resolution du problème de primalite */

//Q1.1 Teste si p est premier de maniere naive
int is_prime_naive(long p){

  if (p == 1 || p == 0){
    return 0;
  }

  for (size_t i = 2; i < p; i++) { //on commence à 2 et finit à p - 1 car tout nombre p est divisible par 1 et lui-même
    if (p%i == 0){
      return 0; //si p est divisible par un nombre i différent de 1 ou lui-même alors il n'est pas premier
    }
  }
  return 1;
}

//Q1.3 Exponentiation modulaire naïve
long modpow_naive(long a, long m, long n){
  long res = 1;
  for (long i = 0; i < m; i++) {
    res = res * a;
    res = res % n;
  }
  return res;
}

//Q1.4 Exponentiation modulaire rapide
long modpow (long a, long m, long n){

  if (m == 0){
    return 1;
  }

  if (m == 1){
    return a % n;
  }

  long b = modpow (a, m / 2, n);

  if (m % 2 == 0){
    return (b * b) % n;
  }

  else{
    return (a * b * b) % n;
  }

}

//Q1.6 Test de Miller-Rabin
int witness (long a, long b, long d, long p){
  long x = modpow(a, d, p);
  if (x == 1){
    return 0;
  }
  for (long i = 0; i < b; i++) {
    if (x == p-1){
      return 0;
    }
    x = modpow(x, 2, p);
  }
  return 1;
}

long rand_long(long low, long up){
  return rand() % (up - low + 1) + low;
}

int is_prime_miller(long p, int k){
  if (p == 2){
    return 1;
  }
  if (!(p & 1) || p <= 1){ //on vérifie que p est impair et différent de 1;
    return 0;
  }
  //on détermine b et d
  long b = 0;
  long d = p - 1;
  while (!(d & 1)) { //tant que d n'est pas impair
    d = d/2;
    b = b + 1;
  }
  // On génère k valeurs pour a, et on teste si c'est un faux témoin
  long a;
  int i;
  for (i = 0; i < k; i++) {
    a = rand_long (2,  p-1);
    if (witness(a, b, d, p)){
      return 0;
    }
  }
  return 1;
}


//Q1.8 Retoune un nombre premier de taille comprise entre low_size et up_size
long random_prime_number(int low_size, int up_size, int k){

  long p, up = 1, low = 1;
  int trouve = 0;

  //Initialisation du plus petit entier possible
  for (int i = 0; i < low_size-1; i++) {
	  low *= 2;
  }

  //Initialisation du plus grand entier possible
  for (int i = 0; i < up_size; i++) {
	  up *= 2;
  }
  up--;

  while (trouve == 0) {
    p = rand_long(low, up);
    trouve = is_prime_miller(p, k);
  }

  return p;
}


/* Exercice 2 - Implementation du protocole RSA */

// Algorithme d'Euclide etendu pour calculer le pgcd
long extended_gcd(long s, long t, long *u, long *v) {
	if (s == 0) {
		*u = 0;
		*v = 1;
		return t;
	}

	long uPrim, vPrim;
	long gcd = extended_gcd(t % s, s, &uPrim, &vPrim);

	*u = vPrim - (t/s) * uPrim;
	*v = uPrim;

	return gcd;
}

//Q2.1 Gerere la cle publique et la cle secrete en suivant le protocole RSA
void generate_key_values(long p, long q, long *n, long *s, long *u) {

	*n = p * q;

	long t = (p - 1) * (q - 1);

	*s = rand_long(1, t);

	long v = 0;
	while (extended_gcd(*s, t, u, &v) != 1) {
		*s = rand_long(1, t);
	}
}

//Q2.2 chiffre chaine avec la cle publique (s,n)
long *encrypt(char* chaine, long s, long n) {
	int taillec = strlen(chaine);
	long *tabres = malloc(sizeof(long) * taillec);
  if (tabres == NULL){
    printf("Erreur malloc long\n");
    exit(EXIT_FAILURE);
  }

	for (int i = 0; i < taillec; i++) {
		tabres[i] = modpow(chaine[i], s, n);
	}

	return tabres;
}

//Q2.3 dechiffre crypted avec la cle privee (u, n)
char *decrypt(long* crypted, int size, long u, long n) {
	char* message = (char*)malloc(sizeof(char)*(size+1));
  if (message == NULL){
    printf("Erreur malloc char\n");
    exit(EXIT_FAILURE);
  }

	for (int i = 0; i < size; i++) {
		message[i] = modpow(crypted[i], u, n);
	}

	message[size] = '\0';

	return message;
}

//Fonctions pour libérer la mémoire (ajoutées)

void free_encrypt(long * encrypt) {
  if (encrypt != NULL) {
    free(encrypt);
  }
}

void free_decrypt(char * decrypt) {
  if (decrypt != NULL) {
    free(decrypt);
  }
}
