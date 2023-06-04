#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/open_file.h"

#define MAX_LINE_LENGTH 64
#define MIN_BUFF_LENGTH 128 
#define MAX_WORD_LENGTH 256

void get_cpu_name(char *cpu_name, FILE* cpuinfo_file) {
        char buff[MIN_BUFF_LENGTH];

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
        char buff[MAX_WORD_LENGTH];        
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
        char buff[MAX_WORD_LENGTH];        
        char score_count[MAX_LINE_LENGTH];
        
        rewind(cpuinfo_file); 

        while (fgets(buff, sizeof(buff), cpuinfo_file)) {
                if (strncmp(buff, "cpu cores", 9) == 0) {
                        sscanf(buff, "%*[^:]:%[^\n]", score_count); /* Skip the characters before the : */
                        break;      
                }
        }

        *(core_count) = atoi(score_count);
}


void hardinfo() {     

        char cpu_name[MAX_LINE_LENGTH];
        int proc_count = 0;
        int core_count;

        FILE* cpuinfo_file = open_file("/proc/cpuinfo", "r"); 

        get_cpu_name(cpu_name, cpuinfo_file);
        
        get_proc_count(&proc_count, cpuinfo_file);

        get_core_count(&core_count, cpuinfo_file);

        printf("%s\n", cpu_name);
        printf("Processors: %d, Cores: %d\n", proc_count, core_count);

}
