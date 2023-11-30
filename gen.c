


// 構文木からアセンブラコードを生成
//	機械的にスタックマシンとしてのコードを生成。


#include	"gen.h"



// 構文木からアセンブラコードを生成する gen()					//TAG_JUMP_MARK
//	機械的にスタックマシンとしての x86コードを生成。
int gen ( Node * node, int nDepth )
{

	nLastError = 0;

	if( node == NULL ){
		nLastError = 2;
		return 2;
	}

	if( node->kind == ND_NUM ){
		Indent0( nDepth );
		printf( "    push    %d\n", node->val );
		return 0;
	}

	// 左辺値の生成
	//Indent0( nDepth+1 );
	//printf( "    ; left hand side\n" );
	nLastError = gen( node->lhs, nDepth+1 );
	if( nLastError != 0 )	return nLastError;

	// 右辺値の生成
	//Indent0( nDepth+1 );
	//printf( "    ; right hand side\n" );
	nLastError = gen( node->rhs, nDepth+1 );
	if( nLastError != 0 )	return nLastError;

	// 計算コードの生成
	Indent0( nDepth );
	printf( "    pop     edi\n" );
	Indent0( nDepth );
	printf( "    pop     eax\n" );

	switch( node->kind ){
	case ND_ADD:
		Indent0( nDepth );
		printf( "    add     eax, edi\n" );
		break;
	case ND_SUB:
		Indent0( nDepth );
		printf( "    sub     eax, edi\n" );
		break;
	case ND_MUL:
		Indent0( nDepth );
		printf( "    imul    eax, edi\n" );
		break;
	case ND_DIV:
		Indent0( nDepth );
		printf( "    cdq\n" );
			// eaxを64ビット値に拡張し、上位32ビットをedxへ入れる。
		Indent0( nDepth );
		printf( "    idiv    edi\n" );
			//edx eax を64ビット整数とみなしてediで符号付き除算をして
			//	商をeax、余りをedxに入れる。
		break;
	}// switch

	// 計算結果をスタックへ積む。
	Indent0( nDepth );
	printf( "    push    eax\n" );
	printf( "\n\n" );

	return 0;

}
//int gen ( Node * node )



// 構文木からアセンブラコードを生成する gen()					//TAG_JUMP_MARK
//	機械的にスタックマシンとしての x86コードを生成。
int GenAsm ( Node * node )
{

	nLastError = 0;

	if( node == NULL ){
		nLastError = 2;
		return 2;
	}

	printf( "\n\n\n" );
	printf( ".386\n" );
	printf( ".model  flat, stdcall\n" );
	printf( "option  casemap: none\n" );
	printf( "\n\n\n" );
	printf( ".code\n" );
	printf( "start:\n" );
	printf( "\n\n\n" );

	nLastError = gen( node, 0 );

	printf( "    pop     eax\n" );
	printf( "    ret\n" );
	printf( "\n\n\n" );
	printf( "end start\n" );
	printf( "\n\n\n" );

	return nLastError;

}



// end of this file : gen.c
