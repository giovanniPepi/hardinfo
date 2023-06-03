#include <stdio.h>
#include <string.h>
#include "../include/open_file.h"

#define MAX_LINE_LENGTH 256

char line[MAX_LINE_LENGTH];
char cpu_name[MAX_LINE_LENGTH];

void get_cpu_info(char *cpu_name, FILE* cpuinfo_file) {
        printf("Getting /proc/cpuinfo ...\n");

        while (fgets(line, sizeof(line), cpuinfo_file)) {
                if (strncmp(line, "model name", 10) == 0) {
                        sscanf(line, "%*[^:]:%[^\n]", cpu_name); /* Skip the characters before the : */
                        break;      
                }
        }
}

void hardinfo() {        
        FILE* cpuinfo_file = open_file("/proc/cpuinfo", "r"); 

        get_cpu_info(cpu_name, cpuinfo_file);

        printf("CPU name: %s\n", cpu_name);
}
