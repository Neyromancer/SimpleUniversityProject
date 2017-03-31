// main.c

#include <stdlib.h>
#include <stdio.h>

#include "interface.h"

int main( int argc, char *argv[] ) {
	int exitStatus = 0;

	exitStatus = displayMenu();
	if ( !exitStatus ) {
		return EXIT_SUCCESS;
	} else
		return EXIT_FAILURE;
}
