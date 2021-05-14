g++ $(pkg-config --cflags --libs opencv4) -std=c++11 -O3 test.cpp -o test_o3

g++ -g $(pkg-config --cflags --libs opencv4) -std=c++11 -O3 CNN.cpp -o CNN

g++ $(pkg-config --cflags --libs opencv4) -std=c++11 -O3 test.cpp -o test

g++ -std=c++11 -O3 CNN.cpp -o CNN

g++ -I /home/zyao09/boost_1_76_0 -std=c++11 -O3 CNN.cpp -o CNN

g++ -I /home/zyao09/boost_1_76_0 -std=c++11 -O3 test.cpp -o test

find ./ -name "Halton_bits*" -exec rm {} \;
