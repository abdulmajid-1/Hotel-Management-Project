#!/bin/bash
# g++ main.cpp utils.cpp persistor.cpp user.cpp -o main.bin && ./main.bin
g++ main.cpp ./**/*.cpp -o main.bin && ./main.bin
