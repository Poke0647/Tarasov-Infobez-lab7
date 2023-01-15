
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

void PrintH (){
    system ("./helpprint.sh");
    _Exit (0);
}

void KeyGen(char * isFile, int bits){

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
