


// ��������g�[�N���ɕ�����



#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<stdbool.h>
#include	<string.h>

#include	"object.h"



#ifndef	B__TOKEN_H__B
#define	B__TOKEN_H__B



extern char *	user_input;		// ��������v���O�����̕�����S�̂̐擪



// �g�[�N���̎�� TokenKind										//TAG_JUMP_MARK
typedef enum {

	TK_RESERVED,	// �L��
	TK_NUM,			// ����
	TK_EOF			// ���͂̏I���

} TokenKind;

typedef	struct Token	Token;

// �g�[�N���^ Token												//TAG_JUMP_MARK
struct Token {

	TokenKind	kind;	// �g�[�N���̌^
	Token *		next;	// ���̓��̓g�[�N��
	int			val;	// kind��TK_NUM�̎�
	char *		str;	// �g�[�N��������̐擪

};

// ���ݒ��ڂ��Ă���g�[�N��
extern Token *	token;



// consume()													//TAG_JUMP_MARK
//	���̃g�[�N��token�����҂��Ă���L���̎��͐^��Ԃ��B�łȂ���΋U��Ԃ��B
//	�g�[�N����ǂݐi�߂�B
bool consume ( char op );



// expect()														//TAG_JUMP_MARK
//	���̃g�[�N��token�����҂��Ă���L���̎��̓g�[�N�����P�ǂݐi�߂�B
//	�łȂ���΃G���[��񍐂���B
int expect ( char op, int nErrCode );



// expect_number()												//TAG_JUMP_MARK
//	���̃g�[�N��token�����l�̏ꍇ�A�g�[�N�����P�ǂݐi�߂āA���̐��l��Ԃ��B
//	�łȂ���΃G���[��񍐂���B
int expect_number ( int nErrCode );



// at_eof()														//TAG_JUMP_MARK
//	���̃g�[�N��token�����̓g�[�N���̏I���Ȃ�^��Ԃ��B
bool at_eof ();



// tokenize()													//TAG_JUMP_MARK
//	�w�肵����������g�[�N���ɕ������ă��X�g���쐬���A
//	���X�g�̐擪�̃A�h���X��Ԃ��B
Token * tokenize ( char * pStr, int nErrCode );



// delete_list()												//TAG_JUMP_MARK
//	�g�[�N���̃��X�g��S�ĊJ������B
void delete_list ( Token * pList );



#endif



// end of this file : token.h
