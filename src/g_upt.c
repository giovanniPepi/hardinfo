#include <string.h>
#include <math.h>
#include "../include/open_file.h"
#include "../include/constants.h"

void g_uptime(int ncpu) {
        
	double upt, idlet;
	int upt_int, idlet_int, active;
	char upt_unit, idlet_unit, active_unit;

	FILE *upt_f = open_file("/proc/uptime", "r");

	fscanf(upt_f, "%lf %lf", &upt, &idlet);
	
	/* Get avg per core count*/
	idlet = idlet / ncpu;

	upt_int = (int)(upt);
	idlet_int = (int)(idlet);

	active = upt - idlet;
	
	if(upt_int > S_TO_MIN) {
		upt_int = upt_int /  S_TO_MIN;
		upt_unit = 'm';
	} else if(upt_int > S_TO_HOUR) {
		upt_int = upt_int /  S_TO_HOUR;
		upt_unit = 'h';
	} else if(upt_int > S_TO_D) {
		upt_int = upt_int /  S_TO_D;
		upt_unit = 'd';
	} else {
		upt_unit = 's';
	}

	if(idlet_int > S_TO_MIN) {
		idlet_int = idlet_int /  S_TO_MIN;
		idlet_unit = 'm';
	} else if(idlet_int > S_TO_HOUR) {
		idlet_int = idlet_int /  S_TO_HOUR;
		idlet_unit = 'h';
	} else if(idlet_int > S_TO_D) {
		idlet_int = idlet_int /  S_TO_D;
		idlet_unit = 'd';
	} else {
		idlet_unit = 's';
	}

	if(active > S_TO_MIN) {
		active = active /  S_TO_MIN;
		active_unit = 'm';
	} else if(active > S_TO_HOUR) {
		active = active /  S_TO_HOUR;
		active_unit = 'h';
	} else if(active > S_TO_D) {
		active = active /  S_TO_D;
		active_unit = 'd';
	} else {
		active_unit = 's';
	}

	printf("Uptime: %d%c, Active %d%c, Idle Core time: %d%c\n", upt_int, upt_unit, active, active_unit, idlet_int, idlet_unit );


	fclose(upt_f);
}