rmdir /s /q build
mkdir build
cd build
cmake ..
cmake --build . --config Release
copy Release\correction.exe ..\correction.exe
cd ..