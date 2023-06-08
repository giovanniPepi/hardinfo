#include "../include/constants.h"
#include "string.h"
#include <stdio.h>

void g_tot_mem(FILE *mem_file) {
        char buff[BUFF_LENGTH];   
	int totm = 0;
        
        rewind(mem_file); 

        while (fgets(buff, sizeof(buff), mem_file)) {                
                if (strstr(buff, "MemTotal")) {
                        sscanf(buff, "%*[^:]:%d", &totm); 
                        break;      
                }
        }

	printf("Total: %.2f GiB\n ", (float)(totm/GIB_KB));
}
