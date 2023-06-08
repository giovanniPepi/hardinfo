#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/open_file.h"
#include "../include/cpu.h"
#include "../include/constants.h"

/*Initialize min values with higher values so they can be compared*/
void fill_min_freq_values (double **min_cpuf, int pcount) {
        
        int i;

        for (i = 0; i < pcount; i++) {
                (*min_cpuf)[i] = 10000;
        }
}

void g_cpu(char *cpu_name, FILE* cpuinfo_file) {
        char buff[BUFF_LENGTH];

        /* Without rewind, any other function that used the stream may have moved the start pos */
        rewind(cpuinfo_file); 

        while (fgets(buff, sizeof(buff), cpuinfo_file)) {
                if (strncmp(buff, "model name", 10) == 0) {
                        sscanf(buff, "%*[^:]:%*[ ]%[^\n]", cpu_name); /* Skip the characters before the : */
                        break;      
                }
        }
}

void g_proc(int *pcount, FILE* cpuinfo_file) {
        char buff[BUFF_LENGTH];        
        char sword[] = "processor";

        rewind(cpuinfo_file); 

        /* fscanf == 1 means successful reading*/
        while (fscanf(cpuinfo_file, "%s", buff) == 1) {
                if(strcmp(buff, sword) == 0){
                        (*pcount)++;
                }
        }
}

void g_cc(int *cc, FILE* cpuinfo_file) {
        char buff[BUFF_LENGTH];        
        char scc[MAX_WORD_LENGTH];
        
        rewind(cpuinfo_file); 

        while (fgets(buff, sizeof(buff), cpuinfo_file)) {                
                if (strstr(buff, "cpu cores")) {
                        sscanf(buff, "%*[^:]:%[^\n]", scc); /* Skip the characters before the : */
                        break;      
                }
        }

        *(cc) = atoi(scc);
}

void get_cpuf(double **cpuf, double **min_cpuf, double **max_cpuf, FILE* cpuinfo_file) {
    char buff[BUFF_LENGTH];
    char tcpuf[MAX_WORD_LENGTH];
    int i = 0;
    double min = 0;
    double current = 0;
    double max = 0;

    rewind(cpuinfo_file);

        while (fgets(buff, sizeof(buff), cpuinfo_file)) {
                if (strstr(buff, "MHz")) {
                        sscanf(buff, "%*[^:]:%[^\n]", tcpuf);
                        (*cpuf)[i] = strtod(tcpuf, NULL);                        

                        /* Historical data set: */
                        current = (*cpuf)[i];
                        min = (*min_cpuf)[i];
                        max = (*max_cpuf)[i];

                        if (current < min) {
                                (*min_cpuf)[i] = current;
                        }   
                        
                        if(current > max) {
                                (*max_cpuf)[i] = current;
                        }

                        i++;
                }
        }
}

