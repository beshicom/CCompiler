


all : CCompiler.exe


CCompiler.exe : CCompiler.obj object.obj token.obj parse.obj
	cl CCompiler.obj object.obj token.obj parse.obj


CCompiler.obj : CCompiler.c object.h token.h parse.h
	cl -c CCompiler.c


object.obj : object.c object.h
	cl -c object.c


token.obj : token.c object.h token.h
	cl -c token.c


parse.obj : parse.c object.h token.h parse.h
	cl -c parse.c



