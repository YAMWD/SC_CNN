# SC_CNN

Stochastic Computing Convolution Neural Network. 

In the project, we replaced traditional binary multiplication with stochastic computing multiplication in all convolution layers of a simple convolution neural net.

Focusing on forward propagation phase, we tested the accuracy, latency and hardware resource consumptions of two differen stochastic computing method: stochastic computing based on pseudorandom and low discrepancy sequence, based on FPGA simulation on MNIST dataset.

In addition, we verified the PP(Pregressive Precision) principle of stochatic computing. And the results indicating that with little precision loss, SC could reduce latency and resource comsumption significantly.

All the weight/bias are transferred from a simple CNN, you could find the architecture and code in 

'
./notebooks/CNN.ipybn
'
