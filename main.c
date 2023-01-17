
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __linux__
    #include <gmp.h>
#endif
#ifdef __MACH__
    #include "/opt/homebrew/Cellar/gmp/6.2.1_1/include/gmp.h"
#endif




void PrintH (){
    system ("../helpprint.sh");
    _Exit (0);
}

mpz_t *PrimeNumber(){
    mpz_t *res;
    mpz_t a;

    gmp_randstate_t randd;
    

    res = &a;
    return res;
}

void KeyGen(char *isFile, int bits){



}

int main (int argc, char *argv[]) {

    if (argc < 4 || argc > 5) {
       PrintH();
    }

    if (strcmp(argv[1], "-kg")){
        int kgBitsArg = atoi(argv[3]);
        KeyGen(argv[2], kgBitsArg);
    } else if (strcmp(argv[1], "-cr")) {

    } else PrintH();

    return EXIT_SUCCESS;
}
