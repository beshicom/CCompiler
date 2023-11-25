@echo off

call :assert 0 0
call :assert 42 42
call :assert 21 "5+20-4"

echo OK

exit /b 0

:assert
	setlocal

	set expected=%1
	set input=%~2

	CCompiler %input% > tmp.asm
	ml /c /coff tmp.asm
	link /SUBSYSTEM:WINDOWS tmp.obj

	tmp
	set actual=%errorlevel%

	if %actual% == %expected% (
		echo %input% ���� %actual%
	) else (
		echo %input% ���� %expected% expected, but got %actual%
		exit /b 1
	)

	echo -----------

	endlocal
	exit /B 0
