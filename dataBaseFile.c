// dataBaseFile.c

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct Matrix {
	int rows;
	int cols;
	char **movies;
} Matrix;

//===========================================
static void deleteExcessiveSpace( char * );
static void displayTableTitle( void );
static void toLower( char * );
static int isEqual( char *, char * );
static Matrix *fillArray( int row, int col );
static void freeMemory( Matrix *matrix );
void horizontalIndentation( void );
void verticalIndentation( void );
//============================================
		 
int readFromFile( void ) {
	char *textLine = NULL;
	size_t nbytes = 100;

	FILE *file = fopen( "movieStorage", "r" );
	if ( file == NULL ) {
		printf( "file can't be opened" );

		return 1;
	}
	if ( feof( file ) ) {
		printf( "file is empty" );

		return 1;
	}

	displayTableTitle();
	textLine = ( char * )malloc( nbytes + 1 );
	while ( getline( &textLine, &nbytes, file ) != -1 ) {
		horizontalIndentation();
		printf( "%s", textLine );
	}


	free( textLine );
	if ( fclose( file ) ) {
		printf( "file corrupted and can't be closed" );

		return 1;
	}

	return 0;
}

short rangeReleaseDate( int year ) { 
	int row = 0, col = 0, maxCol = 0;
	int c;	// read charachters from a file

	FILE *file = fopen( "movieStorage", "r" );
	if ( file == NULL ) {
		printf( "file can't be onpened" );

		return 1;
	}

	// count number of rows and columns
	do {
		col = 0;
		do {
			++col;
			c = fgetc( file );
		} while ( c != 10 );
		if ( col > maxCol ) 
			maxCol = col;
		
		++row;
		c = fgetc( file );
	} while ( c != EOF );
	col = maxCol;
	
	Matrix *mtrx = fillArray( row, col );

	// move to the begining of the file
	rewind( file );

	// read file and fill array
	int i = 0, j = 0;
	c = fgetc( file );	
	do {
		j = 0;
		do {	
			mtrx->movies[ i ][ j ] = c;
			++j;
			c = fgetc( file );
		} while( c != 10 );
		mtrx->movies[ i ][ j ] = c;
		++i;
		c = fgetc( file );
	} while( c != EOF );

/** print array's content
  *	printf( "\n" );
  *	for ( i = 0; i < row; ++i ) {
  *		for ( j = 0; j < col; ++j ) {
  *			printf( "%c", movies[ i ][ j ] );
  *		}
  *	}
**/
	displayTableTitle();
	// range in accordance with year
	int count, ind;
	char textYear[ 4 ];
	for ( int i = 0; i < mtrx->rows; ++i ) {
		count = 0;
		ind = 0;
		for ( int j = 0; j < mtrx->cols; ++j ) {
			if ( count == 1 && mtrx->movies[ i ][ j ]  != '|' ) {
				textYear[ ind ] = mtrx->movies[ i ][ j ];

				++ind;
			}

			if ( mtrx->movies[ i ][ j ] == '|' )
				++count;
		}

		// display movies that qualify the requirements
		if ( !atoi( textYear ) ) {
			printf( "Fail to read data from file." );
			printf( "\nData corrupted" );

			return 1;	// fail to read data from file
		}

		if ( year <= atoi( textYear ) ) {
			horizontalIndentation();
			printf( "%s", mtrx->movies[ i ] );
		}
	}
	printf( "\n" );
	
	// free memory
	freeMemory( mtrx );

	// close file	
	if ( fclose( file ) ) {
		printf( "File corrupted and can't be closed" );	

		return 1;	// fail to close file
	}

	return 0;	// function finished successfuly
}

