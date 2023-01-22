
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>
#include <time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

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

void Encryption(char *pubKeyPass, char *isFile){
    FILE * keyFile;
    if ((keyFile = fopen(pubKeyPass, "r")) == NULL) {
        perror("fopen: keyFile");
        exit(1);
    }
    char *pubKeyS1 = malloc(sizeof(char[2500])); // сторка с модулем
    char *pubKeyS2 = malloc(sizeof(char[6])); //  строка с экспонентой
//////// РАБОТА С КЛЮЧОМ//
//////////////////////////
    // чтение модуля
    while (getc(keyFile) != '\n'){ // пропускаем первую строку
    }
    for (int i = 0; i < 44; i++){ // пропускаем первые 44 символа
        getc(keyFile);
    }
    int modulusSymbols = 0;
    while (pubKeyS1[modulusSymbols-1] != '\n'){ // считываем модуль
    pubKeyS1[modulusSymbols] = fgetc(keyFile);
    modulusSymbols++; // считаем количество знаков у модуля
    }
    // modulus только для хранения модуля
    mpz_t modulus;
    mpz_init(modulus);
    mpz_set_str(modulus, pubKeyS1, 10);
    gmp_printf("MODULUS\n%Zd\n\n", modulus);
    free(pubKeyS1);
    // чтение открытой экспоненты
    for(int i = 0; i < 44; i++){ // пропускаем 44 символа перед
        getc(keyFile);
    }
    int pubKeyCounter = 0;
    while (pubKeyS2[pubKeyCounter-1] != '\n'){
        pubKeyS2[pubKeyCounter] = fgetc(keyFile);
        pubKeyCounter++;
    }
    // publicExponent только для хранения открытой экпоненты
    unsigned int publicExponent = atoi(pubKeyS2);
    printf("PUBLIC EXPONENT\n%d\n\n", publicExponent);
    free(pubKeyS2);
    fclose(keyFile);
///////// РАБОТА С СООБЩЕНИЕМ//
///////////////////////////////
    // подготовка к вводу сообщения
    char* inp = malloc(sizeof(char[1025]));
    char* outp = malloc(sizeof(char[1024]) * modulusSymbols + 1);
    modulusSymbols--; // подготовили для хранения количества знаков
    printf("modulusSymbols %d\n", modulusSymbols);
    int messageIterator = 0;
    int intCharBuf; // хранение символов в виде int - 96
    mpz_t mpzCharBuf;       // хранение символов в виде mpz
    mpz_init(mpzCharBuf);   //
    int charLength; // длина зашифрованного символа в формате mpz
    mpz_t tempMpzCharBuf;       // временное хранение символов в виде mpz
    mpz_init(tempMpzCharBuf);   //
    mpz_t tempMpzCharBuf2;      // временное хранение символа в виде mpz пред отправкой к toOutp
    mpz_init(tempMpzCharBuf2);  //
    char *toOutp = malloc(sizeof(char[2])); // хранение символа цифры перед отправкой в вывод
    int toOutIterator;
    int toOutInt;
    // ввод сообщения
    printf("Введите сообщение(максимум 1024 символа).\nКогда захотите выйти, нажмите Ctrl + C\n");
    scanf("%s", inp);
    while (inp[messageIterator] != '\0'){
        intCharBuf = tolower((int)inp[messageIterator]) - 95; // приводим все вводимые симолы в нижний регистр и отнимаем у их значения 96 для удобства представления */
        mpz_set_ui(mpzCharBuf, intCharBuf); // переводим символ int -> mpz для дальнейшей работы с большими числами
        mpz_pow_ui(mpzCharBuf, mpzCharBuf, publicExponent); // c = m^e mod n
        mpz_mod(mpzCharBuf, mpzCharBuf, modulus);           //
        //вычисление длины числа, полученного после шифрования буквы
        mpz_set(tempMpzCharBuf, mpzCharBuf);
        charLength = 0;
        while (mpz_cmp_ui(tempMpzCharBuf, 0) > 0){
            mpz_div_ui(tempMpzCharBuf, tempMpzCharBuf, 10);
            charLength++;
        }
        toOutIterator = 0;
        //запись нулей перед числом, более коротким чем модуль
        for (int j = 0; j < (modulusSymbols - charLength); j++){
            outp[(messageIterator * modulusSymbols) + toOutIterator]='0';
            toOutIterator++;
        }
        mpz_set_ui(tempMpzCharBuf, 1);
        for (int j = 0; j < charLength - 1; j++){
            mpz_mul_ui(tempMpzCharBuf, tempMpzCharBuf, 10);
        }
        for (int i = modulusSymbols - charLength; i < modulusSymbols; i++){
            if (mpz_cmp_ui(mpzCharBuf, 1) == 0){
                toOutInt = 1;
            } else {
                mpz_div(tempMpzCharBuf2, mpzCharBuf, tempMpzCharBuf);
                toOutInt = mpz_get_ui(tempMpzCharBuf2);
            }
            mpz_mod(mpzCharBuf, mpzCharBuf, tempMpzCharBuf);
            mpz_div_ui(tempMpzCharBuf, tempMpzCharBuf, 10);
            sprintf(toOutp, "%d", toOutInt);
            outp[messageIterator * modulusSymbols + toOutIterator] = toOutp[0];
            toOutIterator++;
        }
        messageIterator++;
    }
    if (strcmp(isFile, "-c") == 0){
        printf("outp %s\n", outp);
    } else {
        int outFileNum = 0;
        char *outFileName = malloc(sizeof(char[21]));
        char *outFileName2 = malloc(sizeof(char[22]));

        sprintf(outFileName, "OutMessage.txt");
        while (access(outFileName, F_OK) == 0){
            outFileNum++;
            sprintf(outFileName, "OutMessage%d.txt", outFileNum);
            if (outFileNum > 1000) {
                perror("File creation");
                _exit(0);
            }
        }
        if (outFileNum == 0) {
            sprintf(outFileName2, "OutMessage.txt");
        } else {
            sprintf(outFileName2, "OutMessage%d.txt", outFileNum);
        }

        if (access(outFileName2, F_OK) == 0){
            printf("Удалите или переместите в другой каталог файл %s\n", outFileName);
        } else {
            FILE *outMesFile;
            outMesFile = fopen(outFileName, "w");
            fprintf(outMesFile, "%s", outp);
            fclose(outMesFile);
        }
    }

    mpz_clears(modulus, mpzCharBuf, tempMpzCharBuf, tempMpzCharBuf2, NULL);
    free(inp);
    free(outp);
    free(toOutp);
}
void Decryption(char *prKeyPass, char *crMesPass){
    mpz_t message;
    mpz_init(message);

    gmp_scanf("%Zd", message);
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
        if (strcmp(argv[2], "-ec") == 0){
            if (argc == 5){
                Encryption(argv[3], argv[4]);
            } else {
                PrintH();
                exit(0);
            }
        } else if (strcmp(argv[2], "-dc") == 0) {
            if (strcmp(argv[4], "-f") && argc == 6){
                Decryption(argv[3], argv[5]);
            } else if (strcmp(argv[4], "-c") && argc == 5){
                Decryption(argv[3], NULL);
            } else {
                PrintH();
                exit(0);
            }

        } else PrintH();
        printf("Готовится\n");
    } else PrintH();

    clock_t end = clock();
    ts = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f seconds \n", ts);
    return EXIT_SUCCESS;
}
