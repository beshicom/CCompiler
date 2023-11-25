


//	�ċA���~�\����͖@ recursive descent parsing
//	32�r�b�g�p



#include	<stdio.h>
#include	<stdlib.h>



int main ( int argc, char **argv )
{

	if( argc !=2 ){
		fprintf( stderr, "�����̌�������������܂���B\n" );
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
		// �^����ꂽ�������10�i���Ƃ݂Ȃ��Đ��l���A���̃A�h���X���Z�b�g����

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
			fprintf( stderr, "�\�����Ȃ������ł��B: '%c'\n", *pCmd );
			return 1100;
		}// switch

	} //while *pCmd

	printf( "    ret\n" );
	printf( "\n\n\n" );
	printf( "end start\n" );
	printf( "\n\n\n" );

	return 0;

}



