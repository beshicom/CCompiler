


// 文字列をトークンに分ける
//	変数は、英小文字１文字の２６種類があらかじめ使えるようになっている。
//	変数のサイズは８バイトのみ。


#include	<stdio.h>
#include	<stdlib.h>
#include	<stdarg.h>
#include	<stdbool.h>
#include	<string.h>

#include	"object.h"



#ifndef	B__TOKEN_H__B
#define	B__TOKEN_H__B



extern char *	user_input;		// 処理するプログラムの文字列全体の先頭



// トークンの種類 TokenKind										//TAG_JUMP_MARK
typedef enum {

	TK_START,

	TK_RESERVED,	// 記号
	TK_RESERVED2,	// ２文字の記号 (トークナイズの時のみ使用)
	TK_IDENT,		// 識別子
	TK_NUM,			// 整数
	TK_EOF,			// 入力の終わり

	TK_END

} TokenKind;

typedef	struct Token	Token;

// トークン型 Token												//TAG_JUMP_MARK
struct Token {

	TokenKind	kind;	// トークンの型
	Token *		next;	// 次の入力トークン
	int			val;	// kindがTK_NUMの時
	char *		str;	// トークン文字列の先頭
	int			len;	// トークンの長さ

};

// 現在着目しているトークン
extern Token *	token;



// consume()													//TAG_JUMP_MARK
//	次のトークンtokenが期待している記号の時は真を返す。でなければ偽を返す。
//	トークンを読み進める。
bool consume ( char * op );



// consume_kind()												//TAG_JUMP_MARK
//	次のトークンtokenが期待している種類の時は真を返す。でなければ偽を返す。
//	トークンを読み進める。
bool consume_kind ( int kind, Token ** ppToken );



// expect()														//TAG_JUMP_MARK
//	次のトークンtokenが期待している記号の時はトークンを１つ読み進める。
//	でなければエラーを報告する。
int expect ( char * op, int nErrCode );



// expect_number()												//TAG_JUMP_MARK
//	次のトークンtokenが数値の場合、トークンを１つ読み進めて、その数値を返す。
//	でなければエラーを報告する。
int expect_number ( int nErrCode );



// at_eof()														//TAG_JUMP_MARK
//	次のトークンtokenが入力トークンの終わりなら真を返す。
bool at_eof ();



// tokenize()													//TAG_JUMP_MARK
//	指定した文字列をトークンに分割してリストを作成し、
//	リストの先頭のアドレスを返す。
Token * tokenize ( char * pStr, int nErrCode );



// delete_list()												//TAG_JUMP_MARK
//	トークンのリストを全て開放する。
void delete_list ( Token * pList );



#endif



// end of this file : token.h
