mkdir cmake-msvc-debug
pushd cmake-msvc-debug
del CMakeCache.txt
setlocal
set ProjectRoot=F:/My_Projects

call "%VS140COMNTOOLS%..\..\VC\vcvarsall.bat"
cmake .. -DCMAKE_BUILD_TYPE=Debug -DSCRIPTS_DIR="%ProjectRoot%/cmake-scripts" -DCMAKE_INSTALL_PREFIX="%ProjectRoot%/install-msvc" -DPLATFORM_INSTALL="%ProjectRoot%/install-msvc" -G "Visual Studio 14 2015 Win64"
endlocal
popd
