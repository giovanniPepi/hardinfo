#include "../include/constants.h"
#include "string.h"
#include <stdio.h>
#include "../include/open_file.h"

typedef struct {
        int tot;
        int avail;
        
        double dtot;
        double davail;     

}  Mem;

Mem mem = {.tot = 0, .avail = 0, .dtot = 0, .davail = 0  };       

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

void g_memi() {
        FILE* mem_file = open_file("/proc/meminfo", "r");     
        char memt_s[] = "MemTotal";
        char memf_s[] = "MemFree";
        
        g_value(mem_file, memt_s, &mem.tot);
        conv_gib(&mem.tot, &mem.dtot );

        printf("Total: %.2f GiB\n", mem.dtot);
}