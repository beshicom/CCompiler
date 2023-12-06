


// �\���؂���A�Z���u���R�[�h�𐶐�
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��ẴR�[�h�𐶐��B



#include	"gen.h"



// �\���؂���A�Z���u���R�[�h�𐶐����� gen_lval()				//TAG_JUMP_MARK
//	���Ӓl�̃R�[�h�̐����B
//	�ϐ��̃A�h���X���v�Z���ăX�^�b�N��push����B
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��Ă� x86�R�[�h�𐶐��B
int gen_lval ( Node * node, int nDepth )
{

	nLastError = 0;

	if( node == NULL ){
		nLastError = 2;
		return 2;
	}

	if( node->kind != ND_LCVAR ){
		error( 10000, 100, "����̍��Ӓl���ϐ��ł͂���܂���B" );
	}

	Indent0( nDepth );
	printf( "    mov     eax, ebp\n" );
	printf( "    sub     eax, %d\n", node->offset );
	printf( "    push    eax\n" );

	return 0;

}
//int gen_lval ( Node * node, int nDepth )



// �\���؂���A�Z���u���R�[�h�𐶐����� gen()					//TAG_JUMP_MARK
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��Ă� x86�R�[�h�𐶐��B
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
	case ND_NUM:	// ���l
		Indent0( nDepth );
		printf( "    push    %d\n", node->val );
		return 0;
	case ND_LCVAR:	// ���[�J���ϐ�
		{
		int		rslt = gen_lval( node, nDepth+1 );
		if( rslt != 0 )		return rslt;
		Indent0( nDepth );
		printf( "    pop     eax\n" );
					// �X�^�b�N�ɐς܂ꂽ���[�J���ϐ��̃A�h���X�����o��
		Indent0( nDepth );
		printf( "    mov     eax, [eax]\n" );
					// ���̃A�h���X����f�[�^�����[�h����
		Indent0( nDepth );
		printf( "    push    eax\n" );
					// �X�^�b�N�ɐς�
		return 0;
		}
	case ND_ASSIGN:	// =
		int		rslt;
		if( node->lhs != NULL ){	// �����̃A�h���X
			rslt = gen_lval( node->lhs, nDepth+1 );
			if( rslt != 0 )		return rslt;
		}
		if( node->rhs != NULL ){	// �������f�[�^
			rslt = gen( node->rhs, nDepth+1 );
			if( rslt != 0 )		return rslt;
		}
		Indent0( nDepth );
		printf( "    pop     edi\n" );	// �������f�[�^
		Indent0( nDepth );
		printf( "    pop     eax\n" );	// �����̃A�h���X
		Indent0( nDepth );
		printf( "    mov     [eax], edi\n" );	// ���
		Indent0( nDepth );
		printf( "    push    edi\n" );	// ���̒l���v�b�V��
		return 0;
	}// switch

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
	case ND_SML:	// <
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setl    al\n" );	// ���O��cmp�� < �̎���al��1���Z�b�g
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// ��ʃr�b�g���[����
		break;
	case ND_BIG:	// >  ->  ! <=
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setle   al\n" );	// ���O��cmp�� <= �̎���al��1���Z�b�g
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// ��ʃr�b�g���[����
		Indent0( nDepth );
		printf( "    xor     eax, 1\n" );
		break;
	case ND_SME:	// <=
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setle   al\n" );	// ���O��cmp�� <= �̎���al��1���Z�b�g
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// ��ʃr�b�g���[����
		break;
	case ND_BGE:	// >=  ->  ! <
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setl    al\n" );	// ���O��cmp�� < �̎���al��1���Z�b�g
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// ��ʃr�b�g���[����
		Indent0( nDepth );
		printf( "    xor     eax, 1\n" );
		break;
	case ND_EQU:
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    sete    al\n" );	// ���O��cmp������������al��1���Z�b�g
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// ��ʃr�b�g���[����
		break;
	case ND_NEQ:
		Indent0( nDepth );
		printf( "    cmp     eax, edi\n" );
		Indent0( nDepth );
		printf( "    setne    al\n" );	// ���O��cmp���������Ȃ�����al��1
		Indent0( nDepth );
		printf( "    movzx   eax, al\n" );	// ��ʃr�b�g���[����
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

	nLastError = gen( node, 0 );

	return nLastError;

}
//int GenAsm ( Node * node )



// �\���؂���A�Z���u���R�[�h�𐶐����� GenerateAsmCode()		//TAG_JUMP_MARK
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��Ă� x86�R�[�h�𐶐��B
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

	// �v�����[�O
	//	�ϐ�26���̗̈���m�ۂ���
	printf( "    push    ebp\n" );		// BP��ޔ�
	printf( "    mov     ebp, esp\n" );
	printf( "    sub     esp, 208\n" );	// 8 * 26 = 208
	printf( "\n\n\n" );

	// �擪�̎����珇�ɃR�[�h�𐶐�
	int		i = 0;
	for( ; NodeTop[i] != NULL; ++i ){
		printf( ";----------------------------------\n" );
		GenAsm( NodeTop[i] );
		printf( "    pop     eax\n" );	// �ς܂ꂽ���̒l���|�b�v
	}

	// �G�s���[�O
	//	SP�����ɖ߂�
	printf( "\n\n\n" );
	printf( ";----------------------------------\n" );
	printf( "    mov     esp, ebp\n" );	// SP�����ɖ߂�
	printf( "    pop     ebp\n" );		// BP�𕜋A

	printf( "    ret\n" );
	printf( "\n\n\n" );
	printf( "end start\n" );
	printf( "\n\n\n" );

	return nLastError;

}
//int GenerateAsmCode ( )



// end of this file : gen.c
