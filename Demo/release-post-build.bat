upx.exe --ultra-brute ..\x64\Release\Demo.exe
if not exist ..\x64\Release\Data mkdir ..\x64\Release\Data
copy Data\*.* ..\x64\Release\Data\
move ..\x64\Release\Data\bass.dll ..\x64\Release\