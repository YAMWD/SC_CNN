To run

`g++ -g $(pkg-config --cflags --libs opencv4) -std=c++11 -O3 CNN.cpp -o CNN`

`g++ $(pkg-config --cflags --libs opencv4) -std=c++11 -O3 test.cpp -o test`
