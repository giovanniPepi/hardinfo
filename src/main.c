#include "../include/hardinfo.h"
#include <sys/time.h>

double get_wall_time() {
	struct timeval time;
	gettimeofday(&time, 0);
	return (double)time.tv_sec + (double)time.tv_usec / 1000000.0;
}

int main() {

	double start_time, end_time, execution_time;
	start_time = get_wall_time();

    hardinfo();

    end_time = get_wall_time();
	execution_time = end_time - start_time;	

    printf("Time: %f seconds\n", execution_time);

    return 0;
}