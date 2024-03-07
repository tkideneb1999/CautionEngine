@echo OFF
echo "Starting build for CautionEngine Release"
call "C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\devenv.com" "../solutions/CautionEngine.sln" /build Release /out "caution_build_log.log"
echo "Build completed..."
pause