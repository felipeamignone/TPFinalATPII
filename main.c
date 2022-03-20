#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SNAME 20
#define SCODE 2
#define SNOTE 7 //dd.dd/n (note is the last information in line)
#define SLINE_CLASS 22 // SNAME + SCODE
#define SLINE_REPORT 28 // SNAME + SCODE + SNOTE

struct ClassType {
    char code[SCODE];
    char name[SNAME];
    struct ClassType *nextClass;
};

struct Note {
    char classCode[SCODE];
    char className[SNAME];
    char value[SNOTE];
};

struct ReportType {
    char studentName[SNAME];
    struct Note note;
    struct ReportType *nextReport;
};

//interface to load a file
FILE *readFile() {
    FILE *file = NULL;
    char fileDir[200];
    int continueOrExit = 0;

    //gets fileName by user
    printf("\nInput file's directory to be loaded: ");
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
            return readFile();
        } else {
            //exit function
            printf("\n\nWasn't possible read the file!");
            return NULL;
        }
    }
    return file;
}

//call reports by reference and push lines of txt on that.
int readClasses(struct ClassType *classes, int *amountClasses) {
    printf("\nReading classes file ...");
    FILE *file = readFile();
    char fileLine[SLINE_CLASS];
    int amount = 0, i, j;

    //looping while fgets doesn't return an empty line.
    while (fgets(fileLine, SLINE_CLASS, file)) {
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

int readReport(struct ReportType *reports, int *amountReports) {
    printf("\nReading reports file ...");
    FILE *file = readFile();
    char fileLine[SLINE_REPORT];
    int amount = 0, i, j;

    //looping while fgets doesn't return an empty line.
    while (fgets(fileLine, SLINE_REPORT, file)) {
        i = 0;
        j = 0;
        char studentName[SNAME] = {}, classCode[SCODE] = {}, note[SNOTE] = {};

        //get student's name in line
        while (fileLine[i] != ' ') {
            studentName[i] = fileLine[i];
            i++;
        }
        i++;
        //get class code
        while (fileLine[i] != ' ') {
            classCode[j] = fileLine[i];
            i++;
            j++;
        }
        j = 0;

        //get note
        while (fileLine[i] != '\0' && fileLine[i] != '\n') {
            note[j] = fileLine[i];
            i++;
            j++;
        }

        //assign values on pointer
        strcpy(reports[amount].studentName, studentName);
        strcpy(reports[amount].note.classCode, classCode);
        strcpy(reports[amount].note.value, note);
        reports[amount].nextReport = &reports[amount + 1];
        amount++;
    }

    //get reports amount to use it on main.
    if (amount) {
        *amountReports = amount;
    }
    return 0;

}

int main(void) {
    int initializerAmountClasses = 0,initializerAmountReports = 0, k;
    int *amountClasses = &initializerAmountClasses;
    int *amountReports = &initializerAmountReports;
    struct ClassType classes[10] = {};
    struct ReportType reports[20] = {};
    readClasses(classes, amountClasses);
    readReport(reports, amountReports);

    printf("\nClasses:\n");
    for (k = 0; k < *amountClasses; k++) {
        printf("Codigo: %s - Disciplina: %s\n", classes[k].code, classes[k].name);
    }

    printf("\nReports:\n");
    for (k = 0; k < *amountReports; k++) {
        printf("Aluno: %s - Disciplina: %s - Nota: %s\n", reports[k].studentName, reports[k].note.classCode, reports[k].note.value);
    }
}

