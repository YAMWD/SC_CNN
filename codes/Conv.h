#ifndef _CONV_H_
#define _CONV_H_

#include "Kernel.h"
#include "utils.h"
#include "SC.h"

class Conv2D
{

private:
    int m_output_channels;
    int m_stride;
    int m_input_size;
    int m_output_size;
    int m_kernel_width;
    int m_kernel_length;

public:
    std::vector<Kernel> kernels;
    std::vector<double> bias;

    Conv2D(int output_channels, int kernel_width, int kernel_length, int stride);

    int get_input_size();
    int get_output_size();
    double calc(int coordinate_x, int coordinate_y, int kernel_num, std::vector< std::vector<double> > inputs);
    double SC_calc(int coordinate_x, int coordinate_y, int kernel_num, std::vector< std::vector<double> > inputs, std::string RNG_type, int data_bits);
    std::vector< std::vector< std::vector<double> > > forward(std::vector< std::vector<double> > inputs);
    std::vector< std::vector< std::vector<double> > > SC_forward(std::vector< std::vector<double> > inputs, std::string RNG_type, int data_bits);
    void load_weights(std::string save_path);
    void load_bias(std::string save_path);
    void show_weights();
    void show_bias();
    void show_weights_range();
    void show_bias_range();

};

Conv2D::Conv2D(int output_channels, int kernel_width, int kernel_length, int stride)
{
    m_output_channels = output_channels;
    m_stride = stride;
    m_kernel_width = kernel_width;
    m_kernel_length = kernel_length;

    kernels.reserve(output_channels);
    bias.reserve(output_channels);

    for(int i = 0; i < output_channels; i++)
    {
        Kernel temp(kernel_width, kernel_length);
        kernels.push_back(temp);

        bias.push_back(0);
    }
}

int Conv2D::get_input_size()
{
    return m_input_size;
}

int Conv2D::get_output_size()
{
    return m_output_size;
}

double Conv2D::calc(int coordinate_x, int coordinate_y, int kernel_num, std::vector< std::vector<double> > inputs)
{
    double temp = 0;

    int size = kernels[kernel_num].shape().first;

    for(int i = coordinate_x; i < coordinate_x + size; i++)
        for(int j = coordinate_y; j < coordinate_y + size; j++)
            temp += inputs[i][j] * kernels[kernel_num].v[i - coordinate_x][j - coordinate_y];

    return temp;
}

double Conv2D::SC_calc(int coordinate_x, int coordinate_y, int kernel_num, std::vector< std::vector<double> > inputs, std::string RNG_type, int data_bits)
{
    SC sc("bipolar", RNG_type, data_bits, 3);

    double temp = 0;

    int size = kernels[kernel_num].shape().first;

    for(int i = coordinate_x; i < coordinate_x + size; i++)
        for(int j = coordinate_y; j < coordinate_y + size; j++)
            temp += sc.SC_Mul(inputs[i][j], kernels[kernel_num].v[i - coordinate_x][j - coordinate_y]);

    return temp;
}



std::vector< std::vector< std::vector<double> > > Conv2D::forward(std::vector< std::vector<double> > inputs)
{
    //forward prop with ReLu activation function. (8, 26, 26)
    std::vector< std::vector< std::vector<double> > > outputs;

    m_input_size = inputs.size();

    int n = m_input_size;

    int m = kernels[0].shape().first;

    m_output_size = n - m + 1;

    outputs.reserve(m_output_channels);

    for(int k = 0; k < m_output_channels; k++)
    {
        std::vector< std::vector<double> > temp_i;
        temp_i.reserve(m_output_size);
        for(int i = 0; i < m_output_size; i += m_stride)
        {
            std::vector<double> temp_j;
            temp_j.reserve(m_output_size);
            for(int j = 0; j < m_output_size; j += m_stride)
                temp_j.push_back(ReLu(calc(i, j, k, inputs) + bias[k]));
            temp_i.push_back(temp_j);
        }
        outputs.push_back(temp_i);
    }

    return outputs;
}

