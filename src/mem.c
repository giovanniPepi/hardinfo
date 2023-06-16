#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/constants.h"
#include "../include/open_file.h"

typedef struct {
        int tot;
        int free;
        int used;
        int tot_s;
        int free_s;
        int used_s;
        
        double dtot;
        double dfree;     
        double dused;
        double dtot_s;
        double dfree_s;
        double dused_s;

}  Mem;

Mem mem;

/*finds value in /proc*/
void g_value(FILE *mem_file, char *name, int *result) {
        
        char buff[BUFF_LENGTH];   	
        rewind(mem_file); 

        while (fgets(buff, sizeof(buff), mem_file)) {                
                if (strstr(buff, name)) {
                        sscanf(buff, "%*[^:]:%d", result); 
                        break;      
                }
        }        
}

void conv_gib(int *val, double *res){
        *(res) = (float)*(val) /GIB_KB;
}

void g_used(int *tot, int *free, int* used) {
        *(used) = *(tot) - *(free);
}

void g_swapn(FILE *swap_file, char *swap_name) {
        
        char line[MAX_WORD_LENGTH];

        /*Read the first line, do nothing*/
        fgets(line, sizeof(line), swap_file);

        if (fgets(line, sizeof(line), swap_file) != NULL) {
                char *name = strtok(line, " \t");
                strcpy(swap_name, name);
        }

        fclose((swap_file));
}


void g_memi() {

        FILE* mem_file = open_file("/proc/meminfo", "r");     
        FILE* swap_file = open_file("/proc/swaps", "r");     

        char memt_s[] = "MemTotal";
        char memf_s[] = "MemFree";
        char memt_swap[] = "SwapTotal";
        char memf_swap[] = "SwapFree";
        char *swap_name = calloc(MAX_WORD_LENGTH, sizeof(char));
        
        g_value(mem_file, memt_s, &mem.tot);
        g_value(mem_file, memf_s, &mem.free);
        g_used(&mem.tot, &mem.free, &mem.used);

        g_swapn(swap_file, swap_name);
        g_value(mem_file, memt_swap, &mem.tot_s);
        g_value(mem_file, memf_swap, &mem.free_s);
        g_used(&mem.tot_s, &mem.free_s, &mem.used_s);
        
        conv_gib(&mem.tot, &mem.dtot );
        conv_gib(&mem.free, &mem.dfree );
        conv_gib(&mem.used, &mem.dused );

        conv_gib(&mem.tot_s, &mem.dtot_s );
        conv_gib(&mem.free_s, &mem.dfree_s );
        conv_gib(&mem.used_s, &mem.dused_s );      

        printf("\nMEMORY\n");
        printf("Total: %.2f GiB\n", mem.dtot);
        printf("Free: %.2f GiB\n", mem.dfree);
        printf("Used: %.2f GiB\n", mem.dused);
        
        printf("SWAP: %s\n", swap_name);
        printf("Total Swap: %.2f GiB\n", mem.dtot_s);
        printf("Free Swap: %.2f GiB\n", mem.dfree_s);
        printf("Used Swap: %.2f GiB\n", mem.dused_s);

        free(swap_name);

}