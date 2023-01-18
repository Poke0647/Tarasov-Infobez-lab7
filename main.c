
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>
#include <time.h>
#include <sys/wait.h>
struct timespec startKG;
mpz_t prNumForFunc;

void PrintH (){
    system ("./printer.sh 1");
    _Exit (0);
}

/* def findModInverse(a, m):
    while a != 0:
      a = b % a
      b = a

    if b != 1: */
/*       return -1; */
/*    u1 = 1;
      u2 = 0;
      u3 = a;

    v1 = 0
    v2 = 1
    v3 = m */

/*    while v3 != 0: */
/*       q = u3 // v3 */
/*          v1, v2, v3, u1, u2, u3 = (u1 - q * v1), (u2 - q * v2), (u3 - q * v3), v1, v2, v3 */
/*    return u1 % m */

/* int IsModInv(mpz_t a, mpz_t m){ */
/* //НОД(a, m) */
/*     while(mpz_cmp_ui(a, 0) != 0){ */
/*         mpz_mod(a, a, m); */
/*         mpz_set(m, a); */
/*     } */
/*     if (mpz_cmp_ui(m, 1) != 0){ */
/*         return -1; */
/*     } else { */
/*         return 0; */
/*     } */
/* } */

mpz_t *PrimeNumber(unsigned long bitts){
    clock_gettime(CLOCK_MONOTONIC_RAW, &startKG);
    unsigned long int seeed = (unsigned) startKG.tv_nsec;
    seeed /= 1000;

    printf("PrimeNumber seed: %lu\n", seeed);

    gmp_randstate_t randd;
    gmp_randinit_mt(randd);
    gmp_randseed_ui(randd, seeed);

    mpz_urandomb(prNumForFunc, randd, bitts);
    mpz_nextprime(prNumForFunc, prNumForFunc);
    gmp_randclear(randd);
    return &prNumForFunc;
}

void KeyGen(/*char *isFile,*/ unsigned long bits){
//генерация p и q
    mpz_t p;
    mpz_init(p);
    mpz_set(p, *PrimeNumber(bits));
    gmp_printf("p: %Zd \n", p);

    mpz_t q;
    mpz_init(q);
    mpz_set(q, *PrimeNumber(bits));
    gmp_printf("q: %Zd \n", q);
    //вычисление модуля
    mpz_t modulleN;
    mpz_init(modulleN);
    mpz_mul(modulleN, p, q);
    //вычисление функции Эйлера
    mpz_t eilFuncY;
    mpz_t eilFuncY2;
    mpz_init(eilFuncY);
    mpz_init(eilFuncY2);
    mpz_sub_ui(eilFuncY, p, 1);
    mpz_sub_ui(eilFuncY2, q, 1);
    mpz_mul(eilFuncY, eilFuncY, eilFuncY2);
    gmp_printf("eilFunc: %Zd\n", eilFuncY);

    /* mpz_clears(p, q, eilFuncY2); */

    int e;
    system("./printer.sh 2");


    scanf("%d\n", &e);

    mpz_t ee;
    mpz_init(ee);

    switch (e){
        case 1:
            mpz_set_ui(ee, 3);
            break;
        case 2:
            mpz_set_ui(ee, 5);
            break;
        case 3:
            mpz_set_ui(ee, 17);
            break;
        case 4:
            mpz_set_ui(ee, 257);
            break;
        case 5:
            mpz_set_ui(ee, 65537);
            break;
    }


    printf("dada %d\n", mpz_cmp_ui(ee, 0));

    /* int div; */
    /* if (n1 == n2) */
    /*     return n1; */
    /* int d = n1 - n2; */
    /* if (d < 0){ */
    /*     d = -d; */
    /*     div = NOD(n1, d); */
    /* } else{ */
    /*     div = NOD(n2, d); */
    /* } */
    /* return div; */

    mpz_t d;
    mpz_init(d);
    while (mpz_cmp_ui(ee, 0) != 0){
    	if (mpz_cmp(ee, eilFuncY) == 0){
    		mpz_set_ui(ee, 1);
		break;
    	}
	
    	mpz_sub(d, ee, eilFuncY);
    	if (mpz_cmp_ui(d, 0) < 0){
		mpz_neg(d, d);
		mpz_set(eilFuncY, d);	
   	 } else if (mpz_cmp_ui(d, 0) > 0){
		 mpz_set(ee, d);
	 }
	gmp_printf("tryy %Zd %Zd %Zd\n", ee, eilFuncY, d);
    }


    /* while(mpz_cmp_ui(ee, 0) != 0){ */
    /*         mpz_mod(ee, eilFuncY, ee); */
    /*         mpz_set(eilFuncY, ee); */
    /*         gmp_printf("try %Zd %Zd\n", eilFuncY, ee); */
    /*     } */
      //  gmp_printf("try %Zd %Zd\n", eilFuncY, ee);
//        mpz_gcd(ee, eilFuncY, ee);
      //  gmp_printf("try %Zd %Zd\n", eilFuncY, ee);
        if (mpz_cmp_ui(ee, 1) != 0){
            e = 1;
        } else {
            e = 0;
        }

    printf("%u\n", e);
    /* mpz_clears(modulleN, eilFuncY, ee); */
}

int main (int argc, char *argv[]) {
    mpz_init(prNumForFunc);
    double ts = 0;
    clock_t begin = clock();
    unsigned long kgBitsArg = atoi(argv[1]);
    KeyGen(/*argv[2],*/ kgBitsArg);

    /* if (argc < 4 || argc > 5) { */
    /*    PrintH(); */
    /* } */

    /* if (strcmp(argv[1], "-kg")){ */
        /* int kgBitsArg = atoi(argv[3]); */
        /* KeyGen(argv[2], kgBitsArg); */
    /* } else if (strcmp(argv[1], "-cr")) { */

    /* } else PrintH(); */

    mpz_clear(prNumForFunc);

    clock_t end = clock();
    ts = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f seconds \n", ts);
    return EXIT_SUCCESS;
}

