#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/open_file.h"
#include "../include/cpu.h"
#include "../include/constants.h"
#include "../include/g_upt.h"

/*Initialize min values with higher values so they can be compared*/
void fill_min_freq_values (double **min_cpuf, int pcount) {
        
        int i;

        for (i = 0; i < pcount; i++) {
                (*min_cpuf)[i] = 10000;
        }
}

void g_cpu(char *cpu_name, FILE *cpu_file) {

        char buff[BUFF_LENGTH];

        /* Without rewind, any other function that used the stream may have moved the start pos */
        rewind(cpu_file); 

        while (fgets(buff, sizeof(buff), cpu_file)) {
                if (strncmp(buff, "model name", 10) == 0) {
                        sscanf(buff, "%*[^:]:%*[ ]%[^\n]", cpu_name); /* Skip the characters before the : */
                        break;      
                }
        }
}

void g_proc(int *pcount, FILE *cpu_file) {
        char buff[BUFF_LENGTH];        
        char sword[] = "processor";

        rewind(cpu_file); 

        /* fscanf == 1 means successful reading*/
        while (fscanf(cpu_file, "%s", buff) == 1) {
                if(strcmp(buff, sword) == 0){
                        (*pcount)++;
                }
        }
}

void g_cc(int *cc, FILE *cpu_file) {
        char buff[BUFF_LENGTH];        
        char scc[MAX_WORD_LENGTH];
        
        rewind(cpu_file); 

        while (fgets(buff, sizeof(buff), cpu_file)) {                
                if (strstr(buff, "cpu cores")) {
                        sscanf(buff, "%*[^:]:%[^\n]", scc); /* Skip the characters before the : */
                        break;      
                }
        }

        *(cc) = atoi(scc);
}

void get_cpuf(double **cpuf, double **min_cpuf, double **max_cpuf, FILE *cpu_file) {
    char buff[BUFF_LENGTH];
    char tcpuf[MAX_WORD_LENGTH];
    int i = 0;
    double min = 0;
    double current = 0;
    double max = 0;

    rewind(cpu_file);

        while (fgets(buff, sizeof(buff), cpu_file)) {
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

void g_cpui() {
        FILE *cpu_file = open_file("/proc/cpuinfo", "r"); 

        int i = 0;
        int seconds = 1;
        
        typedef struct {
                char cpu_name[MAX_WORD_LENGTH];
                int pcount;
                int cc;
                double *cpuf;
                double *min_cpuf;
                double *max_cpuf;

        } CPU;

        CPU cpu = {.pcount = 0, .cc = 0, .cpuf = NULL, .min_cpuf = NULL, .max_cpuf = NULL };       

        /*Proc count is used for mallocs*/
        g_proc(&cpu.pcount, cpu_file);

        /* Mallocs*/
        cpu.cpuf = calloc(cpu.pcount, sizeof(double));                
        cpu.min_cpuf = calloc(cpu.pcount, sizeof(double));
        cpu.max_cpuf = calloc(cpu.pcount, sizeof(double));
        fill_min_freq_values(&cpu.min_cpuf, cpu.pcount);

        g_cpu(cpu.cpu_name, cpu_file);        
        g_cc(&cpu.cc, cpu_file);

        /* prints*/
        printf("\nCPU\n");
        printf("%s\n", cpu.cpu_name);
        printf("Processors: %d, Cores: %d\n", cpu.pcount, cpu.cc);

        /* Timed data*/
        clock_t start_time = clock();
        printf("Collecting data for %d seconds...\n", seconds);

        do {                
                get_cpuf(&cpu.cpuf, &cpu.min_cpuf, &cpu.max_cpuf, cpu_file);
        } while((clock() - start_time) / CLOCKS_PER_SEC < seconds);

        printf("[Frequencies] \n");

        for (i = 0; i < cpu.pcount; i++) {
                printf("Processor [%d]\tCurrent %.0f MHz \tMax %.0f \tMin %.0f\n", i, cpu.cpuf[i],  cpu.max_cpuf[i], cpu.min_cpuf[i]);
        }

        g_uptime(cpu.pcount);
        
        free(cpu.cpuf);
        free(cpu.min_cpuf);
        free(cpu.max_cpuf);
       
}