


// �ċA���~�\�����



#include	<stdio.h>
#include	<malloc.h>

#include	"object.h"
#include	"token.h"



#ifndef	B__PARSE_H__B
#define	B__PARSE_H__B



// ���ۍ\���؂̃m�[�h�̎�� NodeKind							//TAG_JUMP_MARK
typedef enum
{

	ND_START,

	ND_NUM,		// ����
	ND_LCVAR,	// ���[�J���ϐ�

	ND_ASSIGN,	// =

	ND_ADD,		// +
	ND_SUB,		// -

	ND_MUL,		// *
	ND_DIV,		// /

	ND_SML,		// <
	ND_SME,		// <=
	ND_BIG,		// >
	ND_BGE,		// >=

	ND_EQU,		// ==
	ND_NEQ,		// !=

	ND_END

} NodeKind ;



typedef	struct Node	Node;

// ���ۍ\���؂̃m�[�h�̌^ Node									//TAG_JUMP_MARK
struct Node
{

	NodeKind	kind;		// �m�[�h�̌^
	Node *		lhs;		// ���� left hand side
	Node *		rhs;		// �E�� roight hand side
	int			val;		// kind==ND_NUM�̎��̂ݎg��
	int			offset;		// kind==ND_LCVAR�̎��̂ݎg��
							// BP����̃I�t�Z�b�g�l

	int			nErr;		// �G���[������������not0
	int			nBlockID;	// �������[�u���b�N�̒ʂ��ԍ�

};

#define	nMaxNode	10000	// �v���O������statement�̐�
extern Node *	NodeTop[ nMaxNode ];	// �\���؂̐擪

extern int		nNumBlock;	// �������[�u���b�N�̑���



// �g�|�N���̃��X�g�����ƂɁA�\���؂��쐬���� Parse()			//TAG_JUMP_MARK
//	�O���[�o���ϐ� token : �g�[�N�����X�g
int Parse ();



// �\���؂��v�����g���� Print()									//TAG_JUMP_MARK
//	�O���[�o���ϐ� NodeTop : �\����
int Print ();



#endif



// end of this file : parse.h
