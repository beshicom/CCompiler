


//	再帰下降構文解析法 recursive descent parsing
//	32ビット用



#include	<stdio.h>
#include	<stdlib.h>



int main ( int argc, char **argv )
{

	if( argc !=2 ){
		fprintf( stderr, "引数の個数が正しくありません。\n" );
		return 100;
	}

	printf( "\n\n\n" );
	printf( ".386\n" );
	printf( ".model  flat, stdcall\n" );
	printf( "option  casemap: none\n" );
	printf( "\n\n\n" );
	printf( ".code\n" );
	printf( "start:\n" );
	printf( "\n\n\n" );

	char *	pCmd = argv[1];

	printf(  "    mov     eax, %ld\n",  strtol( pCmd, &pCmd, 10 )  );
		// 与えられた文字列を10進数とみなして数値化、次のアドレスをセットする

	while( *pCmd ){

		switch( *pCmd ){
		case '+':
			++pCmd;
			printf(  "    add     eax, %ld\n",  strtol( pCmd, &pCmd, 10 )  );
			break;
		case '-':
			++pCmd;
			printf(  "    sub     eax, %ld\n",  strtol( pCmd, &pCmd, 10 )  );
			break;
		default:
			fprintf( stderr, "予期しない文字です。: '%c'\n", *pCmd );
			return 1100;
		}// switch

	} //while *pCmd

	printf( "    ret\n" );
	printf( "\n\n\n" );
	printf( "end start\n" );
	printf( "\n\n\n" );

	return 0;

}



