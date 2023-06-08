#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../include/open_file.h"
#include "../include/cpu.h"

void hardinfo() {     

        char cpu_name[MAX_WORD_LENGTH];
        int pcount = 0;
        int cc = 0;
        int i = 0;
        int seconds = 1;
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
        fill_min_freq_values(&min_cpuf, pcount);

        printf("%s\n", cpu_name);
        printf("Processors: %d, Cores: %d\n", pcount, cc);

        clock_t start_time = clock();
        printf("Collecting data for %d seconds...\n", seconds);

        do {                
                get_cpuf(&cpuf, &min_cpuf, &max_cpuf, cpuinfo_file);
        } while((clock() - start_time) / CLOCKS_PER_SEC < seconds);

        printf("[Frequencies] \n");
        for (i = 0; i < pcount; i++) {
                printf("Processor [%d] Current %.0f MHz || Max %.0f || Min %.0f  \n", i, cpuf[i],  max_cpuf[i], min_cpuf[i]);
        }
        
        free(cpuf);
}
