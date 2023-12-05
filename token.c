


// ��������g�[�N���ɕ�����



#include	"token.h"



char *	user_input;		// ��������v���O�����̕�����S�̂̐擪



// ���ݒ��ڂ��Ă���g�[�N��
Token *	token;



// �G���[��񍐂��邽�߂̊֐� error()							//TAG_JUMP_MARK
//	printf()�Ɠ��������B
int error ( int nErrCode, int nSubCode, char * fmt, ... )
{

	va_list	ap;
	va_start( ap, fmt );

	fprintf( stderr, "error %d (%d) : ", nErrCode, nSubCode );
	vfprintf( stderr, fmt, ap );
	fprintf( stderr, "\n" );
	exit( nErrCode );

	nLastError = nErrCode;

	return nErrCode;

}
//int error ( char * fmt, ... )



// �G���[��񍐂��邽�߂̊֐� error_at()						//TAG_JUMP_MARK
//	printf()�Ɠ��������B
int error_at( int nErrCode, int nSubCode, char * loc, char * fmt, ... )
{

	va_list		ap;
	va_start( ap, fmt );

	int	pos = loc - user_input;

	// 1+2-a
	//     ^ ���@���Ⴂ�܂�

	fprintf( stderr, "%s\n", user_input );
	fprintf( stderr, "%*s", pos, " " );	// pos�̃X�y�[�X
	fprintf( stderr, "^ " );

	char	buf[ 100 ];
	vsprintf( buf, fmt, ap );
	strcat( buf, "\n" );

	fprintf( stderr, buf );

	error ( nErrCode, nSubCode, buf );

	nLastError = nErrCode;

	return nErrCode;

}
//int error_at( int nErrCode, int nSubCode, char *p loc, char * fmt, ... )



// consume()													//TAG_JUMP_MARK
//	���̃g�[�N��token�����҂��Ă���L���̎��͐^��Ԃ��B�łȂ���΋U��Ԃ��B
//	�g�[�N����ǂݐi�߂�B
bool consume ( char * op )
{

	nLastError = 0;

	if( op == NULL ){
		nLastError = 3;
		return false;
	}

	if(
		( token->kind != TK_RESERVED )||
		( strlen(op) != token->len )||
		memcmp( token->str, op, token->len )
	){
		nLastError = 1;
		return false;
	}

	token = token->next;	// ���̃g�[�N����

	return true;

}
//bool consume ( char * op )



// expect()														//TAG_JUMP_MARK
//	���̃g�[�N��token�����҂��Ă���L���̎��̓g�[�N�����P�ǂݐi�߂�B
//	�łȂ���΃G���[��񍐂���B
int expect ( char * op, int nErrCode )
{

	nLastError = 0;

	if(
		( token->kind != TK_RESERVED )||
		( strlen( op ) != token->len )||
		memcmp( token->str, op, token->len )
	){
		error_at( nErrCode, 100, token->str, "'%s'�ł͂���܂���B", op );
		nLastError = nErrCode;
		return nErrCode;
	}

	token = token->next;	// ���̃g�[�N����

	return 0;

}
//int expect ( char * op, int nErrCode )



// expect_number()												//TAG_JUMP_MARK
//	���̃g�[�N��token�����l�̏ꍇ�A�g�[�N�����P�ǂݐi�߂āA���̐��l��Ԃ��B
//	�łȂ���΃G���[��񍐂���B
int expect_number ( int nErrCode )
{

	nLastError = 0;

	if( token->kind != TK_NUM ){
		error_at( nErrCode, 100, token->str, "���l�ł͂���܂���B" );
		nLastError = nErrCode;
	}

	int	val = token->val;
	token = token->next;	// ���̃g�[�N����

	return val;

}
//int expect_number ( int nErrCode )



// at_eof()														//TAG_JUMP_MARK
//	���̃g�[�N��token�����̓g�[�N���̏I���Ȃ�^��Ԃ��B
bool at_eof ()
{

	nLastError = 0;

	bool	f = ( token->kind == TK_EOF ) ;
	if( f )	nLastError = 1;

	return f;

}
//bool at_eof ()



// new_token()													//TAG_JUMP_MARK
//	�V�����g�[�N�����쐬���āA���Ă����g�[�N��cur�̌�Ɍq����B
//	�쐬�����g�[�N���̃A�h���X��Ԃ��B
//	�Ăяo���������������J������B
Token * new_token ( TokenKind kind, Token * cur, char * str )
{

	nLastError = 0;

	Token * tok = (Token*)calloc( 1, sizeof(Token) );	// �[���N���A����
	if( tok == NULL ){
		nLastError = 1;
		return NULL;
	}

	tok->kind = kind;  tok->len = 1;
	if( kind == TK_RESERVED2 ){  tok->kind = TK_RESERVED;  tok->len = 2;  }
	tok->str = str;

	// cur�̎��ɍ쐬�����g�[�N�����q����B
	cur->next = tok;

	return tok;

}
//Token * new_token ( TokenKind kind, Token * cur, char * str )



// delete_list()												//TAG_JUMP_MARK
//	�g�[�N���̃��X�g��S�ĊJ������B
void delete_list ( Token * pList )
{

	nLastError = 0;

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

	nLastError = 0;

	char *	p = pStr;

	Token	head;		// ���X�g�̐擪�A�h���X�m�ۗp
	head.next = NULL;

	Token *	cur = &head;	// ���݂̍Ō���̗v�f

	if( pStr == NULL ){
		nLastError = 1;
		return NULL;
	}

	while ( *p ) {

		// �󔒕������X�L�b�v
		if( isspace( *p ) ) {  ++p;  continue;  }

		// �L��
		{
		char *	key[] = { "==", "!=", "<=", ">=", NULL };
		int		i = 0;
		for( ; key[i] != NULL; ++i ){
			if( memcmp( key[i], p, 2 ) == 0 ){
				cur = new_token( TK_RESERVED2, cur, p );
				p += 2;
				if( cur == NULL ){
					delete_list( head.next );
					error( nErrCode, 300, "�������s���ł��B" );
					nLastError = 300;
					return head.next;
				}// if
				continue;
			}// if
		}// for
		}// end

		// �L��
		{
		char	key[] = "+-*/()<>";
		if( strchr( key, *p ) != NULL ){
			cur = new_token( TK_RESERVED, cur, p++ );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 100, "�������s���ł��B" );
				nLastError = 100;
				return head.next;
			}
			cur->len = 1;
			continue;
		}
		}// end

		/*
		// + or -
		if(  ( *p == '+' )||( *p == '-' )  ){
			cur = new_token( TK_RESERVED, cur, p++ );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 100, "�������s���ł��B" );
				nLastError = 100;
				return head.next;
			}
			continue;
		}
		*/

		// ���l
		if( isdigit( *p ) ) {
			cur = new_token( TK_NUM, cur, p );
			if( cur == NULL ){
				delete_list( head.next );
				error( nErrCode, 200, "�������s���ł��B" );
				nLastError = 200;
				return head.next;
			}
			cur->val = strtol( p, &p, 10 );	// p�ɂ͎��̃A�h���X������
			continue;
		}

		error_at( nErrCode, 100, p, "���@���Ⴂ�܂��B(%d�o�C�g��)", p-pStr );

	}// while *p

	// �I�[
	cur = new_token( TK_EOF, cur, p );
	if( cur == NULL ){
		delete_list( head.next );
		error( nErrCode, 900, "�������s���ł��B" );
		nLastError = 900;
		return head.next;
	}

	return head.next;

}
//Token * tokenize ( char * pStr, int nErrCode )



// end of this file : token.c
