@echo OFF
echo "Building USD Library"

call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsx86_amd64.bat"
python "validate_python_and_packages.py"
python "..\thirdparty\usd\build_scripts\build_usd.py" ..\thirdparty\usd\bin
python "ensure_usd_paths.py"
pause