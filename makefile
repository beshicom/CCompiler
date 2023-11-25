


all : CCompiler.exe



CCompiler.exe : CCompiler.obj
	cl CCompiler.obj



CCompiler.obj : CCompiler.c
	cl -c CCompiler.c



