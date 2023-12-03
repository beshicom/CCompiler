


// �ċA���~�\�����



#include	"parse.h"
#include	"token.h"



Node *	NodeTop = NULL;	// �\���؂̐擪

int		nNumBlock = 0;	// �������[�u���b�N�̑���



// �V�����m�[�h���쐬���� new_node()							//TAG_JUMP_MARK
//	�Q�����Z�q�̃m�[�h���쐬����B
Node * new_node ( NodeKind kind, Node * lhs, Node * rhs )
{

	nLastError = 0;

	Node *	node = (Node*)calloc( 1, sizeof(Node) );
	if( node == NULL ){
		nLastError = 1;
		return NULL;
	}

	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	node->val = 0;

	node->nErr = 0;
	node->nBlockID = nNumBlock++;	// �ʂ��ԍ�

	return node;

}
//Node * new_node ( NodeKind kind, Node * lhs, Node * rhs )



// �V�����m�[�h���쐬���� new_node_num()						//TAG_JUMP_MARK
//	���l�̃m�[�h���쐬����B
Node * new_node_num ( int val )
{

	nLastError = 0;

	Node *	node = (Node*)calloc( 1, sizeof(Node) );
	if( node == NULL ){
		nLastError = 2;
		return NULL;
	}

	node->kind = ND_NUM;
	node->lhs = NULL;
	node->rhs = NULL;
	node->val = val;

	node->nErr = 0;
	node->nBlockID = nNumBlock++;	// �ʂ��ԍ�

	return node;

}
//Node * new_node_num ( int val )



// delete_tree()												//TAG_JUMP_MARK
void delete_tree ( Node * pNode )
{

	nLastError = 0;

	if( pNode == NULL )		return;

	if( pNode->rhs != NULL )	delete_tree( pNode->rhs );
	if( pNode->lhs != NULL )	delete_tree( pNode->lhs );

	free( pNode );

}
//void delete_tree ( Node * pNode )



Node * expr();



// ������()���p�[�X���� primary()								//TAG_JUMP_MARK
//	primary = '(' expr ')' | num
Node * primary ()
{

	nLastError = 0;

	Node *	node;

	// ���̃g�[�N���� '(' �Ȃ�A'(' expr ')' �łȂ���΂Ȃ�Ȃ��B
	if( consume( '(' ) ){

		node = expr();
		if( node == NULL ){
			//nLastError = 2;
			return NULL;
		}
		nLastError = expect( ')', 1000 );
		return node;

	}

	// �����łȂ���ΐ��l�łȂ���΂Ȃ�Ȃ��B
	node = new_node_num( expect_number( 1100 ) );
	if( node == NULL ){
		nLastError = 2;
		return NULL;
	}

	return node;

}
//Node * primary ()



// �P���� + �� - ���p�[�X����B unary()							//TAG_JUMP_MARK
//	unary = ( "+" | "-" )? primary
//
Node * unary ()
{

	nLastError = 0;

	// +x -> x
	if( consume('+') )	return primary();

	// -x -> 0-x
	if( consume('-') ){
		Node *	node = new_node( ND_SUB, new_node_num(0), primary() );
		if( node == NULL ){
			nLastError = 2;
			return primary();
		}
		return node;
	}

	return primary();

}
//Node * unary ()



// * �� / ���p�[�X���� mul()									//TAG_JUMP_MARK
//	mul = unary ( "*" unary | "/" unary )*
//	* �� / �͍������̉��Z�q�Blhs���[���Ȃ�B
Node * mul ()
{

	nLastError = 0;

	Node *	node = unary();
	if( node == NULL ){
		nLastError = 1;
		return NULL;
	}
	Node *	mnd = node;

	for(;;){

		if( consume( '*' ) ){
			mnd = node;
			node = new_node( ND_MUL, node, unary() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		if( consume( '/' ) ){
			mnd = node;
			node = new_node( ND_DIV, node, unary() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		break;

	}// for(;;)

	return node;

}
//Node * mul ()



// + �� - ���p�[�X���� expr()									//TAG_JUMP_MARK
//	expr = mul ( "+" mul | "-" mul )*
//	+ �� - �͍������̉��Z�q�Blhs���[���Ȃ�B
Node * expr ()
{

	nLastError = 0;

	Node *	node = mul();
	if( node == NULL ){
		nLastError = 1;
		return NULL;
	}
	Node *	mnd = node;

	for(;;){

		if( consume( '+' ) ){
			mnd = node;
			node = new_node( ND_ADD, node, mul() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		if( consume( '-' ) ){
			mnd = node;
			node = new_node( ND_SUB, node, mul() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		break;

	}// for(;;)

	return node;

}
//Node * expr ()



// �g�|�N���̃��X�g�����ƂɁA�\���؂��쐬���� Parse()			//TAG_JUMP_MARK
//	�O���[�o���ϐ� token : �g�[�N�����X�g
int Parse ()
{

	nNumBlock = 0;
	nLastError = 0;

	NodeTop = expr();

	return 0;

}



int Calc ( Node * node )										//TAG_JUMP_MARK
{

	nLastError = 0;

	if( node == NULL ){
		nLastError = 100;
		return 0;
	}

	int		d = 0;

	switch( node->kind ) {
	case ND_ADD:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return lv + rv ;
		}
	case ND_SUB:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return lv - rv ;
		}
	case ND_MUL:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return lv * rv ;
		}
	case ND_DIV:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return lv / rv ;
		}
	case ND_NUM:
		return node->val;
	default:
		printf( "error\n" );
		nLastError = 900;
	}

	return 0;

}
//int Calc ( Node * node )



int Print0 ( Node * node, int nDepth )							//TAG_JUMP_MARK
{

	if( node == NULL ){
		nLastError = 2;
		return 2;
	}

	char	space[] = "?";
	char *	ps = space;

	switch( node->kind ) {
	case ND_ADD:
		Indent( nDepth );
		printf( "ADD(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_SUB:
		Indent( nDepth );
		printf( "SUB(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_MUL:
		Indent( nDepth );
		printf( "MUL(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_DIV:
		Indent( nDepth );
		printf( "DIV(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_NUM:
		{
		char *	p;
		Indent( nDepth );
		printf( "NUM(%d) = %d\n", node->nBlockID, node->val );
		}
		return 0;
	default:
		printf( "error\n" );
	}

	nLastError = 1;
	return 1;

}
//int Print0 ( Node * node, int nDepth )



// �\���؂��v�����g���� Print()									//TAG_JUMP_MARK
//	�O���[�o���ϐ� NodeTop : �\����
int Print ()
{

	nLastError = 0;

	return Print0( NodeTop, 0 );

}
//int Print ( )



// end of this file : parse.c
