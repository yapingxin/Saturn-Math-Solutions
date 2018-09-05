cmake -G "Visual Studio 14 2015 Win64" ../..
cmake --build .

@echo off

@cd .\Debug

@echo Generated EXE files:
@dir *.exe

@echo on
