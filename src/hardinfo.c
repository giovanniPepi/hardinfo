#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/open_file.h"
#include "../include/cpu.h"
#include "../include/constants.h"
#include "../include/mem.h"

void hardinfo() {     
        
        printf("\nMEMORY\n");
        g_memi();
        
        printf("\nCPU\n");
        g_cpui();

}
