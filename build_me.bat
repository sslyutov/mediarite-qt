cmake -B "binary" -G "Visual Studio 16 2019"

cd "binary"

devenv "igcryptor.vcxproj" /Rebuild "Release|x64"

devenv "igcryptor.vcxproj" /Rebuild "Debug|x64"

cd ..
