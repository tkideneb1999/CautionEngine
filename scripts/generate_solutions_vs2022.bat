@echo off
pushd %~dp0\..\
call thirdparty\premake\premake5.exe vs2022
PAUSE