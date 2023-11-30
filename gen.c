


// �\���؂���A�Z���u���R�[�h�𐶐�
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��ẴR�[�h�𐶐��B


#include	"gen.h"



// �\���؂���A�Z���u���R�[�h�𐶐����� gen()					//TAG_JUMP_MARK
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��Ă� x86�R�[�h�𐶐��B
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

	// ���Ӓl�̐���
	//Indent0( nDepth+1 );
	//printf( "    ; left hand side\n" );
	nLastError = gen( node->lhs, nDepth+1 );
	if( nLastError != 0 )	return nLastError;

	// �E�Ӓl�̐���
	//Indent0( nDepth+1 );
	//printf( "    ; right hand side\n" );
	nLastError = gen( node->rhs, nDepth+1 );
	if( nLastError != 0 )	return nLastError;

	// �v�Z�R�[�h�̐���
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
			// eax��64�r�b�g�l�Ɋg�����A���32�r�b�g��edx�֓����B
		Indent0( nDepth );
		printf( "    idiv    edi\n" );
			//edx eax ��64�r�b�g�����Ƃ݂Ȃ���edi�ŕ����t�����Z������
			//	����eax�A�]���edx�ɓ����B
		break;
	}// switch

	// �v�Z���ʂ��X�^�b�N�֐ςށB
	Indent0( nDepth );
	printf( "    push    eax\n" );
	printf( "\n\n" );

	return 0;

}
//int gen ( Node * node )



// �\���؂���A�Z���u���R�[�h�𐶐����� gen()					//TAG_JUMP_MARK
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��Ă� x86�R�[�h�𐶐��B
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
