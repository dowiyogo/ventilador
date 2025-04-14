#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void remove_spaces(char* s) {
    const char* d = s;
    while (*s++ = *d++) while (*d == ' ') ++d;
}

int main() {
    const int NPARAMS = 23;
    const char *sep = ",";
    char *devnull;
    char   buff_str[255];
    double buff_darr[NPARAMS];

    // open file.
    FILE *fp;
    fp = fopen("data/continuo.txt", "r");
    if (fp == NULL) {
        perror("Error opening file");
        return(1);
    }

    // process file.
    while(fgets(buff_str, 255, fp)) {
        // clean line.
        remove_spaces(buff_str);

        // split line.
        char *tkn;
        tkn = strtok(buff_str, sep);

        // iterate through line.
        int word_cntr = 0;
        while (tkn) {
            // remove lines with malformed data (more than one dot).
            int dot_cntr = 0;
            for (int tkn_i = 0; tkn_i < strlen(tkn); ++tkn_i) if (tkn[tkn_i] == '.') ++dot_cntr;

            // store double in arr.
            sscanf(tkn, "%lf", &buff_darr[word_cntr]);
            word_cntr++;
            if (word_cntr > NPARAMS || dot_cntr > 1) { // Abandon line if data is malformed.
                word_cntr = -1;
                break;
            }
            tkn = strtok(NULL, sep);
        }
        if (word_cntr != NPARAMS) continue; // Skip lines with wrong number of parameters.

        // NOTE: Process buff_darr here!
        // for (int i = 0; i < NPARAMS; ++i) {
        //     printf("%f ", buff_darr[i]);
        // }
        // printf("\n");
    }
    fclose(fp);

    return(0);
}
