


// ���[�e�B���e�B



#include	"object.h"



int		nLastError = 0;	// ���߂̃G���[�R�[�h



// n�̃X�y�[�X��\�� Indent()									//TAG_JUMP_MARK
void Indent ( int n )
{
	for( int i=0; i<n; ++i ){
		char * p = ". ";
		if( i % 2 )	p = "  ";
		printf( p );
	}
}



// n�̃X�y�[�X��\�� Indent0()								//TAG_JUMP_MARK
void Indent0 ( int n )
{
	for( int i=0; i<n; ++i ){
		char * p = "  ";
		if( i % 2 )	p = "  ";
		printf( p );
	}
}



// �L�[�{�[�h����P�s���͂��� getline()							//TAG_JUMP_MARK
int getline ( char * pBuf, int nChar )
{

	int		c, n;

	for( n=0; (c=getchar())!='\n'; ++n )	pBuf[n] = c;
	pBuf[n] = 0;

	return n;

}



// end of this file : object.c
