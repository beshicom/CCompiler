


// 再帰下降構文解析
//	英小文字１文字の８バイトの変数を扱う。
//	代入演算子 = を加える。 a=b=c=1
//	; で区切って複数の文が書けるようにする。
//
//	program = statement*
//	statement = expr ";"
//	expr = assign
//	assign = equality ( "=" assign )?
//	equality = relational ( "==" relational | "!=" relational )*
//	relational = add ( "<" add | "<=" add | ">" add | ">=" add )*
//	add = mul ( "+" mul | "-" mul )*
//	mul = unary ( "*" unary | "/" unary )*
//	unary = ( "+" | "-" )? primary
//	primary = '(' expr ')' | num | ident
//

#include	"parse.h"
#include	"token.h"



Node *	NodeTop[ nMaxNode ];	// 構文木の先頭

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



// 新しいノードを作成する new_node_ident()						//TAG_JUMP_MARK
//	シンボルのノードを作成する。
Node * new_node_ident ( NodeKind kind, Token * pToken )
{

	nLastError = 0;

	Node *	node = (Node*)calloc( 1, sizeof(Node) );
	if( node == NULL ){
		nLastError = 2;
		return NULL;
	}

	node->kind = kind;
	node->lhs = NULL;
	node->rhs = NULL;
	node->val = 0;
	node->offset = ( pToken->str[0] - 'a' + 1 ) * 8;

	node->nErr = 0;
	node->nBlockID = nNumBlock++;	// 通し番号

	return node;

}
//Node * new_node_ident ( NodeKind kind, Token * pToken )



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



// delete_tree_all ()											//TAG_JUMP_MARK
void delete_tree_all ()
{

	int		i = 0;
	for( ; NodeTop[i] != NULL; ++i )	delete_tree( NodeTop[i] );

}



Node * expr();



