#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>

// C++ program Miller-Rabin primality test
#include <bits/stdc++.h>
using namespace std;

// Utility function to do modular exponentiation.
// It returns (x^y) % p
long int power(long int x, unsigned long int y, long int p){
	long int res = 1;	 // Initialize result
	x = x % p; // Update x if it is more than or
				// equal to p
	while (y > 0){
		// If y is odd, multiply x with result
		if (y & 1)
			res = (res*x) % p;

		// y must be even now
		y = y>>1; // y = y/2
		x = (x*x) % p;
	}
	return res;
}

// This function is called for all k trials. It returns
// false if n is composite and returns false if n is
// probably prime.
// d is an odd number such that d*2<sup>r</sup> = n-1
// for some r >= 1
bool miillerTest(long int d, long int n){
	// Pick a random number in [2..n-2]
	// Corner cases make sure that n > 4
	long int a = 2 + rand() % (n - 4);

	// Compute a^d % n
	long int x = power(a, d, n);

	if (x == 1 || x == n-1){
		return true;
	}

	// Keep squaring x while one of the following doesn't
	// happen
	// (i) d does not reach n-1
	// (ii) (x^2) % n is not 1
	// (iii) (x^2) % n is not n-1
	while (d != n-1){
		x = (x * x) % n;
		d *= 2;

		if (x == 1)	 return false;
		if (x == n-1) return true;
	}

	// Return composite
	return false;
}

// It returns false if n is composite and returns true if n
// is probably prime. k is an input parameter that determines
// accuracy level. Higher value of k indicates more accuracy.
bool isPrime(long int n, long int k)
{
	// Corner cases
	if (n <= 1 || n == 4) return false;
	if (n <= 3) return true;

	// Find r such that n = 2^d * r + 1 for some r >= 1
	long int d = n - 1;
	while (d % 2 == 0)
		d /= 2;

	// Iterate given nber of 'k' times
	for (long int i = 0; i < k; i++){
		if (miillerTest(d, n) == false){
			return false;
		}
	}
		
	return true;
}

long int millerRabin(long int number){
  long int k = 10; // Number of iterations

	long int prime, n;
	for (n = 1; n < number; n++){
    if (isPrime(n, k)){
        prime = n;
    }
  }
	return n;
}


/*Acha o primo 1*/
long int findPrimo(){
    long int number = 1, number2 = 0, seed = 0;
    long int i = 0, j = 0, *aux, returnNumber = 0;

    srand((unsigned)time(NULL));

    number2 = rand();

    number = number2*number2;

    number = number % 10000000;

    return millerRabin(number);
}

/*Acha o primo 2*/
long int findPrimo2(){

	long int number = 1, number2 = 0, seed = 0;

	srand( (unsigned)time(NULL) );
	number2 = rand()/7;

	number = number2*number2;

	number = number % 10000000;

  return millerRabin(number);
}

/*Dois n�meros s�o primos entre si se, e somente se, o MDC
(M�ximo divisor comum) entre os dois � 1.
Essa fun��o calcula o MDC, se ele for igual a 1, retorna 0,
caso contr�rio, retorna o mdc*/
int primeBetween(long int x, long int y){
    long int resto = 0, biggest = 0,  smallest = 0, aux = 0;

    /*Tratamos o caso onde o e � maior que o z*/
    if(x > y){
        biggest = x;
        smallest = y;
    } else{
        biggest = y;
        smallest = x;
    }

    /*Encontramos o MDC, caso o resto seja 1, retornamos verdade
    caso contr�rio, retornamos o menor divisor comum*/
    resto = biggest%smallest;

    while(resto != 0 || resto != 1){
        if(resto == 0 || resto == 1){
          if(resto == 1)
            aux = 0;

          if(resto==0)
            aux = smallest;

          return aux;
        }else{
          biggest = smallest;
          smallest = resto;
        }
        resto = biggest%smallest;
    }

    return smallest;
}

/*Encontra o valor de e.
e precisa ser primo com z, e menor que n*/
long int findE(long int z, long int n){
    long int e = 1, aux = 1;

    while(aux != 0 && e < n){
        aux = primeBetween(z, e);
     //   printf("%d ", aux);
        e++;
    }
    //printf("%d ", e - 1);
 return (e - 1);
}

