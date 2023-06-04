#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/open_file.h"

#define BUFF_LENGTH 128 
#define MAX_WORD_LENGTH 64

void get_cpu_name(char *cpu_name, FILE* cpuinfo_file) {
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

void get_proc_count(int *proc_count, FILE* cpuinfo_file) {
        char buff[BUFF_LENGTH];        
        char sword[] = "processor";

        rewind(cpuinfo_file); 

        /* fscanf == 1 means successful reading*/
        while (fscanf(cpuinfo_file, "%s", buff) == 1) {
                if(strcmp(buff, sword) == 0){
                        (*proc_count)++;
                }
        }
}

void get_core_count(int *core_count, FILE* cpuinfo_file) {
        char buff[BUFF_LENGTH];        
        char score_count[MAX_WORD_LENGTH];
        
        rewind(cpuinfo_file); 

        while (fgets(buff, sizeof(buff), cpuinfo_file)) {
                if (strstr(buff, "cpu cores")) {
                        sscanf(buff, "%*[^:]:%[^\n]", score_count); /* Skip the characters before the : */
                        break;      
                }
        }

        *(core_count) = atoi(score_count);
}

void get_cpu_mhz(double **cpu_mhz, double **min_cpu_mhz, double **max_cpu_mhz, FILE* cpuinfo_file) {
    char buff[BUFF_LENGTH];
    char tcpu_mhz[MAX_WORD_LENGTH];
    int i = 0;
    double min = 0;
    double current = 0;
    double max = 0;
    rewind(cpuinfo_file);

        while (fgets(buff, sizeof(buff), cpuinfo_file)) {
                if (strstr(buff, "MHz")) {
                        sscanf(buff, "%*[^:]:%[^\n]", tcpu_mhz);
                        (*cpu_mhz)[i] = strtod(tcpu_mhz, NULL);                        

                        /* Historical data set: */
                        current = (*cpu_mhz)[i];
                        min = (*min_cpu_mhz)[i];
                        max = (*max_cpu_mhz)[i];

                        if (current < min) {
                                (*min_cpu_mhz)[i] = current;
                        }   
                        
                        if(current > max) {
                                (*max_cpu_mhz)[i] = current;
                        }

                        i++;
                }
        }
}

void fill_min_freq_values (double **min_cpu_mhz, int proc_count) {
        
        int i;
        for (i = 0; i < proc_count; i++) {
                (*min_cpu_mhz)[i] = 10000;
        }
}

void hardinfo() {     

        char cpu_name[MAX_WORD_LENGTH];
        int proc_count = 0;
        int core_count = 0;
        int i = 0;
        int j = 0;         
        double *cpu_mhz = NULL;
        double *min_cpu_mhz = NULL;
        double *max_cpu_mhz = NULL;

        FILE* cpuinfo_file = open_file("/proc/cpuinfo", "r"); 

        get_cpu_name(cpu_name, cpuinfo_file);        
        get_proc_count(&proc_count, cpuinfo_file);
        get_core_count(&core_count, cpuinfo_file);

        /* Mallocs*/
        cpu_mhz = calloc(proc_count, sizeof(double));                
        min_cpu_mhz = calloc(proc_count, sizeof(double));
        max_cpu_mhz = calloc(proc_count, sizeof(double));

        /* Fill start values with something bigger to compare */
        fill_min_freq_values(&min_cpu_mhz, proc_count);

        while (j < 10000) {
                get_cpu_mhz(&cpu_mhz, &min_cpu_mhz, &max_cpu_mhz, cpuinfo_file);
                j++;
        }

        /* Print region*/
        printf("%s\n", cpu_name);
        printf("Processors: %d, Cores: %d\n", proc_count, core_count);
        printf("[Frequencies] \n");

        for (i = 0; i < proc_count; i++) {
                printf("Processor [%d] Current %.0f MHz || Min %.0f || Max %.0f \n", i, cpu_mhz[i], min_cpu_mhz[i], max_cpu_mhz[i]);
        }

        
        free(cpu_mhz);

}
