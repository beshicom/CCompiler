@echo off

call :assert 0 0
call :assert 42 42
call :assert 21 "5+20-4"
call :assert 41 " 12 + 34 - 5 "
call :assert 47 "5+6*7"
call :assert 15 "5*(9-6)"
call :assert 4 "(3+5)/2"
call :assert 2 "+2"
call :assert -2 "-2"
call :assert 2 "(+2)"
call :assert -2 "(-2)"
call :assert 1 "-2++3"
call :assert 2 "-2*-3+(-4)"
call :assert 1 "1<2"
call :assert 0 "2<1"
call :assert 1 "1>=1"
call :assert 1 "2>=1"
call :assert 0 "1>=2"
call :assert 1 "1+9<=2*6"
call :assert 1 "1==1"
call :assert 0 "1!=1"

echo OK

exit /b 0

:assert
	setlocal

	set expected=%1
	set input=%2

	CCompiler %input% > tmp.asm
	ml /c /coff tmp.asm
	link /SUBSYSTEM:WINDOWS tmp.obj

	tmp
	set actual=%errorlevel%

	if %actual% == %expected% (
		echo %input% ÅÅÅÑ %actual%
	) else (
		echo %input% ÅÅÅÑ %expected% expected, but got %actual%
		exit /b 1
	)

	echo -----------

	endlocal
	exit /B 0
