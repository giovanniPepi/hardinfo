#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/open_file.h"

#define BUFF_LENGTH 128 
#define MAX_WORD_LENGTH 64

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

void fill_min_freq_values (double **min_cpuf, int pcount) {
        
        int i;

        for (i = 0; i < pcount; i++) {
                (*min_cpuf)[i] = 10000;
        }
}

void hardinfo() {     

        char cpu_name[MAX_WORD_LENGTH];
        int pcount = 0;
        int cc = 0;
        int i = 0;
        int seconds = 5;
        double *cpuf = NULL;
        double *min_cpuf = NULL;
        double *max_cpuf = NULL;

        FILE* cpuinfo_file = open_file("/proc/cpuinfo", "r"); 

        g_cpu(cpu_name, cpuinfo_file);        
        g_proc(&pcount, cpuinfo_file);
        g_cc(&cc, cpuinfo_file);

        /* Mallocs*/
        cpuf = calloc(pcount, sizeof(double));                
        min_cpuf = calloc(pcount, sizeof(double));
        max_cpuf = calloc(pcount, sizeof(double));

        /* Fill start values with something bigger to compare */
        fill_min_freq_values(&min_cpuf, pcount);

        clock_t start_time = clock();

        printf("Collecting data for %d seconds...\n", seconds);

        do {                
                get_cpuf(&cpuf, &min_cpuf, &max_cpuf, cpuinfo_file);
        } while((clock() - start_time) / CLOCKS_PER_SEC < seconds);

        /* Print region*/
        printf("%s\n", cpu_name);
        printf("Processors: %d, Cores: %d\n", pcount, cc);
        printf("[Frequencies] \n");

        for (i = 0; i < pcount; i++) {
                printf("Processor [%d] Current %.0f MHz || Max %.0f || Min %.0f  \n", i, cpuf[i],  max_cpuf[i], min_cpuf[i]);
        }
        
        free(cpuf);
}
