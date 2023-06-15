#include "../include/constants.h"
#include "string.h"
#include <stdio.h>
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

void g_memi() {
        FILE* mem_file = open_file("/proc/meminfo", "r");     

        char memt_s[] = "MemTotal";
        char memf_s[] = "MemFree";
        char memt_swap[] = "SwapTotal";
        char memf_swap[] = "SwapFree";
        
        g_value(mem_file, memt_s, &mem.tot);
        g_value(mem_file, memf_s, &mem.free);
        g_used(&mem.tot, &mem.free, &mem.used);

        g_value(mem_file, memt_swap, &mem.tot_s);
        g_value(mem_file, memf_swap, &mem.free_s);
        g_used(&mem.tot_s, &mem.free_s, &mem.used_s);
        
        conv_gib(&mem.tot, &mem.dtot );
        conv_gib(&mem.free, &mem.dfree );
        conv_gib(&mem.used, &mem.dused );

        conv_gib(&mem.tot_s, &mem.dtot_s );
        conv_gib(&mem.free_s, &mem.dfree_s );
        conv_gib(&mem.used_s, &mem.dused_s );      

        printf("Total: %.2f GiB\n", mem.dtot);
        printf("Free: %.2f GiB\n", mem.dfree);
        printf("Used: %.2f GiB\n", mem.dused);

        printf("Total Swap: %.2f GiB\n", mem.dtot_s);
        printf("Free Swap: %.2f GiB\n", mem.dfree_s);
        printf("Used Swap: %.2f GiB\n", mem.dused_s);

}