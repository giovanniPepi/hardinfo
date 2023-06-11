#pragma once

/*Get CPU name*/
void g_cpu(char *cpu_name, FILE *cpu_file);

/*Get number of processors*/
void g_proc(int *pcount, FILE *cpu_file);

/*Get number of cores*/
void g_cc(int *cc, FILE *cpu_file);

/*Get CPU frequency*/
void get_cpuf(double **cpuf, double **min_cpuf, double **max_cpuf, FILE *cpu_file);

/* Fill start values with something bigger to compare */
void fill_min_freq_values (double **min_cpuf, int pcount);

void g_cpui();