/*Algoritmo de Eucldies estendido, implementado com
o aux�lio da fun��o "primeBetween"*/
long int findD(long int z, long int e){
    long int mdc = 0, aux = 1, i = 0, j = 0, biggest = 0, smallest = 0;

    /*Tratamos o caso onde o e � maior que o z*/
    if(z > e){
        biggest = z;
        smallest = e;
    }else{
        biggest = e;
        smallest = z;
    }

    mdc = primeBetween(biggest, smallest);

    for(i = 0; i < e; i++){
        for(j = 0; j < z; j++){
            aux = (j*e) - (i*z);

            if(aux == 1){
              //  printf("%ld %ld\n", j, i);
                return j;
            }
        }
    }
    return -1;
}

/*Transforma a mensagem (que est� em texto ASCI) em
n�meros - no nosso caso, h� a tradu��o do ASCI para
o seu respectivo inteiro*/
void transformInNumber(char *message, long int *m){
    int size = 0, i = 0;
    size = strlen(message);

        for(i = 0; i < size; i++){
            m[i] = (long int)message[i];
         //   printf("%ld ", m[i]);
        }
}

/*
Transforma a mensagem de n�meros, para textos
*/
char* transformInAscii(long int *message, int size){
    int i = 0;
    char *m;

    m = (char*)malloc(size*sizeof(char));

    for(i = 0; i < size; i++){
        m[i] = (char)message[i];
    }

    return m;
}

/* Como os n�meros podem ficar imensos, fa�o a exponencia��o
por meio de uma opera��o bit a bit
 */
int myPow(long long a, long long b, int c) {
	int res = 1;
	while(b > 0) {
		/* Need long multiplication else this will overflow... */
		if(b & 1) {
			res = (res * a) % c;
		}
		b = b >> 1;
		a = (a * a) % c; /* Same deal here */
	}
	return res;
}

long int* rsaEncode(long int *m, long int e, long int n, long int size){
    long int i = 0, *c;

    c = (long int*)malloc(size*sizeof(long int));

    for(i = 0; i < size; i++){
        c[i] = myPow(m[i], e, n);
    }

    return c;
}

long int* rsaDecode(long int *c, long int d, long int n, long int size){
    long int i = 0, *m;

    m = (long int*)malloc(size*sizeof(long int));

    for(i = 0; i < size; i++){
        m[i] = myPow(c[i], d, n);
    }

    return m;
}


char* readFile(FILE *fp, int *size){
  char *message = NULL;
  char c;

  do{
    message = (char*) realloc(message, (*size + 1) * sizeof(char));
    c = fgetc(fp);
    message[*size] = c;
    *size = *size + 1;
  }while(c != EOF);

  *size = *size - 1;

  return message;
}

int main(int argc, char* argv[]){
  FILE *fp;
  int size = 0, i;

  long int p, q, e = 0, d = 0;
  long int n, z;
  long int *v2, *v3;

  char *m, *message;

  if(argc != 2){
    printf("Usage: ./rsa file.txt\n");
    exit(0);
  }

  fp = fopen(argv[1], "r");

  if(fp == NULL){
    printf("I could not open this file\n");
    exit(0);
  }

  message = readFile(fp, &size);

  p = findPrimo();
	printf("achou o primeiro\n");
  q = findPrimo2();
	printf("achou o segundo\n");

  n = p*q;
  z = (p-1)*(q-1);


  e = findE(z, n);
	printf("acho o e\n");

  d = findD(z, e);
	printf("achou o d\n");

  long int* vet = (long int*) malloc(sizeof(long int)* size);

  transformInNumber(message, vet);
	printf("transformou a msg em numero\n");
  v2 = rsaEncode(vet, e, n, size);

	printf("ENCODE REALIZADO\n");
  v3 = rsaDecode(v2, d, n, size);
	printf("DECODE REALIZADO\n");
  m = transformInAscii(v3, size);


  for(i = 0; i < size; i++){
    printf("%c", m[i]);
  }

  fclose(fp);
  free(vet);
  free(message);

  return 0;
}
