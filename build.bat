@ECHO OFF

xcopy src\glsl bin\glsl /I /Y
xcopy src\lua bin\lua /I /Y
xcopy src\opencl bin\opencl /I /Y
xcopy graphics bin\graphics /I /Y
xcopy data bin\data /I /Y

cd bin

mingw32-make

GameEngine.exe

cd ..