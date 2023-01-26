
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
#include <termios.h>

struct timespec startKG;
mpz_t prNumForFunc;
int percents = 0;

void draw_progress_bar( int percents ) {
    int half = percents / 2;
    printf( "\r [" );
    for ( int i = 0; i <    half; ++i ) putc( (char)120, stdout );
    for ( int i = 0; i < 50-half; ++i ) putc( ' ', stdout );
    printf( "] %3d %%", percents );
    fflush( stdout );
}

void PrintH (){
    system ("./printer.sh 1");
    _Exit (0);
}

void clear_icanon(void) // перевод терминала в неканоничный режим
{
  struct termios settings;

  if (tcgetattr (STDIN_FILENO, &settings) < 0) {
      perror ("tcgetattr");
      exit(1);
    }

  settings.c_lflag &= ~ICANON;

  if (tcsetattr (STDIN_FILENO, TCSANOW, &settings) < 0) {
      perror ("tcsetattr");
      exit(1);
   }
}

mpz_t *PrimeNumber(unsigned long bitts){
    clock_gettime(CLOCK_MONOTONIC_RAW, &startKG);
    unsigned long int seeed = (unsigned) startKG.tv_nsec;
    seeed /= 1000;
    /* printf("PrimeNumber seed: %lu\n", seeed); */
percents += 5;
draw_progress_bar(percents);
    gmp_randstate_t randd;
percents += 5;
draw_progress_bar(percents);
    gmp_randinit_mt(randd);
percents += 5;
draw_progress_bar(percents);
    gmp_randseed_ui(randd, seeed);
percents += 5;
draw_progress_bar(percents);
    mpz_urandomb(prNumForFunc, randd, bitts);
percents += 5;
draw_progress_bar(percents);
    mpz_nextprime(prNumForFunc, prNumForFunc);
percents += 5;
draw_progress_bar(percents);
    gmp_randclear(randd);
    return &prNumForFunc;
}

