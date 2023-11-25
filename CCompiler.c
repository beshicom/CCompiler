


//	再帰下降構文解析法 recursive descent parsing
//	32ビット用



#include	<stdio.h>
#include	<stdlib.h>



int main ( int argc, char **argv )
{

	if( argc !=2 ){
		fprintf( stderr, "引数の個数が正しくありません。\n" );
		return 1;
	}

	printf( "\n\n\n" );
	printf( ".386\n" );
	printf( ".model  flat, stdcall\n" );
	printf( "option  casemap: none\n" );
	printf( "\n\n\n" );
	printf( ".code\n" );
	printf( "start:\n" );
	printf( "\n\n\n" );
	printf( "    mov     eax, %s\n", argv[1] );
	printf( "    ret\n" );
	printf( "\n\n\n" );
	printf( "end start\n" );
	printf( "\n\n\n" );

	return 0;

}