// 数字や()やシンボルをパースする primary()						//TAG_JUMP_MARK
//	primary = '(' expr ')' | num | ident
Node * primary ()
{

	nLastError = 0;

	Node *	node;

	// 次のトークンが '(' なら、'(' expr ')' でなければならない。
	if( consume( "(" ) ){

		node = expr();
		if( node == NULL ){
			//nLastError = 2;
			return NULL;
		}
		nLastError = expect( ")", 1000 );
		return node;

	}

	// ローカル変数
	Token *	pToken;
	if( consume_kind( TK_IDENT, &pToken ) ){

		node = new_node_ident( ND_LCVAR, pToken );	// ローカル変数
		if( node == NULL ){
			//nLastError = 2;
			return NULL;
		}
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



// 単項の + と - をパースする。 unary()							//TAG_JUMP_MARK
//	unary = ( "+" | "-" )? primary
//
Node * unary ()
{

	nLastError = 0;

	// +x -> x
	if( consume("+") )	return primary();

	// -x -> 0-x
	if( consume("-") ){
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



// * や / をパースする mul()									//TAG_JUMP_MARK
//	mul = unary ( "*" unary | "/" unary )*
//	* と / は左結合の演算子。lhsが深くなる。
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

		if( consume( "*" ) ){
			mnd = node;
			node = new_node( ND_MUL, node, unary() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		if( consume( "/" ) ){
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



// + や - をパースする add()									//TAG_JUMP_MARK
//	add = mul ( "+" mul | "-" mul )*
//	+ と - は左結合の演算子。lhsが深くなる。
Node * add ()
{

	nLastError = 0;

	Node *	node = mul();
	if( node == NULL ){
		nLastError = 1;
		return NULL;
	}
	Node *	mnd = node;

	for(;;){

		if( consume( "+" ) ){
			mnd = node;
			node = new_node( ND_ADD, node, mul() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		if( consume( "-" ) ){
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
//Node * add ()



// < と <= と > と >= をパースする relational()					//TAG_JUMP_MARK
//	relational = add ( "<" add | "<=" add | ">" add | ">=" add )*
//	< と <= と > と >= は左結合の演算子。lhsが深くなる。
Node * relational()
{

	nLastError = 0;

	Node *	node = add();
	if( node == NULL ){
		nLastError = 1;
		return NULL;
	}
	Node *	mnd = node;

	for(;;){

		if( consume( "<" ) ){
			mnd = node;
			node = new_node( ND_SML, node, add() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		if( consume( "<=" ) ){
			mnd = node;
			node = new_node( ND_SME, node, add() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		if( consume( ">" ) ){
			mnd = node;
			node = new_node( ND_BIG, node, add() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		if( consume( ">=" ) ){
			mnd = node;
			node = new_node( ND_BGE, node, add() );
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
//Node * relational()



// == と != をパースする equality()								//TAG_JUMP_MARK
//	equality = relational ( "==" relational | "!=" relational )*
//	== と != は左結合の演算子。lhsが深くなる。
Node * equality ()
{

	nLastError = 0;

	Node *	node = relational();
	if( node == NULL ){
		nLastError = 1;
		return NULL;
	}
	Node *	mnd = node;

	for(;;){

		if( consume( "==" ) ){
			mnd = node;
			node = new_node( ND_EQU, node, relational() );
			if( node == NULL ){
				nLastError = mnd->nErr = 2;
				return mnd;
			}
			continue;
		}

		if( consume( "!=" ) ){
			mnd = node;
			node = new_node( ND_NEQ, node, relational() );
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



// = をパースする assign()										//TAG_JUMP_MARK
//	assign = equality ( "=" assign )?
//	== と != は左結合の演算子。lhsが深くなる。
Node * assign ()
{

	nLastError = 0;

	Node *	node = equality();
	if( node == NULL ){
		nLastError = 1;
		return NULL;
	}

	if( consume( "=" ) ){
		Node *	mnd = node;
		node = new_node( ND_ASSIGN, node, assign() );
		if( node == NULL ){
			nLastError = mnd->nErr = 2;
			return mnd;
		}
		return node;
	}

	return node;

}



// expr()														//TAG_JUMP_MARK
// expr = assign
Node * expr ()
{

	return assign();

}
//Node * expr ()



// ; をパースする statement()									//TAG_JUMP_MARK
//	statement = expr ";"
Node * statement ()
{

	nLastError = 0;

	Node *	node = expr();
	if( node == NULL ){
		nLastError = 1;
		return NULL;
	}

	nLastError = expect( ";", 2000 );

	return node;

}



// ト－クンのリストをもとに、構文木を作成する Parse()			//TAG_JUMP_MARK
//	グローバル変数 token : トークンリスト
//	program = staemanet*
int Parse ()
{

	nNumBlock = 0;
	nLastError = 0;

	int	i = 0;
	while( ! at_eof() ){
		NodeTop[i] = statement();
		if( NodeTop[i] == NULL )	return nLastError;
		if( ++i >= nMaxNode-1 )		break;
	}
	NodeTop[i] = NULL;

	return nLastError;

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
	case ND_ASSIGN:
		{
		if( node->rhs == NULL )	return 0;
		return Calc( node->rhs );
		}
	case ND_EQU:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return ( lv == rv ) ? 1 : 0 ;
		}
	case ND_NEQ:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return ( lv != rv ) ? 1 : 0 ;
		}
	case ND_SML:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return ( lv < rv ) ? 1 : 0 ;
		}
	case ND_SME:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return ( lv <= rv ) ? 1 : 0 ;
		}
	case ND_BIG:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return ( lv > rv ) ? 1 : 0 ;
		}
	case ND_BGE:
		{
		int	lv = Calc( node->lhs );
		if( node->rhs == NULL )	return lv;
		int	rv = Calc( node->rhs );
		return ( lv >= rv ) ? 1 : 0 ;
		}
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
	case ND_LCVAR:
		return 0;
	default:
		printf( "error Calc() : node->kind=%d\n", node->kind );
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
	case ND_ASSIGN:
		Indent( nDepth );
		printf( "ASSIGN=(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_EQU:
		Indent( nDepth );
		printf( "EQU==(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_NEQ:
		Indent( nDepth );
		printf( "NEQ!=(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_SML:
		Indent( nDepth );
		printf( "SML<(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_SME:
		Indent( nDepth );
		printf( "SME<=(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_BIG:
		Indent( nDepth );
		printf( "BIG>(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
	case ND_BGE:
		Indent( nDepth );
		printf( "BGE>=(%d)\n", node->nBlockID );
		if( node->lhs != NULL )
			Print0( node->lhs, nDepth+1 );
		if( node->rhs != NULL )
			Print0( node->rhs, nDepth+1 );
		return 0;
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
		Indent( nDepth );
		printf( "NUM(%d) = %d\n", node->nBlockID, node->val );
		return 0;
	case ND_LCVAR:
		Indent( nDepth );
		printf( "LCVAR(%d) = %c\n", node->nBlockID, (node->offset)/8+'a'-1 );
		return 0;
	default:
		printf( "error Print0()\n" );
	}

	nLastError = 1;
	return 1;

}
//int Print0 ( Node * node, int nDepth )



// 構文木をプリントする Print()									//TAG_JUMP_MARK
//	グローバル変数 NodeTop[] : 構文木
int Print ()
{

	nLastError = 0;

	int	i = 0;
	for( ; NodeTop[i] != NULL; ++i ){
		int	rslt = Print0( NodeTop[i], 0 );
		if( rslt )	nLastError = rslt;
	}

	return nLastError;

}
//int Print ( )



// end of this file : parse.c
