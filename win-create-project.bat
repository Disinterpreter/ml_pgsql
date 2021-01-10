@echo off

rem Generate solutions
utils\premake5.exe vs2019

rem Create a shortcut to the solution - http://superuser.com/questions/392061/how-to-make-a-shortcut-from-cmd
set SCRIPTFILE="%TEMP%\CreateMyShortcut.vbs"
(
	echo Set oWS = WScript.CreateObject^("WScript.Shell"^)
	echo sLinkFile = oWS.ExpandEnvironmentStrings^("ml_pgsql.sln - Shortcut.lnk"^)
	echo Set oLink = oWS.CreateShortcut^(sLinkFile^)
	echo oLink.TargetPath = oWS.ExpandEnvironmentStrings^("%~dp0\Build\ml_pgsql.sln"^)
	echo oLink.Save
) 1>%SCRIPTFILE%
cscript //nologo %SCRIPTFILE%
del /f /q %SCRIPTFILE%

if %0 == "%~0" pause
