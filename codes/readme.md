g++ $(pkg-config --cflags --libs opencv4) -std=c++11 -O3 test.cpp -o test_o3

g++ $(pkg-config --cflags --libs opencv4) -std=c++11 test.cpp -o test