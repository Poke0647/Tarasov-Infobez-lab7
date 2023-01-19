
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>
#include <time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>

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

void KeyGen(char *isFile, unsigned long bits){
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

    mpz_t x, y;
    mpz_inits(x, y, NULL);
    mpz_set(x, eilFuncY);

    mpz_t a, a1, a2, b, b1, b2;
    mpz_inits(a, a1, a2, b, b1, b2, NULL);

    mpz_t q1, r, m;
    mpz_inits(q1, r, m, NULL);

    unsigned long int e = 0;
    while (mpz_cmp_ui(m, 1) != 0){
        e > 0 ? printf("Число не подошло. Выберите другое:\n") : printf("Пожалуйста, выберите открытую экспоненту (введя число 1-5):\n");
        system("./printer.sh 2");
        scanf("%lu", &e);

        switch (e){
            case 1:
                e = 3;
                break;
            case 2:
                e = 5;
                break;
            case 3:
                e = 17;
                break;
            case 4:
                e = 257;
                break;
            case 5:
                e = 65537;
                break;
        }
            //Расширенный алг Евклида
            /* gmp_printf("\ntry %Zd %Zd\n", eilFuncY, ee); // переделать без использования mpz_gcd */
            /* mpz_cmp(ee, eilFuncY) > 0 ? mpz_gcd(ee, eilFuncY, ee) : mpz_gcd(ee, ee, eilFuncY); */

            mpz_set_ui(y, e);

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
    mpz_clears(p, q, eilFuncY2, x, y, a, a1, a2, b, b1, b2, q1, r, m, NULL);

    if (strcmp(isFile, "-c") == 0){
        //вывод в консоль
        printf("RSAPublicKey ::= SEQUENCE {\n");
        gmp_printf("            modulus            INTEGER,  -- %Zd\n", modulleN);
        printf("            publicExponent     INTEGER,  -- %lu\n", e);
        printf("        }\n\n");

        printf("RSAPrivateKey ::= SEQUENCE {\n");
        gmp_printf("            modulus           INTEGER,  -- %Zd\n", modulleN);
        printf("            publicExponent    INTEGER,  -- %lu\n", e);
        gmp_printf("            privateExponent   INTEGER,  -- %Zd\n", b);
        gmp_printf("            prime1            INTEGER,  -- %Zd\n", pP);
        gmp_printf("            prime2            INTEGER,  -- %Zd\n", qQ);
        printf("        }\n\n");

    } else {
        //вывод в файл
        int outFileNum = 0;
        char *outFileName = malloc(sizeof(char[21]));
        char *outFileName2 = malloc(sizeof(char[22]));

        sprintf(outFileName, "RSAPublicKey.txt");
        while (access(outFileName, F_OK) == 0){
            outFileNum++;
            sprintf(outFileName, "RSAPublicKey%d.txt", outFileNum);
            if (outFileNum > 1000) {
                perror("File creation");
                _exit(0);
            }
        }

        if (outFileNum == 0) {
            sprintf(outFileName2, "RSAPrivateKey.txt");
        } else {
            sprintf(outFileName2, "RSAPrivateKey%d.txt", outFileNum);
        }

        if (access(outFileName2, F_OK) == 0){
            printf("Удалите или переместите в другой каталог файл %s\n", outFileName);
        } else {
            FILE *outFPub;
            outFPub = fopen(outFileName, "w");
            fprintf(outFPub, "RSAPublicKey ::= SEQUENCE {\n");
            gmp_fprintf(outFPub, "            modulus            INTEGER,  -- %Zd\n", modulleN);
            fprintf(outFPub, "            publicExponent     INTEGER,  -- %lu\n", e);
            fprintf(outFPub, "        }\n\n");
            fclose(outFPub);

            FILE *outFPr;
            outFPr = fopen(outFileName2, "w");
            fprintf(outFPr, "RSAPrivateKey ::= SEQUENCE {\n");
            gmp_fprintf(outFPr, "            modulus           INTEGER,  -- %Zd\n", modulleN);
            fprintf(outFPr, "            publicExponent    INTEGER,  -- %lu\n", e);
            gmp_fprintf(outFPr, "            privateExponent   INTEGER,  -- %Zd\n", b);
            gmp_fprintf(outFPr, "            prime1            INTEGER,  -- %Zd\n", pP);
            gmp_fprintf(outFPr, "            prime2            INTEGER,  -- %Zd\n", qQ);
            fprintf(outFPr, "        }\n\n");
            fclose(outFPr);
            printf("Готово!\n");
        }

        free(outFileName);
        free(outFileName2);

    }

mpz_clears(pP, qQ, modulleN, eilFuncY, NULL);
}

int main (int argc, char *argv[]) {
    printf("LAB7 by Pavel Isaenko\n");
    double ts = 0;
    clock_t begin = clock();

    if (argc < 4 || argc > 5) {
       PrintH();
    }

    if (strcmp(argv[1], "-kg") == 0){
        unsigned long kgBitsArg = atoi(argv[3]);
        KeyGen(argv[2], kgBitsArg);
    } else if (strcmp(argv[1], "-cr") == 0) {

        printf("Готовится\n");
    } else PrintH();

    clock_t end = clock();
    ts = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f seconds \n", ts);
    return EXIT_SUCCESS;
}
