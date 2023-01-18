
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>
#include <time.h>

struct timespec startKG;
mpz_t prNumForFunc;

void PrintH (){
    system ("./printer.sh 1");
    _Exit (0);
}

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
mpz_init(prNumForFunc);
//генерация p и q
    mpz_t p;
    mpz_init(p);
    mpz_set(p, *PrimeNumber(bits));
    /* gmp_printf("p: %Zd \n", p); */

    mpz_t pP;
    mpz_init(pP);
    mpz_set(pP, p);

    mpz_t q;
    mpz_init(q);
    mpz_set(q, *PrimeNumber(bits));
    /* gmp_printf("q: %Zd \n", q); */

    mpz_t qQ;
    mpz_init(qQ);
    mpz_set(qQ, q);

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
    /* gmp_printf("eilFunc: %Zd\n", eilFuncY); */

    /* mpz_clears(p, q, eilFuncY2); */
//выбор открытой экспоненты и поиск d
    mpz_t ee;
    mpz_init(ee);

    mpz_t x, y;
    mpz_init(x);
    mpz_init(y);
    mpz_set(x, eilFuncY);

    mpz_t a, a1, a2, b, b1, b2;
    mpz_inits(a, a1, a2, b, b1, b2);

    mpz_t q1, r, m;
    mpz_inits(q1, r, m);

    int e = 0;
    while (mpz_cmp_ui(m, 1) != 0){
        e > 0 ? printf("Число не подошло. Выберите другое:\n") : printf("Пожалуйста, выберите открытую экспоненту (введя число 1-5):\n");
        system("./printer.sh 2");
        scanf("%d", &e);

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
            //Расширенный алг Евклида
            /* gmp_printf("\ntry %Zd %Zd\n", eilFuncY, ee); // переделать без использования mpz_gcd */
            /* mpz_cmp(ee, eilFuncY) > 0 ? mpz_gcd(ee, eilFuncY, ee) : mpz_gcd(ee, ee, eilFuncY); */

            mpz_set(y, ee);

            mpz_set_ui(a1, 0);
            mpz_set_ui(a2, 1);
            mpz_set_ui(b1, 1);
            mpz_set_ui(b2, 0);

            while (mpz_cmp_ui(y, 0) != 0){
                mpz_div(q1, x, y);
                /* mpz_cmp(x, y) > 0 ? mpz_div(q1, x, y) : mpz_div(q1, y, x); */
                mpz_submul(x, q1, y);
                mpz_set(r, x);
                mpz_submul(a2, q1, a1);
                mpz_set(a, a2);
                mpz_submul(b2, q1, b1);
                mpz_set(b, b2);

                mpz_set(x, y);
                mpz_set(y, r);
                mpz_set(a2, a1);
                mpz_set(a1, a);
                mpz_set(b2, b1);
                mpz_set(b1, b);
            }
            mpz_set(m, x);
            mpz_set(a, a2);
            mpz_set(b, b2);


            /* gmp_printf("y = %Zd\nm = %Zd\na = %Zd\nb = %Zd\n", y, m, a, b); */
    }
mpz_clear(prNumForFunc);
mpz_clears(p, q, eilFuncY2, x, y, a, a1, a2, b, b1, b2, q1, r, m);
printf("RSAPublicKey ::= SEQUENCE {\n");
gmp_printf("            modulus            INTEGER,  -- %Zd\n", modulleN);
gmp_printf("            publicExponent     INTEGER,  -- %Zd\n", ee);
printf("        }\n\n");

printf("RSAPrivateKey ::= SEQUENCE {\n");
gmp_printf("            modulus           INTEGER,  -- %Zd\n", modulleN);
gmp_printf("            publicExponent    INTEGER,  -- %Zd\n", ee);//ee сделать int а не mpz_t
gmp_printf("            privateExponent   INTEGER,  -- %Zd\n", b);
gmp_printf("            prime1            INTEGER,  -- %Zd\n", pP);
gmp_printf("            prime2            INTEGER,  -- %Zd\n", qQ);
printf("        }\n\n");


mpz_clears(pP, qQ, modulleN, eilFuncY, ee);
}

int main (int argc, char *argv[]) {

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



    clock_t end = clock();
    ts = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f seconds \n", ts);
    return EXIT_SUCCESS;
}

