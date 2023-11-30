


//	�ċA���~�\����͖@ recursive descent parsing
//	32�r�b�g�p



#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<stdarg.h>
#include	<stdbool.h>
#include	<string.h>

#include	"token.h"
#include	"parse.h"
#include	"gen.h"



// main()														//TAG_JUMP_MARK
int main ( int argc, char **argv )
{

	if(  ( argc != 2 )&&( argc != 3 )  ){
		fprintf( stderr, "�����̌�������������܂���B\n" );
		fprintf( stderr, "Usage : CCompiler �\�[�X������ [1|2|3]\n" );
		fprintf( stderr, "\t1 (�K��l) : �A�Z���u���\�[�X�\��\n" );
		fprintf( stderr, "\t2          : �\���ؕ\��\n" );
		fprintf( stderr, "\t3          : �A�Z���u���\�[�X�\���A�\���ؕ\��\n" );
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

	// ��������g�[�N���̃��X�g�����
	user_input = argv[1];
	Token *	mToken = token = tokenize( argv[1], 1000 );

	// �g�[�N���̃��X�g����\���؂����
	Parse();

	if( fPrint ){
		printf( "%s = %d\n", argv[1], Calc( NodeTop ) );
		Print();
		printf( "push return -------------" );
		getchar();
		printf( "\n" );
	}

	if( ! fAsm ){
		delete_list( mToken );
		delete_tree( NodeTop );
		return 0;
	}

	// x86 �A�Z���u���R�[�h�̐���
	GenAsm( NodeTop );

	delete_list( mToken );
	delete_tree( NodeTop );

	return 0;

}



