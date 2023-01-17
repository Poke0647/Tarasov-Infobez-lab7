
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include <time.h>

struct timespec start;

void PrintH (){
    system ("../helpprint.sh");
    _Exit (0);
}

mpz_t *PrimeNumber(int bitts){
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    unsigned long randseed = start.tv_nsec + start.tv_nsec * 32;


    /* unsigned long int randseed = (unsigned long int) time(NULL); */
    /* printf("%lu\n", randseed); */
    mpz_t a;
    gmp_randstate_t randd;

    mpz_init(a);

    gmp_randinit_mt(randd);
    /* gmp_randseed_ui(randd, randseed); */

    mpz_urandomb(a, randd, bitts);
    /* mpz_nextprime(a, a); */

    gmp_randclear(randd);
    mpz_t *res;
    res = &a;
    return res;
}

void KeyGen(/*char *isFile,*/ int bits){

    mpz_t fsPr;
    mpz_set(fsPr, *PrimeNumber(bits));
    mpz_t sndPr;
    mpz_set(sndPr, *PrimeNumber(bits));

    gmp_printf("p: %Zd \n", fsPr);
    gmp_printf("q: %Zd \n", sndPr);
}

int main (int argc, char *argv[]) {

    /* double ts = 0; */
    /* clock_t begin = clock(); */
    int kgBitsArg = atoi(argv[1]);
    KeyGen(/*argv[2],*/ kgBitsArg);

    /* if (argc < 4 || argc > 5) { */
    /*    PrintH(); */
    /* } */

    /* if (strcmp(argv[1], "-kg")){ */
        /* int kgBitsArg = atoi(argv[3]); */
        /* KeyGen(argv[2], kgBitsArg); */
    /* } else if (strcmp(argv[1], "-cr")) { */

    /* } else PrintH(); */
    /* clock_t end = clock(); */
    /*     ts = (double)(end - begin) / CLOCKS_PER_SEC; */
    /*     printf("%f seconds \n", ts); */
    return EXIT_SUCCESS;
}
