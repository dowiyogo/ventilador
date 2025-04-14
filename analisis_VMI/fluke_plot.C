#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TNtuple.h"
#include "TFile.h"

void remove_spaces(char* s) {
    const char* d = s;
    while (*s++ = *d++) while (*d == ' ') ++d;
}

int main() {
    const int NPARAMS = 23;
    const char* SEP = ",";
    const char* PARAMS = "high_flow:pressure_HF:volume_HF:V_ti:V_te:V_i:V_o:" \
                         "flujo_neto:volumen_paciente:volumen_tidal:presion_paciente:" \
                         "presion_via_aerea:peep:frecuencia_respiratoria:Vp1_set:u_vf1:" \
                         "y_sf1:y_sf2:Sf1_old:Sf2_old:volumen_esp:volumen_espirado:" \
                         "presion_maxima";
    char*  devnull;
    char   buff_str[255];
    double buff_darr[NPARAMS];
    TFile* file_data = new TFile("hosp_normal.root","RECREATE");
    TNtuple* test_data = new TNtuple("test_data", "test data", PARAMS);

    // open file.
    FILE* fp;
    fp = fopen("normal_nuevo.txt", "r");
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
        tkn = strtok(buff_str, SEP);

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
            tkn = strtok(NULL, SEP);
        }
        if (word_cntr != NPARAMS) continue; // Skip lines with wrong number of parameters.

        Float_t buff_farr[NPARAMS];
        for (int par_i = 0; par_i < NPARAMS; ++par_i) buff_farr[par_i] = (Float_t) buff_darr[par_i];
        test_data->Fill(buff_farr);
    }

    fclose(fp);

    // NOTE: Process ntuple from here on out.
    test_data->Show(7);
    file_data->Write();
    file_data->Close();

    return(0);
}
