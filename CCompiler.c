


//	�ċA���~�\����͖@ recursive descent parsing
//	32�r�b�g�p



#include	<stdio.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<stdarg.h>
#include	<stdbool.h>
#include	<string.h>



// �g�[�N���̎�� TokenKind										//TAG?JUMP_MARK
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
	char *		str;	// �g�[�N��������

};

// ���ݒ��ڂ��Ă���g�[�N��
Token *	token;



// �G���[��񍐂��邽�߂̊֐� error()							//TAG_JUMP_MARK
//	printf()�Ɠ��������B
void error ( int nErrCode, int nSubCode, char * fmt, ... )
{

	va_list	ap;
	va_start( ap, fmt );

	fprintf( stderr, "error %d (%d) : ", nErrCode, nSubCode );
	vfprintf( stderr, fmt, ap );
	fprintf( stderr, "\n" );
	exit( nErrCode );

}
//void error ( char * fmt, ... )



// consume()													//TAG_JUMP_MARK
//	���̃g�[�N��token�����҂��Ă���L���̎��͐^��Ԃ��B�łȂ���΋U��Ԃ��B
//	�g�[�N����ǂݐi�߂�B
bool consume ( char op )
{

	if(  ( token->kind != TK_RESERVED )||( token->str[0] != op )  )
		return false;

	token = token->next;	// ���̃g�[�N����

	return true;

}
//bool consume ( char op )



// expect()														//TAG_JUMP_MARK
//	���̃g�[�N��token�����҂��Ă���L���̎��̓g�[�N�����P�ǂݐi�߂�B
//	�łȂ���΃G���[��񍐂���B
void expect ( char op, int nErrCode )
{

	if(  ( token->kind != TK_RESERVED )||( token->str[0] != op )  )
		error( nErrCode, 100, "'%c'�ł͂���܂���B", op );

	token = token->next;	// ���̃g�[�N����

}
//void expect ( char op, int nErrCode )



// expect_number()												//TAG_JUMP_MARK
//	���̃g�[�N��token�����l�̏ꍇ�A�g�[�N�����P�ǂݐi�߂āA���̐��l��Ԃ��B
//	�łȂ���΃G���[��񍐂���B
int expect_number ( int nErrCode )
{

	if( token->kind != TK_NUM )
		error( nErrCode, 100, "���l�ł͂���܂���B" );

	int	val = token->val;
	token = token->next;	// ���̃g�[�N����

	return val;

}
//int expect_number ( int nErrCode )



// at_eof()														//TAG_JUMP_MARK
//	���̃g�[�N��token�����̓g�[�N���̏I���Ȃ�^��Ԃ��B
bool at_eof ()
{

	return token->kind == TK_EOF ;

}
//bool at_eof ()



// new_token()													//TAG_JUMP_MARK
//	�V�����g�[�N�����쐬���āA���Ă����g�[�N��cur�̌�Ɍq����B
//	�쐬�����g�[�N���̃A�h���X��Ԃ��B
//	�Ăяo���������������J������B
Token * new_token ( TokenKind kind, Token * cur, char * str )
{

	Token * tok = (Token*)calloc( 1, sizeof(Token) );	// �[���N���A����
	if( tok == NULL )	return NULL;

	tok->kind = kind;
	tok->str = str;

	// cur�̎��ɍ쐬�����g�[�N�����q����B
	cur->next = tok;

	return tok;

}
//Token * new_token ( TokenKind kind, Token * cur, char * str )



// delete_list()												//TAG_JUMP_MARK
void delete_list ( Token * pList )
{

	Token *	p = pList;
	while( p != NULL ){
		Token *	mp = p->next;
		free( p );
		p = mp;
	}

}
//void delete_list ( Token * pList )



// tokenize()													//TAG_JUMP_MARK
//	�w�肵����������g�[�N���ɕ������ă��X�g���쐬���A
//	���X�g�̐擪�̃A�h���X��Ԃ��B
Token * tokenize ( char * pStr, int nErrCode )
{

	char *	p = pStr;

	Token	head;		// ���X�g�̐擪�A�h���X�m�ۗp
	head.next = NULL;

	Token *	cur = &head;	// ���݂̍Ō���̗v�f

	if( pStr == NULL )		return NULL;

	while ( *p ) {

		// �󔒕������X�L�b�v
		if( isspace( *p ) ) {  ++p;  continue;  }

		// + or -
		if(  ( *p == '+' )||( *p == '-' )  ){
			cur = new_token( TK_RESERVED, cur, p++ );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 100, "�������s���ł��B" );
			}
			continue;
		}

		// ���l
		if( isdigit( *p ) ) {
			cur = new_token( TK_NUM, cur, p );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 200, "�������s���ł��B" );
			}
			cur->val = strtol( p, &p, 10 );	// p�ɂ͎��̃A�h���X������
			continue;
		}

		error( nErrCode, 100, "���@���Ⴂ�܂��B(%d�o�C�g��)", p-pStr );

	}// while *p

	// �I�[
	cur = new_token( TK_EOF, cur, p );
	if( cur == NULL ){
		delete_list( head.next );
		error( nErrCode, 900, "�������s���ł��B" );
	}

	return head.next;

}
//Token * tokenize ( char * pStr, int nErrCode )



int main ( int argc, char **argv )
{

	if( argc !=2 ){
		fprintf( stderr, "�����̌�������������܂���B\n" );
		return 100;
	}

	// ��������g�[�N���̃��X�g�����
	Token *	mToken = token = tokenize( argv[1], 1000 );

	printf( "\n\n\n" );
	printf( ".386\n" );
	printf( ".model  flat, stdcall\n" );
	printf( "option  casemap: none\n" );
	printf( "\n\n\n" );
	printf( ".code\n" );
	printf( "start:\n" );
	printf( "\n\n\n" );

	// �m�C�̍ŏ��͐��l�łȂ���΂Ȃ�Ȃ��̂ŁA������`�F�b�N���o��
	printf(  "    mov     eax, %ld\n",  expect_number( 2100 )  );

	while( ! at_eof() ){

		if( consume( '+' ) ){
			printf(  "    add     eax, %ld\n",  expect_number( 2200 )  );
		}

		expect( '-', 2300 );
		printf(  "    sub     eax, %ld\n",  expect_number( 2400 )  );

	} //while *pCmd

	printf( "    ret\n" );
	printf( "\n\n\n" );
	printf( "end start\n" );
	printf( "\n\n\n" );

	delete_list( mToken );

	return 0;

}