short rangeMoviesWithActor( char *name ) {
	int row = 0, col = 0, maxCol = 0;
	int c;	// read charachters from a file

	FILE *file = fopen( "movieStorage", "r" );
	if ( file == NULL ) {
		printf( "file can't be onpened" );

		return 1;	// fail to open file
	}

	// count number of rows and columns
	do {
		col = 0;
		do {
			++col;
			c = fgetc( file );
		} while ( c != 10 );
		if ( col > maxCol ) 
			maxCol = col;
		
		++row;
		c = fgetc( file );
	} while ( c != EOF );
	col = maxCol;
	
	Matrix *mtrx = fillArray( row, col );

	// move to the begining of the file
	rewind( file );

	// read file and fill array
	int i = 0, j = 0;
	c = fgetc( file );	
	do {
		j = 0;
		do {	
			mtrx->movies[ i ][ j ] = c;
			++j;
			c = fgetc( file );
		} while( c != 10 );
		mtrx->movies[ i ][ j ] = c;
		++i;
		c = fgetc( file );
	} while( c != EOF );


	// range in accordance with actor name
	displayTableTitle();
	int count, ind;
	const int SIZE = 40;
	char actorName[ SIZE ];

	for ( int i = 0; i < mtrx->rows; ++i ) {
		count = 0;
		ind = 0;
		for ( int j = 0; j < mtrx->cols; ++j ) {
			if ( count == 2 && mtrx->movies[ i ][ j ]  != '|' ) {
				actorName[ ind ] = mtrx->movies[ i ][ j ];

				++ind;
			}

			if ( mtrx->movies[ i ][ j ] == '|' )
				++count;
		}
		deleteExcessiveSpace( actorName );

		// display movies that qualify the requirements
		if ( !isEqual( name, actorName ) ) {
			horizontalIndentation();
			printf( "%s", mtrx->movies[ i ] );
		}
	}
	
	// free memory
	freeMemory( mtrx );

	// close file	
	if ( fclose( file ) ) {
		printf( "file corrupted and can't be closed" );

		return 1;	// fail to close file	
	}

	return 0;
}

static void freeMemory( Matrix *matrix ) {
	for ( int i = 0; i < matrix->rows; ++i )
		free( matrix->movies[ i ] );
	free( matrix->movies );
	free( matrix );
}

static Matrix *fillArray( int row, int col ) {	
	Matrix *mtrx = ( Matrix* )malloc( sizeof( Matrix ) * row );

	mtrx->rows = row;
	mtrx->cols = col;

	// allocate memory
	mtrx->movies = ( char ** )malloc( sizeof( char *) * ( row ) );
	for ( int i = 0; i < row; ++i )
		mtrx->movies[ i ] = ( char * )malloc( sizeof( char *) * ( col ) );

	return mtrx;
}

int appedToFile( void ) {}

void horizontalIndentation( void ) {
	int feedLineNum = 5;
	for ( int i = 0; i < feedLineNum; ++i )
		printf( "\t" );

}

void verticalIndentation( void ) {
	int feedLineNum = 15;
	for ( int i = 0; i < feedLineNum; ++i )
		printf( "\n" );
}

static int isEqual( char *frstString, char *scndString ) {
	size_t frstSize = strlen( frstString );
	size_t scndSize = strlen( scndString );
	char frstLowCaseString[ frstSize ];
	char scndLowCaseString[ scndSize ];
	strcpy( frstLowCaseString, frstString );	
	strcpy( scndLowCaseString, scndString );
	toLower( frstLowCaseString );
	toLower( scndLowCaseString );
/***********************************************************/
   /*	for ( int i = 0; i < frstSize; ++i ) {		*
    *		printf( "%d", frstLowCaseString[ i ] );	*
    *	}						*
    *	printf( " " );					*
    *	for ( int i = 0; i < scndSize; ++i ) {		*
    *		printf( "%c", scndLowCaseString[ i ] );	*
    *	}						*
    *	printf( "\n" );					*/
/***********************************************************/
	int size = ( frstSize < scndSize ) ? frstSize : scndSize;
	for ( int i = 0; i < size; ++i ) {
		if ( frstLowCaseString[ i ] != scndLowCaseString[ i ] ) {		
			return 1;
		}
	}

	return 0;
}

static void toLower( char *name ) {
	for ( int i = 0; i < strlen( name ); ++i ) {
		if ( name[ i ] < 'a' && name[ i ] != ' ' && name[ i ] != '\0' ) 
			name[ i ] =  ( char )( name[ i ] + 32 );
	}
}

static void displayTableTitle( void ) {
	verticalIndentation();
	horizontalIndentation();
	printf( "         Title           | Year |     Main Actor     |   Index   |\n" );
	horizontalIndentation();
	printf( "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" );
}

// function delete excessive spaces between, before and after words
static void deleteExcessiveSpace( char *actorName ) {
	int  count = 0, i = 0, cnt = 0;
	char c1, c2;
	const size_t SIZE = strlen( actorName );
	char *name = ( char * )malloc( sizeof( char ) * SIZE ) ;

	do { 
		c1 = actorName[ cnt ];
		if ( c1 == ' ' || c1 == 32 ) {
			++count;
		}
		if ( c1 != ' ' || c2 != ' ' ) {
			if ( count <= 1 && cnt >= 2 ) {			
				name[ i ] = c2;
				++i;
			}
			count = 0;
		}
		c2 = c1;
		++cnt;
	} while ( cnt < SIZE );
	name[ i ] = '\0';
	strcpy( actorName, name );
	
	free( name );
}
