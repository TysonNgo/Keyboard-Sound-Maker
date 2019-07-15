release="src\qt\build-gui-Desktop_Qt_5_13_0_MinGW_64_bit-Release\release"
windeployqt="C:\Qt\5.13.0\mingw73_64\bin\windeployqt.exe"
exe="gui.exe"


echo "Adding  DLL's..."
$($windeployqt "$release\\$exe")

echo "Copying additional DLL's..."
cp "C:\Qt\5.13.0\mingw73_64\bin\libgcc_s_seh-1.dll" $release
cp "C:\Qt\5.13.0\mingw73_64\bin\libstdc++-6.dll" $release
cp "C:\Qt\5.13.0\mingw73_64\bin\libwinpthread-1.dll" $release
