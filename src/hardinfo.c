#include <stdio.h>
#include <string.h>
#include "../include/open_file.h"

#define MAX_LINE_LENGTH 64
#define MIN_BUFF_LENGTH 128 
#define MAX_WORD_LENGTH 256

void get_cpu_name(char *cpu_name, FILE* cpuinfo_file) {
        char buff[MIN_BUFF_LENGTH];

        while (fgets(buff, sizeof(buff), cpuinfo_file)) {
                if (strncmp(buff, "model name", 10) == 0) {
                        sscanf(buff, "%*[^:]:%[^\n]", cpu_name); /* Skip the characters before the : */
                        break;      
                }
        }
}

void get_cpu_count(int *cpu_count, FILE* cpuinfo_file) {
        char buff[MAX_WORD_LENGTH];        
        char sword[] = "processor";

        /* Necessary to read the entire file again*/
        rewind(cpuinfo_file); 

        /* fscanf == 1 means successful reading*/
        while (fscanf(cpuinfo_file, "%s", buff) == 1) {
                if(strcmp(buff, sword) == 0){
                        (*cpu_count)++;
                }
        }
}

void hardinfo() {     

        char cpu_name[MAX_LINE_LENGTH];
        int cpu_count = 0;

        FILE* cpuinfo_file = open_file("/proc/cpuinfo", "r"); 

        get_cpu_name(cpu_name, cpuinfo_file);
        
        get_cpu_count(&cpu_count, cpuinfo_file);

        printf("CPU name: %s\n", cpu_name);
        printf("CPU Count: %d\n", cpu_count);

}
