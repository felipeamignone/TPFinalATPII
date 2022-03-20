#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SNAME 20
#define SCODE 2
#define SLINE 22 // SNAME + SCODE

struct ClassType {
    char code[SCODE];
    char name[SNAME];
    struct ClassType *nextClass;
};

//call classes by reference and push lines of txt on that.
int readClasses(struct ClassType *classes, int *amountClasses) {
    FILE *file = NULL;
    char fileDir[200], fileLine[SLINE];
    int continueOrExit = 0, amount = 0, i, j;

    //gets fileName by user
    printf("File's directory to be loaded: ");
    scanf("%s", fileDir);

    //read the file
    file = fopen(fileDir, "r");

    //checks if file exist
    if (file == NULL) {
        printf("\nFile not found!");
        printf("\nTo change file's directory press 1 and enter, to exit 0: ");
        scanf("%d", &continueOrExit);
        if (continueOrExit) {
            //recursion to read a new directory
            printf("\n\n");
            readClasses(classes, amountClasses);
        } else {
            //exit function
            return 0;
        }
    }

    //looping while fgets doesn't return an empty line.
    while (fgets(fileLine, SLINE, file)) {
        i = 0;
        j = 0;
        char code[SCODE] = {}, name[SNAME] = {};

        //catch the chars in fileLine and push it on string "code", the conditions is ' ' because it's the separator
        //of classes codes and names in file.
        while (fileLine[i] != ' ') {
            code[i] = fileLine[i];
            i++;
        }

        //here we use the j to control string "name" index, and i to control fileLine. fgets return \n on end of line
        //and \0 on end of file.
        while (fileLine[i] != '\0' && fileLine[i] != '\n') {
            name[j] = fileLine[i];
            i++;
            j++;
        }

        //now we assign the code, name and next pointer to the struct.
        strcpy(classes[amount].code, code);
        strcpy(classes[amount].name, name);
        classes[amount].nextClass = &classes[amount + 1];
        amount++;
    }

    //We get amount classes in function parameter to use it on main.
    if (amount) {
        *amountClasses = amount;
    }
    return 0;
}

int main(void) {
    int initializerAmount = 0;
    int *amountClasses = &initializerAmount;
    struct ClassType classes[10] = {};
    readClasses(classes, amountClasses);

    printf("\n%d", *amountClasses);
    printf("\n\n");
    for (int k = 0; k < *amountClasses; k++) {
        printf("Codigo: %s - Disciplina: %s\n", classes[k].code, classes[k].name);
    }
}

