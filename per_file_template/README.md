
## Per-File Template Experiment

Demonstrate logging from headers.

Setting a file's log component from anyhere in the file, even headers..


    $ make test
    c++ -std=c++17 -O2   -c -o main.o main.cpp
    c++ -std=c++17 -O2   -c -o other_file.o other_file.cpp
    c++ -std=c++17 -O2 -o per_file_template_test  main.o other_file.o
    ./per_file_template_test
    54321:initialized ./logv2.h
    12345:initialized main.cpp
    54321:initialized ./logv2.h
    12345:main in main.cpp
    54321:from inlineFunc
    -1:from other_file.cpp
