#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <carafe.h>
#include <log.h>

FILE * pLog;

void log_init(void) {
	// Could do with actually making said file.
	pLog = fopen("carafe.log", "a");
	fputs("Application startup....\n", pLog);
	// pLog = fopen("/var/log/carafe/test.log", "a");
	// fprintf(pLog, "Startup\n");
	// fclose(pLog);
}

void log_close(void) {
	fclose(pLog);
}

void log_puts(char * s) {
	fputs(s, pLog);
	fflush(pLog);
}

void log_printf(char * s) {

}