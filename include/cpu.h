#define BUFF_LENGTH 128 
#define MAX_WORD_LENGTH 64

/*Get CPU name*/
void g_cpu(char *cpu_name, FILE* cpuinfo_file);

/*Get number of processors*/
void g_proc(int *pcount, FILE* cpuinfo_file);

/*Get number of cores*/
void g_cc(int *cc, FILE* cpuinfo_file);

/*Get CPU frequency*/
void get_cpuf(double **cpuf, double **min_cpuf, double **max_cpuf, FILE* cpuinfo_file);

/* Fill start values with something bigger to compare */
void fill_min_freq_values (double **min_cpuf, int pcount);