std::vector< std::vector< std::vector<double> > > Conv2D::SC_forward(std::vector< std::vector<double> > inputs, std::string RNG_type, int data_bits)
{
    //forward prop with ReLu activation function. (8, 26, 26)
    std::vector< std::vector< std::vector<double> > > outputs;

    m_input_size = inputs.size();

    int n = m_input_size;

    int m = kernels[0].shape().first;

    m_output_size = n - m + 1;

    outputs.reserve(m_output_channels);

    for(int k = 0; k < m_output_channels; k++)
    {
        std::vector< std::vector<double> > temp_i;
        temp_i.reserve(m_output_size);
        for(int i = 0; i < m_output_size; i += m_stride)
        {
            std::vector<double> temp_j;
            temp_j.reserve(m_output_size);
            for(int j = 0; j < m_output_size; j += m_stride)
                temp_j.push_back(ReLu(SC_calc(i, j, k, inputs, RNG_type, data_bits) + bias[k]));
            temp_i.push_back(temp_j);
        }
        outputs.push_back(temp_i);
    }

    return outputs;
}

void Conv2D::load_weights(std::string save_path)
{
    int kernel_height = kernels[0].shape().first;
    int kernel_width = kernels[0].shape().second;

    FILE* fp;

    if((fp = freopen(save_path.c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting conv layer weights\n");
        exit(-1);
    }

    for(int i = 0; i < m_output_channels; i++)
        for(int j = 0; j < kernel_height; j++)
            for(int k = 0; k < kernel_width; k++)
            {
                double now;
                std::cin >> now;
                kernels[i].v[j][k] = now;
            }

    fp = freopen("/dev/tty", "r", stdin);

    printf("conv layer weights loaded\n");
}

void Conv2D::load_bias(std::string save_path)
{
    FILE* fp;

    if((fp = freopen(save_path.c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting conv layer bias\n");
        exit(-1);
    }

    for(int i = 0; i < m_output_channels; i++)
    {
        double now;
        std::cin >> now;
        bias[i] = now;
    }

    fp = freopen("/dev/tty", "r", stdin);

    printf("conv layer bias loaded\n");
}

void Conv2D::show_weights()
{

    int kernel_height = kernels[0].shape().first;
    int kernel_width = kernels[0].shape().second;

    for(int i = 0; i < m_output_channels; i++)
        for(int j = 0; j < kernel_height; j++)
            for(int k = 0; k < kernel_width; k++)
                printf("%lf ", kernels[i].v[j][k]);

}

void Conv2D::show_bias()
{
    for(int i = 0; i < m_output_channels; i++)
        printf("%lf ", bias[i]);
}

void Conv2D::show_weights_range()
{

    int kernel_height = kernels[0].shape().first;
    int kernel_width = kernels[0].shape().second;

    double weights_min = 0x3f3f3f3f;
    double weights_max = -0x3f3f3f3f;
    for(int i = 0; i < m_output_channels; i++)
        for(int j = 0; j < kernel_height; j++)
            for(int k = 0; k < kernel_width; k++)
            {
                weights_min = std::min(weights_min, kernels[i].v[j][k]);
                weights_max = std::max(weights_max, kernels[i].v[j][k]);
            }

    printf("Conv layer weights min: %lf\n", weights_min);
    printf("Conv layer weights max: %lf\n", weights_max);

}

void Conv2D::show_bias_range()
{
    double bias_min = 0x3f3f3f3f;
    double bias_max = -0x3f3f3f3f;
    for(int i = 0; i < m_output_channels; i++)
    {
        bias_min = std::min(bias_min, bias[i]);
        bias_max = std::max(bias_max, bias[i]);
    }

    printf("Conv layer bias min: %lf\n", bias_min);
    printf("Conv layer bias max: %lf\n", bias_max);
}

#endif