void KeyGen(char *isFile, unsigned long bits){
mpz_init(prNumForFunc);
//генерация p и q
    mpz_t p;
    mpz_init(p);
    mpz_set(p, *PrimeNumber(bits));

    mpz_t pP;
    mpz_init(pP);
    mpz_set(pP, p);
percents += 5;
draw_progress_bar(percents);
    mpz_t q;
    mpz_init(q);
    mpz_set(q, *PrimeNumber(bits));

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
    percents += 5;
    draw_progress_bar(percents);
//выбор открытой экспоненты и поиск d
    mpz_t x, y;
    mpz_inits(x, y, NULL);

    percents += 5;
    draw_progress_bar(percents);
    mpz_t a, a1, a2, b, b1, b2;
    mpz_inits(a, a1, a2, b, b1, b2, NULL);

    mpz_t q1, r, m;
    mpz_inits(q1, r, m, NULL);
    percents += 5;
    draw_progress_bar(percents);
    mpz_t buf;
    mpz_init(buf);

    unsigned long int e = 0;
    while (mpz_cmp_ui(m, 1) != 0){
        e > 0 ? printf("Число не подошло. Выберите другое:\n") : printf("\rПожалуйста, выберите открытую экспоненту (введя число 1-5):\n");
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
            mpz_set_ui(y, e);
            mpz_set(x, eilFuncY);

            mpz_set_ui(a1, 0);
            mpz_set_ui(a2, 1);
            mpz_set_ui(b1, 1);
            mpz_set_ui(b2, 0);

            while (mpz_cmp_ui(y, 0) != 0){
                mpz_div(q1, x, y);
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
    }
    percents += 15;
    draw_progress_bar(percents);
    if (mpz_cmp_ui(b, 0) < 0){
        mpz_add(b, b, eilFuncY);
    }
    percents += 5;
    draw_progress_bar(percents);
    mpz_clear(prNumForFunc);
    mpz_clears(p, q, eilFuncY2, x, y, a, a1, a2, b1, b2, q1, r, m, NULL);

    if (strcmp(isFile, "-c") == 0){
        printf("\r");
        //вывод в консоль
        printf("RSAPublicKey ::= SEQUENCE {\n");
        gmp_printf("            modulus           INTEGER,  -- %Zd\n", modulleN);
        printf("            publicExponent    INTEGER,  -- %lu\n", e);
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
            gmp_fprintf(outFPub, "            modulus           INTEGER,  -- %Zd\n", modulleN);
            fprintf(outFPub, "            publicExponent    INTEGER,  -- %lu\n", e);
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
            printf("\nГотово!\n");
        }

        free(outFileName);
        free(outFileName2);

    }

mpz_clears(pP, qQ, b, modulleN, eilFuncY, NULL);
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
    for (int i = 0; i < 43; i++){ // пропускаем первые 44 символа
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
    /* gmp_printf("MODULUS\n%Zd\n\n", modulus); */
    free(pubKeyS1);
    // чтение открытой экспоненты
    for(int i = 0; i < 43; i++){ // пропускаем 44 символа перед
        getc(keyFile);
    }
    int pubKeyCounter = 0;
    while (pubKeyS2[pubKeyCounter-1] != '\n'){
        pubKeyS2[pubKeyCounter] = fgetc(keyFile);
        pubKeyCounter++;
    }
    // publicExponent только для хранения открытой экпоненты
    unsigned int publicExponent = atoi(pubKeyS2);
    /* printf("PUBLIC EXPONENT\n%d\n\n", publicExponent); */
    free(pubKeyS2);
    fclose(keyFile);
///////// РАБОТА С СООБЩЕНИЕМ//
///////////////////////////////
    // подготовка к вводу сообщения
    char* inp = malloc(sizeof(char[1025]));
    char* outp = malloc(sizeof(char[1024]) * modulusSymbols + 1);
    modulusSymbols--; // подготовили для хранения количества знаков
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
    strcmp(isFile, "-c") == 0 ? printf("Вводите сообщения (максимум 1024 символа).\nКогда захотите выйти, нажмите Ctrl + C\n") : printf("Введите сообщение (максимум 1024 символа)\n");
    while(1){
        messageIterator = 0;
        toOutIterator = 0;
        printf(">> ");
        fgets(inp, 1024, stdin);
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
            printf("<< %s\n", outp);
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
                fprintf(outMesFile, "EncryptedData :: = SEQUENCE {\n");
                fprintf(outMesFile, "%s\n", outp);
                fprintf(outMesFile, "        }\n\n");
                fclose(outMesFile);
            }
            break;
        }
    }
    mpz_clears(modulus, mpzCharBuf, tempMpzCharBuf, tempMpzCharBuf2, NULL);
    free(inp);
    free(outp);
    free(toOutp);
}
void Decryption(char *prKeyPass, char *crMesPass){
    FILE * keyFile;
    if ((keyFile = fopen(prKeyPass, "r")) == NULL){
            perror("fopen: prKeyPass");
            exit(1);
    }
    char *prKeyS1 = malloc(sizeof(char[2500]));
    char *prKeyS2 = malloc(sizeof(char[2500]));



    while (getc(keyFile) != '\n'){ // пропускаем первую строку
    }
    for (int i = 0; i < 43; i++){ // пропускаем первые 43 символа
        getc(keyFile);
    }
    int modulusSymbols = 0;
    while (prKeyS1[modulusSymbols-1] != '\n'){ // считываем модуль
    prKeyS1[modulusSymbols] = fgetc(keyFile);
    modulusSymbols++; // считаем количество знаков у модуля
    }
    // modulus только для хранения модуля
    mpz_t modulus;
    mpz_init(modulus);
    mpz_set_str(modulus, prKeyS1, 10);
    /* gmp_printf("MODULUS\n%Zd\n\n", modulus); */
    free(prKeyS1);

    while (getc(keyFile) != '\n'){ // пропускаем строку
    }
    for (int i = 0; i < 43; i++){ // пропускаем первые 43 символа
        getc(keyFile);
    }
    int prExpSimbs = 0;
    do { // считываем d
    prKeyS2[prExpSimbs] = fgetc(keyFile);
    prExpSimbs++; // считаем количество знаков у d
    } while (prKeyS2[prExpSimbs - 1] != '\n');
    // modulus только для хранения d
    mpz_t prExp;
    mpz_init(prExp);
    mpz_set_str(prExp, prKeyS2, 10);
    free(prKeyS2);
///// получение зашифрованного сообщения//
//////////////////////////////////////////
    // подготовка
    char* inp = malloc(sizeof(char[1024]) * modulusSymbols + 1);
    char* outp = malloc(sizeof(char[1025]));
    int messSimbols = 0;
    mpz_t crMess;
    mpz_init(crMess);
    mpz_t modulusPow10;     // хранение 10 ^ (modulusSymols - 1)
    mpz_init(modulusPow10); //
    mpz_t mpzCharBuf;       // хранение символов в виде mpz
    mpz_init(mpzCharBuf);   //
    mpz_t tempMpzCharBuf;       // временное хранение символов в виде mpz
    mpz_init(tempMpzCharBuf);   //
    int outpIterator;
    int resBuf;
    mpz_t messPow10;
    mpz_init(messPow10);
    // получение
    strcmp(crMesPass, "-c") == 0 ? printf("Вводите шифртекст (Ctrl + C для выхода):\n") : printf("Открываем %s ...\n", crMesPass);
    while (1) {
        messSimbols = 0;
        if (strcmp(crMesPass, "-c") == 0){
            printf(">> ");
            scanf("%s", inp);
            messSimbols = strlen(inp);
        } else {
            FILE * crMesFile;
            if ((crMesFile = fopen(crMesPass, "r")) == NULL){
                /* printf("Opening %s ...\n", crMesPass); */
                perror("fopen: crMesFile");
                exit(1);
            }
            while (getc(crMesFile) != '\n'){ // пропускаем строку
            }
            while ((inp[messSimbols] = getc(crMesFile)) != '\n'){
                messSimbols++;
            }
            fclose(crMesFile);
        }

        mpz_set_str(crMess, inp, 10);
        mpz_ui_pow_ui(modulusPow10, 10, modulusSymbols - 1);
        mpz_ui_pow_ui(messPow10, 10, messSimbols);
        mpz_div(messPow10, messPow10, modulusPow10);
        mpz_set(mpzCharBuf, crMess);
        outpIterator = 0;
        while (mpz_cmp_ui(mpzCharBuf, 0) > 0){
            mpz_div(tempMpzCharBuf, mpzCharBuf, messPow10);
            mpz_mod(mpzCharBuf, mpzCharBuf, messPow10);
            mpz_div(messPow10, messPow10, modulusPow10);

            mpz_powm(tempMpzCharBuf, tempMpzCharBuf, prExp, modulus);
            resBuf = mpz_get_ui(tempMpzCharBuf) + 95;
            outp[outpIterator] = (char)resBuf;
            outpIterator++;
        }
    printf("<< %s\n", outp);
    if(strcmp(crMesPass, "-c") != 0) break;
    }
    free(inp);
    free(outp);
    mpz_clears(modulus, prExp, NULL);
    fclose(keyFile);
}

int main (int argc, char *argv[]) {
    printf("LAB7 by Pavel Isaenko\n");
    /* double ts = 0; */
    /* clock_t begin = clock(); */

    if (argc < 4 || argc > 6) {
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
            }
        } else if (strcmp(argv[2], "-dc") == 0) {
            if (strcmp(argv[4], "-f") == 0 && argc == 6){
                Decryption(argv[3], argv[5]);
            } else if (strcmp(argv[4], "-c") == 0 && argc == 5){
                clear_icanon();
                Decryption(argv[3], "-c");
            } else {
                PrintH();
            }

        } else PrintH();
    } else PrintH();

    /* clock_t end = clock(); */
    /* ts = (double)(end - begin) / CLOCKS_PER_SEC; */
    /* printf("%f seconds \n", ts); */
    return EXIT_SUCCESS;
}




