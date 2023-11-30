


//	再帰下降構文解析法 recursive descent parsing
//	32ビット用



#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<stdarg.h>
#include	<stdbool.h>
#include	<string.h>

#include	"token.h"
#include	"parse.h"



// main()														//TAG_JUMP_MARK
int main ( int argc, char **argv )
{

	if(  ( argc != 2 )&&( argc != 3 )  ){
		fprintf( stderr, "引数の個数が正しくありません。\n" );
		fprintf( stderr, "Usage : CCompiler ソース文字列 [1|2|3]\n" );
		fprintf( stderr, "\t1 (規定値) : アセンブラソース表示\n" );
		fprintf( stderr, "\t2          : 構文木表示\n" );
		fprintf( stderr, "\t3          : アセンブラソース表示、構文木表示\n" );
		return 100;
	}

	int		fAsm = 1;
	int		fPrint = 0;
	if( argc == 3 ){
		switch( atoi(argv[2]) ){
		case 1:  fAsm = 1;  fPrint = 0;  break;
		case 2:  fAsm = 0;  fPrint = 1;  break;
		case 3:  fAsm = 1;  fPrint = 1;  break;
		}
	}

	// 引数からトークンのリストを作る
	user_input = argv[1];
	Token *	mToken = token = tokenize( argv[1], 1000 );

	// トークンのリストから構文木を作る
	Parse();

	if( fPrint ){
		printf( "%s = %d\n", argv[1], Calc( NodeTop ) );
		Print();
		printf( "push return -------------" );
		getchar();
		printf( "\n" );
	}

	/*
	if( ! fAsm ){
		delete_list( mToken );
		delete_tree( NodeTop );
		return 0;
	}

	printf( "\n\n\n" );
	printf( ".386\n" );
	printf( ".model  flat, stdcall\n" );
	printf( "option  casemap: none\n" );
	printf( "\n\n\n" );
	printf( ".code\n" );
	printf( "start:\n" );
	printf( "\n\n\n" );

	// 式の最初は数値でなければならないので、それをチェックしつつ出力
	printf(  "    mov     eax, %ld\n",  expect_number( 2100 )  );

	while( ! at_eof() ){

		if( consume( '+' ) ){
			printf(  "    add     eax, %ld\n",  expect_number( 2200 )  );
		}

		expect( '-', 2300 );
		printf(  "    sub     eax, %ld\n",  expect_number( 2400 )  );

	} //while *pCmd

	printf( "    ret\n" );
	printf( "\n\n\n" );
	printf( "end start\n" );
	printf( "\n\n\n" );

	*/

	delete_list( mToken );
	delete_tree( NodeTop );

	return 0;

}



