// interface.c

#include <stdio.h>
#include <stdlib.h>

#include "dataBaseFile.h"

#define clear() printf( "\033[H\033[J" )

//======================================
static void checkIfContinue( void );
static short chooseCommand( short );
static short displayAllMovies( void );
static void addMovie( void ); 
static void deleteMovie( void ); 
static short rangeMovie( void );
static short exitFunction( void );
//#######################################
 
short displayMenu( void ) {
	clear();
	short commandNum = 0;
	short commandStatus = 0;
	short stopOrContinue;

	do {
		verticalIndentation();
		horizontalIndentation();	
		printf( "%25s", "1. Display all movies.\n" );
		horizontalIndentation();
		printf( "%22s", "2. Add a new movie.\n" );
		horizontalIndentation();
		printf( "%21s", "3. Delete a movie.\n" );
		horizontalIndentation();
		printf( "%19s", "4. Range movies.\n" );
		horizontalIndentation();
		printf( "%11s", "5. Exit.\n" );
		horizontalIndentation();
		printf( "%40s", "Choose a command from the list above: " );
		
		scanf("%1hd", &commandNum);
		stopOrContinue = chooseCommand( commandNum );
		clear();
	} while ( !stopOrContinue );

	return commandStatus;
}

static short chooseCommand( short command ){
	short exitStatus = 0;
	switch( command ) {
		case 1:
			exitStatus = displayAllMovies();
			break;
		case 2:
			addMovie();
			break;
		case 3:
			deleteMovie();
			break;
		case 4:
			exitStatus = rangeMovie();
			break;
		case 5:
			exitStatus = exitFunction();
			break;
		default:
			printf( "Something went wrong!\n" );
			break;
	}

	return exitStatus;
}

static short displayAllMovies( void ) {
	clear();
	short exitStatus = 0;
	
	exitStatus = readFromFile();
	checkIfContinue();

	return exitStatus;
}

static void checkIfContinue( void ) {
	horizontalIndentation();
	printf( "Press \'Y\' to continue: " );
	char ans, c1;
	int count = 0;
	c1 = getchar();
	do {
		ans = c1;
		c1 =  getchar();
	} while( c1 != '\n' );
	printf( "%c", ans );
	while( ans != 'y' && ans != 'Y' ) {
		horizontalIndentation();
		printf( "You have to enter \'y/Y\' to continue.\n");
		horizontalIndentation();
		printf( "Try again: " );
		do {
			ans = c1;
			c1 =  getchar();
		} while( c1 != '\n' );
	}
}

static void addMovie( void ) {}

static void deleteMovie( void ) {}

static short rangeMovie( void ) {
	clear();
	short num = 0;
	int year;
	char *name = NULL;
	size_t nameLength = 20; 
	name = ( char * )malloc( nameLength * sizeof( char ) );

	verticalIndentation();
	horizontalIndentation();
	printf( "Range Movies\n" );
	horizontalIndentation();
	printf( "1. Range with main actor\n" );
	horizontalIndentation();
	printf( "2. Range with release data\n" );
	horizontalIndentation();
	printf( "Choose from the list: " );

	scanf( "%1hd", &num );
	if ( num == 1 ) {
		horizontalIndentation();
		getchar();
		printf( "Enter actor's name: " );
		char c = getchar();
		int i = 0;
		while ( c != '\n' )  {	
			name[ i ] = c;
			c = getchar();
			++i;
		}
		clear();
		if ( rangeMoviesWithActor( name ) ) {
			return 1;
		}
		checkIfContinue();

	} else if( num == 2 ) {
		horizontalIndentation();
		printf( "Enter year: " );
		scanf( "%d", &year );
		clear();
		if ( rangeReleaseDate( year ) ) {
			return 1;
		}
		checkIfContinue();
	}
	
	// free memroy allocated by name array
	free( name );

	return 0;
}

static short exitFunction( void ) {
	clear();
	char exitYesOrNo, c1;
	
	verticalIndentation();
	horizontalIndentation();
	printf( "%38s", "Would you really like to exit? (Y/N) " );
	c1 = getchar();
	do {
		exitYesOrNo = c1;
		c1 = getchar();
	} while( c1 != '\n' );
	while ( ( exitYesOrNo != 'n' && exitYesOrNo != 'N' ) &&
		( exitYesOrNo != 'y' && exitYesOrNo != 'Y' )  
	      ) {
		horizontalIndentation();
		printf( "Incorrect answer format. It should contain (Y/N) only.\n" );
		horizontalIndentation();
		printf( "Try again: " );
		do {
			exitYesOrNo = c1;
			c1 = getchar();
		} while ( c1 != '\n' );
	};

	if ( exitYesOrNo == 'n' || exitYesOrNo == 'N' )
		return 0;
	else
		return 1;
}
