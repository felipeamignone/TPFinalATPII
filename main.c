#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SNAME 20
#define SCODE 4 //dd/n (note is the last information in line of classes file)
#define SNOTE 7 //dd.dd/n (note is the last information in line of reports file)
#define SLINE_CLASS 24 // SNAME + SCODE
#define SLINE_REPORT 31 // SNAME + SCODE + SNOTE

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
    printf("\n\nReading classes file ...");
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

        //now we assign the code, name and next pointer to the struct pointer.
        strcpy(classes[amount].code, code);
        strcpy(classes[amount].name, name);
        classes[amount].nextClass = &classes[amount + 1];
        amount++;
    }

    //We get classes amount in function parameter to use it on main.
    if (amount) {
        *amountClasses = amount;
    }
    return 0;
}

int readReport(struct ReportType *reports, int *amountReports) {
    printf("\n\nReading reports file ...");
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

int assignClassesName(struct ReportType *reports, struct ClassType *classes, int amountReports, int amountClasses) {
    int i, j;
    for (i = 0; i <= amountReports; i++) {
        //index of name founded on linear search
        int nameIndex = -1;

        //find index of class with the same code that parameter with linear search
        for (j = 0; j <= amountClasses; j++) {
            int compareResult = strcmp(reports[i].note.classCode, classes[j].code);
            // strcmp returns 0 if strings are equals
            if (compareResult == 0) {
                printf("\nclassCode %s, classes[j].code: %s", reports[i].note.classCode, classes[j].code);
                printf("class name found: %s", classes[j].name);
                nameIndex = j;
                break;
            }
        }

        //assign the name in struct reports, if founded.
        if (nameIndex != -1) {
            strcpy(reports[i].note.className, classes[nameIndex].name);
        } else {
            strcpy(reports[i].note.className, "WithoutRegister");
        }
    }

    return 0;
}

int main(void) {
    //initializing counters
    int initializerAmountClasses = 0, initializerAmountReports = 0, k;
    int *amountClasses = &initializerAmountClasses;
    int *amountReports = &initializerAmountReports;

    //initialize arrays
    struct ClassType classes[10] = {};
    struct ReportType reports[20] = {};

    //call functions to read files
    readClasses(classes, amountClasses);
    readReport(reports, amountReports);

    //printing classes
    printf("\nClasses:\n");
    for (k = 0; k < *amountClasses; k++) {
        printf("Codigo: %s - Disciplina: %s\n", classes[k].code, classes[k].name);
    }

    //printing reports
    printf("\nReports:\n");
    for (k = 0; k < *amountReports; k++) {
        printf("Aluno: %20s - Disciplina: %s - Nota: %s\n", reports[k].studentName, reports[k].note.classCode,
               reports[k].note.value);
    }

    //assign classes names into struct reports
    assignClassesName(reports, classes, *amountReports, *amountClasses);

    printf("\nReports with names:\n");
    for (k = 0; k < *amountReports; k++) {
        printf("Aluno: %s - Disciplina: %s Codigo: %s - Nota: %s\n", reports[k].studentName, reports[k].note.className,
               reports[k].note.classCode,
               reports[k].note.value);
    }
}

