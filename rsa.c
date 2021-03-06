#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<string.h>

#define MAX_RANDOM 100000
#define FALSE 0
#define TRUE 1
#define BOOL int

typedef unsigned long long int ulli;

ulli power(ulli x, ulli y, ulli p){
	ulli res = 1;
	x = x % p;
	while (y > 0){
		if (y & 1) res = (res*x) % p;
		y = y>>1;
		x = (x*x) % p;
	}
	return res;
}

BOOL miillerTest(ulli d, ulli n){
	ulli a = 2 + rand() % (n - 4);
	ulli x = power(a, d, n);

	if (x == 1 || x == n-1){
		return TRUE;
	}

	while (d != n-1){
		x = (x * x) % n;
		d *= 2;

		if (x == 1)	 return FALSE;
		if (x == n-1) return TRUE;
	}

	return FALSE;
}

BOOL isPrime(ulli n, ulli k){
	if (n <= 1 || n == 4) return FALSE;
	if (n <= 3) return TRUE;

	ulli d = n - 1;
	while (d % 2 == 0) d /= 2;

	for (ulli i = 0; i < k; i++){
		if (miillerTest(d, n) == FALSE){
			return FALSE;
		}
	}

	return TRUE;
}

ulli millerRabin(ulli number){
  ulli k = 1;
	ulli prime, n;
	for (n = 1; n < number; n++){
    if (isPrime(n, k)){
        prime = n;
    }
  }

	return prime;
}

ulli findPrimo(){
    ulli number = 1, number2 = 0, seed = 0;

    srand((unsigned)time(NULL));

    number2 = rand();
    number = number2*number2;
    number = number % MAX_RANDOM;

    return millerRabin(number);
}

ulli findPrimo2(){
	ulli number = 1, number2 = 0, seed = 0;

	srand( (unsigned)time(NULL) );

	number2 = rand()/7;
	number = number2*number2;
	number = number % MAX_RANDOM;

  return millerRabin(number);
}

int primeBetween(ulli x, ulli y){
    ulli resto = 0, biggest = 0,  smallest = 0, aux = 0;

    if(x > y){
        biggest = x;
        smallest = y;
    } else{
        biggest = y;
        smallest = x;
    }

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

ulli findE(ulli z, ulli n){
    ulli e = 1, aux = 1;

    while(aux != 0 && e < n){
        aux = primeBetween(z, e);
        e++;
    }
 return (e - 1);
}

ulli findD(ulli z, ulli e){
    ulli mdc = 0, aux = 1, i = 0, j = 0, biggest = 0, smallest = 0;

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
                return j;
            }
        }
    }
    return -1;
}

void transformInNumber(char *message, ulli *m, int size){
    int i = 0;
    for(i = 0; i < size; i++){
      m[i] = (ulli)message[i];
    }
}


char* transformInAscii(ulli *message, int size){
    int i = 0;
    char *m;

    m = (char*)malloc(size*sizeof(char));

    for(i = 0; i < size; i++){
        m[i] = (char)message[i];
    }

    return m;
}

void rsaEncode(ulli *m, ulli e, ulli n, int size){

		ulli i = 0;

    FILE* encodedFile = fopen("encoded.txt", "w");

		if(encodedFile == NULL){
			printf("I could not open this file\n");
			exit(0);
		}

    for(i = 0; i < size; i++){
        fprintf(encodedFile, "%llu\n", power(m[i], e, n));
    }

		fclose(encodedFile);

}

void rsaDecode(ulli d, ulli n, int size){
    ulli i = 0, c;
		char letter;

		FILE* encodedFile = fopen("encoded.txt", "r");
		FILE* decodedFile = fopen("decoded.txt", "w");

		if(encodedFile == NULL || decodedFile == NULL){
			printf("I could not open this file\n");
			exit(0);
		}

    for(i = 0; i < size; i++){
        fscanf(encodedFile, "%llu", &c);
				letter = (char)power(c, d, n);
				fprintf(decodedFile, "%c", letter);
    }


		fclose(encodedFile);
		fclose(decodedFile);

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

  ulli p, q, e = 0, d = 0;
  ulli n, z;

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

	fclose(fp);

  p = findPrimo();
  q = findPrimo2();

  n = p*q;
  z = (p-1)*(q-1);


  e = findE(z, n);

  d = findD(z, e);

  ulli* vet = (ulli*) malloc(sizeof(ulli)* size);

  transformInNumber(message, vet, size);
  rsaEncode(vet, e, n, size);
  rsaDecode(d, n, size);


  free(vet);
  free(message);

  return 0;
}
