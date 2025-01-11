# script for powershell
g++ main.cpp persistor.cpp user.cpp -o main.bin
if ($?) { ./main.bin }
