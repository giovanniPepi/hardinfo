#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/open_file.h"
#include "../include/cpu.h"
#include "../include/constants.h"
#include "../include/mem.h"


void hardinfo() {     

        char cpu_name[MAX_WORD_LENGTH];
        int pcount = 0;
        int cc = 0;
        int i = 0;
        int seconds = 1;
        double *cpuf = NULL;
        double *min_cpuf = NULL;
        double *max_cpuf = NULL;

        FILE* cpu_file = open_file("/proc/cpuinfo", "r"); 
        FILE* mem_file = open_file("/proc/meminfo", "r"); 

        g_cpu(cpu_name, cpu_file);        
        g_proc(&pcount, cpu_file);
        g_cc(&cc, cpu_file);

        /* Mallocs*/
        cpuf = calloc(pcount, sizeof(double));                
        min_cpuf = calloc(pcount, sizeof(double));
        max_cpuf = calloc(pcount, sizeof(double));
        fill_min_freq_values(&min_cpuf, pcount);

        g_tot_mem(mem_file);

        printf("%s\n", cpu_name);
        printf("Processors: %d, Cores: %d\n", pcount, cc);

        clock_t start_time = clock();
        printf("Collecting data for %d seconds...\n", seconds);

        do {                
                get_cpuf(&cpuf, &min_cpuf, &max_cpuf, cpu_file);
        } while((clock() - start_time) / CLOCKS_PER_SEC < seconds);

        printf("[Frequencies] \n");
        for (i = 0; i < pcount; i++) {
                printf("Processor [%d] Current %.0f MHz || Max %.0f || Min %.0f  \n", i, cpuf[i],  max_cpuf[i], min_cpuf[i]);
        }
        
        free(cpuf);
}
