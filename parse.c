


// 再帰下降構文解析



#include	"parse.h"
#include	"token.h"



Node *	NodeTop = NULL;	// 構文木の先頭

int		nNumBlock = 0;	// メモリーブロックの総数



// 新しいノードを作成する new_node()							//TAG_JUMP_MARK
//	２項演算子のノードを作成する。
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
	node->nBlockID = nNumBlock++;	// 通し番号

	return node;

}
//Node * new_node ( NodeKind kind, Node * lhs, Node * rhs )



// 新しいノードを作成する new_node_num()						//TAG_JUMP_MARK
//	数値のノードを作成する。
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
	node->nBlockID = nNumBlock++;	// 通し番号

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



// 数字や()をパースする primary()								//TAG_JUMP_MARK
//	primary = '(' expr ')' | num
Node * primary ()
{

	nLastError = 0;

	Node *	node;

	// 次のトークンが '(' なら、'(' expr ')' でなければならない。
	if( consume( '(' ) ){

		node = expr();
		if( node == NULL ){
			//nLastError = 2;
			return NULL;
		}
		nLastError = expect( ')', 1000 );
		return node;

	}

	// そうでなければ数値でなければならない。
	node = new_node_num( expect_number( 1100 ) );
	if( node == NULL ){
		nLastError = 2;
		return NULL;
	}

	return node;

}
//Node * primary ()



// * や / をパースする mul()									//TAG_JUMP_MARK
//	mul = primary ( "*" primary | "/" primary )*
//	* と / は左結合の演算子。lhsが深くなる。
Node * mul ()
{

	nLastError = 0;

	Node *	node = primary();
	if( node == NULL ){
		nLastError = 1;
		return NULL;
	}
	Node *	mnd = node;

	for(;;){

		if( consume( '*' ) ){
			mnd = node;
			node = new_node( ND_MUL, node, primary() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		if( consume( '/' ) ){
			mnd = node;
			node = new_node( ND_DIV, node, primary() );
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



// + や - をパースする expr()									//TAG_JUMP_MARK
//	expr = mul ( "+" mul | "-" mul )*
//	+ と - は左結合の演算子。lhsが深くなる。
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



// ト−クンのリストをもとに、構文木を作成する Parse()			//TAG_JUMP_MARK
//	グローバル変数 token : トークンリスト
int Parse ()
{

	nNumBlock = 0;
	nLastError = 0;

	NodeTop = expr();

	return 0;

}



int Calc ( Node * node )
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



// 構文木をプリントする Print()									//TAG_JUMP_MARK
//	グローバル変数 NodeTop : 構文木
int Print ()
{

	nLastError = 0;

	return Print0( NodeTop, 0 );

}
//int Print ( )



// end of this file : parse.c
