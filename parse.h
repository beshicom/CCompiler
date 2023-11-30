


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

	ND_ADD,		// +
	ND_SUB,		// -
	ND_MUL,		// *
	ND_DIV,		// /
	ND_NUM,		// ����

	ND_END		// (�I�[�q)

} NodeKind ;



typedef	struct Node	Node;

// ���ۍ\���؂̃m�[�h�̌^ Node									//TAG_JUMP_MARK
struct Node
{

	NodeKind	kind;		// �m�[�h�̌^
	Node *		lhs;		// ���� left hand side
	Node *		rhs;		// �E�� roight hand side
	int			val;		// kind==ND_NUM�̎��̂ݎg��

	int			nErr;		// �G���[������������not0
	int			nBlockID;	// �������[�u���b�N�̒ʂ��ԍ�

};

extern Node *	NodeTop;	// �\���؂̐擪

extern int		nNumBlock;	// �������[�u���b�N�̑���



// �g�|�N���̃��X�g�����ƂɁA�\���؂��쐬���� Parse()			//TAG_JUMP_MARK
//	�O���[�o���ϐ� token : �g�[�N�����X�g
int Parse ();



// �\���؂��v�����g���� Print()									//TAG_JUMP_MARK
//	�O���[�o���ϐ� NodeTop : �\����
int Print ();



#endif



// end of this file : parse.h
