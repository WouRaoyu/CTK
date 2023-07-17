cmake -B build-msvc -S . -G "Visual Studio 17 2022" -A "x64" ^
    -DCMAKE_TOOLCHAIN_FILE=%CONDA_PREFIX%\h2o.cmake