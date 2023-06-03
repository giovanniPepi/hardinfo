#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

void get_cpu_info(char *cpu_name)
{
    FILE *file = fopen("/proc/cpuinfo", "r");
    char line[MAX_LINE_LENGTH];
    
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "model name", 10) == 0)
        {
            sscanf(line, "%*[^:]:%[^\n]", cpu_name); /* Skip the characters before the : */
            break;
        }
    }
    
    fclose(file);
}

void hardinfo() {
    printf("Getting /proc/cpuinfo ...\n");

    char cpu_name[MAX_LINE_LENGTH];

    get_cpu_info(cpu_name);

    printf("CPU name: %s\n", cpu_name);

}