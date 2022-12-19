
set curdir=%cd%

call C:\QtOpenSource\6.3.1\msvc2019_64\bin\qtenv2.bat

cd %curdir%

qmake -tp vc -r "media-rite.pro" -o "media-rite.vcxproj"

devenv media-rite.vcxproj /Rebuild "Release|x64"