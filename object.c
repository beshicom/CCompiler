


// ユーティリティ



#include	"object.h"



int		nLastError = 0;	// 直近のエラーコード



void Indent ( int n )											//TAG_JUMP_MARK
{
	for( int i=0; i<n; ++i ){
		char * p = ". ";
		if( i % 2 )	p = "  ";
		printf( p );
	}
}



int getline ( char * pBuf, int nChar )							//TAG_JUMP_MARK
{

	int		c, n;

	for( n=0; (c=getchar())!='\n'; ++n )	pBuf[n] = c;
	pBuf[n] = 0;

	return n;

}



// end of this file : object.c
