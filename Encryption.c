void Encryption(char *pubKeyPass){
    FILE * keyFile;
    keyFile = fopen(pubKeyPass, "r");
    mpz_t modulus;
    mpz_init(modulus);
    char *pubKeyS1 = malloc(sizeof(char[2500]));
    char *pubKeyS2 = malloc(sizeof(char[6]));
    //чтение файла с ключом
    while (getc(keyFile) != '\n'){
    }
    for (int z = 0; z < 44; z++){
        getc(keyFile);
    }
    int i = 0;
    while (pubKeyS1[i-1] != '\n'){
    pubKeyS1[i] = fgetc(keyFile);
    i++;
    }
    mpz_set_str(modulus, pubKeyS1, 10);
    gmp_printf("MODULUS\n%Zd\n", modulus);
    free(pubKeyS1);

    for (int z = 0; z < 44; z++){
        getc(keyFile);
    }

    fscanf(keyFile, "%s", pubKeyS2);
    unsigned int publicExponent = atoi(pubKeyS2);
    printf("\nPUBLIC_EXPONENT\n%d\n", publicExponent);

    mpz_t bigBuff;
    mpz_init(bigBuff);

    int nums = 0;

    mpz_set(bigBuff, modulus);
    while (mpz_cmp_ui(bigBuff, 0) > 0){
        mpz_div_ui(bigBuff, bigBuff, 10);
        nums++;
    }

    //ввод сообщения
    printf("Введите сообщение(максимум 1024 символов).\nКогда захотите выйти, нажмите Ctrl + C\n");
    char* inp = malloc(sizeof(char[1025]));
    char* outp = malloc(sizeof(char[1024])*nums+1);
    i = 0;
    unsigned int buf;
    mpz_t bigBuf;
    mpz_init(bigBuf);


    printf("\n%d\n", nums);

    int a = 3;
    while(a > 0){
        a --;
        scanf("%s", inp);
        while (inp[i] != '\0'){
            buf = (int)tolower((int)inp[i]);
printf("%c %u ", tolower((int)inp[i]), buf);
            buf -= 96;

            mpz_set_ui(bigBuf, buf);

            mpz_pow_ui(bigBuf, bigBuf, publicExponent);

            mpz_mod(bigBuf, bigBuf, modulus);

            int nnums = 0;
            mpz_t buff;
            mpz_init(buff);
            mpz_set(buff, bigBuf);
            while (mpz_cmp_ui(buff, 0) > 0){
                mpz_div_ui(buff, buff, 10);
                nnums++;
            }//длина числа представляющего зашифрованную букву
            printf("%d", nnums);

gmp_printf("\n%Zd\n", bigBuf);
            for (int j = 0; j < (nums - nnums); j++){
                outp[(i * nums) + j] = '0';
            }
printf("%s\n", outp);
            mpz_t numb;
            mpz_init(numb);
            mpz_set_ui(numb, 1);
            for (int j = 0; j < nnums - 1; j++){
                mpz_mul_ui(numb, numb, 10);
            }// подготовка числа для разделения числа на отдельные цифры
            gmp_printf("numb %Zd\n", numb);
            mpz_t numb2;
            mpz_init(numb2);
            mpz_div(numb2, bigBuf, numb);
            gmp_printf("numb2 %Zd\n", numb2);
            char *toOutp = malloc(sizeof(char[2]));
            mpz_get_str(toOutp, 1, numb2);
            outp[nums-nnums] = toOutp[0];
            /* mpz_div_ui(numb, numb, 10); */
printf("%s\n", outp);
            for (int j = nums - nnums;j < nnums; j++){
                mpz_mod(numb2, bigBuf, numb);
/* gmp_printf("numb2_mod: %Zd", numb2); */
                mpz_div_ui(numb, numb, 10);
                mpz_div(numb2, numb2, numb);
                gmp_printf("numb2 %Zd\n", numb2);
                mpz_get_str(toOutp, 1, numb2);
                outp[j] = toOutp[0];
            }


            /* outp[i*3] = buf / 100; */

            /* outp[(i*3)+1] = buf % 100 / 10; */

            /* outp[(i*3)+2] = buf % 10; */
            i++;
        /* printf("\n"); */
        }
        printf("outp: %s\n", outp);
        /* fflush(stdout); */

    }
    mpz_clear(bigBuf);
    free(inp);
    free(outp);
})

