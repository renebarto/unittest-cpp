mkdir cmake-mingw-debug
pushd cmake-mingw-debug
del CMakeCache.txt
setlocal
set ProjectRoot=F:/My_Projects

cmake .. -DCMAKE_BUILD_TYPE=Debug -DSCRIPTS_DIR="%ProjectRoot%/cmake-scripts" -DCMAKE_INSTALL_PREFIX="%ProjectRoot%/install-mingw" -DPLATFORM_INSTALL="%ProjectRoot%/install-mingw" -G "MinGW Makefiles"
endlocal
popd
