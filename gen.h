


// �\���؂���A�Z���u���R�[�h�𐶐�



#include	<stdio.h>
#include	<malloc.h>

#include	"object.h"
#include	"token.h"
#include	"parse.h"



#ifndef	B__GEN_H__B
#define	B__GEN_H__B



// �\���؂���A�Z���u���R�[�h�𐶐����� gen()					//TAG_JUMP_MARK
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��Ă� x86�R�[�h�𐶐��B
int gen ( Node * node, int nDepth );



// �\���؂���A�Z���u���R�[�h�𐶐����� GenAsm()				//TAG_JUMP_MARK
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��Ă� x86�R�[�h�𐶐��B
int GenAsm ( Node * node );



// �\���؂���A�Z���u���R�[�h�𐶐����� GenerateAsmCode()		//TAG_JUMP_MARK
//	�@�B�I�ɃX�^�b�N�}�V���Ƃ��Ă� x86�R�[�h�𐶐��B
int GenerateAsmCode ( );



#endif



// end of this file : gen.h
