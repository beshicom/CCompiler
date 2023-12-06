


// 構文木からアセンブラコードを生成
//	機械的にスタックマシンとしてのコードを生成。



#include	"gen.h"



// 構文木からアセンブラコードを生成する gen_lval()				//TAG_JUMP_MARK
//	左辺値のコードの生成。
//	変数のアドレスを計算してスタックにpushする。
//	機械的にスタックマシンとしての x86コードを生成。
int gen_lval ( Node * node, int nDepth )
{

	nLastError = 0;

	if( node == NULL ){
		nLastError = 2;
		return 2;
	}

	if( node->kind != ND_LCVAR ){
		error( 10000, 100, "代入の左辺値が変数ではありません。" );
	}

	Indent0( nDepth );
	printf( "    mov     eax, ebp\n" );
	printf( "    sub     eax, %d\n", node->offset );
	printf( "    push    eax\n" );

	return 0;

}
//int gen_lval ( Node * node, int nDepth )



// 構文木からアセンブラコードを生成する gen()					//TAG_JUMP_MARK
//	機械的にスタックマシンとしての x86コードを生成。
int gen ( Node * node, int nDepth )
{

	nLastError = 0;

	if( node == NULL ){
		nLastError = 2;
		return 2;
	}

	/*
	if( node->kind == ND_NUM ){
		Indent0( nDepth );
		printf( "    push    %d\n", node->val );
		return 0;
	}
	*/

	switch( node->kind ){
	case ND_NUM:	// 数値
		Indent0( nDepth );
		printf( "    push    %d\n", node->val );
		return 0;
	case ND_LCVAR:	// ローカル変数
		{
		int		rslt = gen_lval( node, nDepth+1 );
		if( rslt != 0 )		return rslt;
		Indent0( nDepth );
		printf( "    pop     eax\n" );
					// スタックに積まれたローカル変数のアドレスを取り出す
		Indent0( nDepth );
		printf( "    mov     eax, [eax]\n" );
					// そのアドレスからデータをロードして
		Indent0( nDepth );
		printf( "    push    eax\n" );
					// スタックに積む
		return 0;
		}
	case ND_ASSIGN:	// =
		int		rslt;
		if( node->lhs != NULL ){	// 代入先のアドレス
			rslt = gen_lval( node->lhs, nDepth+1 );
			if( rslt != 0 )		return rslt;
		}
		if( node->rhs != NULL ){	// 代入するデータ
			rslt = gen( node->rhs, nDepth+1 );
			if( rslt != 0 )		return rslt;
		}
		Indent0( nDepth );
		printf( "    pop     edi\n" );	// 代入するデータ
		Indent0( nDepth );
		printf( "    pop     eax\n" );	// 代入先のアドレス
		Indent0( nDepth );
		printf( "    mov     [eax], edi\n" );	// 代入
		Indent0( nDepth );
		printf( "    push    edi\n" );	// 式の値をプッシュ
		return 0;
	}// switch

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
	case ND_SML:	// <
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setl    al\n" );	// 直前のcmpが < の時にalに1をセット
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// 上位ビットをゼロに
		break;
	case ND_BIG:	// >  ->  ! <=
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setle   al\n" );	// 直前のcmpが <= の時にalに1をセット
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// 上位ビットをゼロに
		Indent0( nDepth );
		printf( "    xor     eax, 1\n" );
		break;
	case ND_SME:	// <=
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setle   al\n" );	// 直前のcmpが <= の時にalに1をセット
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// 上位ビットをゼロに
		break;
	case ND_BGE:	// >=  ->  ! <
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setl    al\n" );	// 直前のcmpが < の時にalに1をセット
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// 上位ビットをゼロに
		Indent0( nDepth );
		printf( "    xor     eax, 1\n" );
		break;
	case ND_EQU:
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    sete    al\n" );	// 直前のcmpが等しい時にalに1をセット
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// 上位ビットをゼロに
		break;
	case ND_NEQ:
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setne    al\n" );	// 直前のcmpが等しくない時にalに1
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// 上位ビットをゼロに
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

	nLastError = gen( node, 0 );

	return nLastError;

}
//int GenAsm ( Node * node )



// 構文木からアセンブラコードを生成する GenerateAsmCode()		//TAG_JUMP_MARK
//	機械的にスタックマシンとしての x86コードを生成。
int GenerateAsmCode ( )
{

	nLastError = 0;

	printf( "\n\n\n" );
	printf( ".386\n" );
	printf( ".model  flat, stdcall\n" );
	printf( "option  casemap: none\n" );
	printf( "\n\n\n" );
	printf( ".code\n" );
	printf( "start:\n" );
	printf( "\n\n\n" );

	// プロローグ
	//	変数26個分の領域を確保する
	printf( "    push    ebp\n" );		// BPを退避
	printf( "    mov     ebp, esp\n" );
	printf( "    sub     esp, 208\n" );	// 8 * 26 = 208
	printf( "\n\n\n" );

	// 先頭の式から順にコードを生成
	int		i = 0;
	for( ; NodeTop[i] != NULL; ++i ){
		printf( ";----------------------------------\n" );
		GenAsm( NodeTop[i] );
		printf( "    pop     eax\n" );	// 積まれた式の値をポップ
	}

	// エピローグ
	//	SPを元に戻す
	printf( "\n\n\n" );
	printf( ";----------------------------------\n" );
	printf( "    mov     esp, ebp\n" );	// SPを元に戻す
	printf( "    pop     ebp\n" );		// BPを復帰

	printf( "    ret\n" );
	printf( "\n\n\n" );
	printf( "end start\n" );
	printf( "\n\n\n" );

	return nLastError;

}
//int GenerateAsmCode ( )



// end of this file : gen.c
