#ifndef _POOLING_H_
#define _POOLING_H_

class Pooling
{

private:
    std::string m_type;
    int m_output_channels;
    int m_kernel_height;
    int m_kernel_width;
    int m_output_height;
    int m_output_width;

public:
    Pooling(int height, int width, std::string type);
    double calc(int coordinate_x, int coordinate_y, int channel_num, std::vector< std::vector< std::vector<double> > > inputs);
    std::vector< std::vector< std::vector<double> > > forward(std::vector< std::vector< std::vector<double> > > inputs);

};

Pooling::Pooling(int height, int width, std::string type)
{
    m_type = type;
    m_kernel_height = height;
    m_kernel_width = width;
}

double Pooling::calc(int coordinate_x, int coordinate_y, int channel_num, std::vector< std::vector< std::vector<double> > > inputs)
{
    double temp = -0x3f3f3f3f;

    for(int i = coordinate_x; i < coordinate_x + m_kernel_height; i++)
        for(int j = coordinate_y; j < coordinate_y + m_kernel_width; j++)
            if(m_type == "max")
                temp = std::max(temp, inputs[channel_num][i][j]);

    return temp;
}

std::vector< std::vector< std::vector<double> > > Pooling::forward(std::vector< std::vector< std::vector<double> > > inputs)
{
    //forward prop with ReLu activation function.
    std::vector< std::vector< std::vector<double> > > outputs;

    m_output_channels = inputs.size();

    int input_height = inputs[0].size();

    int input_width = inputs[0][0].size();

    m_output_height = input_height / m_kernel_height;

    m_output_width = input_width / m_kernel_width;

    outputs.reserve(m_output_channels);

    for(int k = 0; k < m_output_channels; k++)
    {
        std::vector< std::vector<double> > temp_i;
        temp_i.reserve(m_output_height);
        for(int i = 0; i < input_height; i += m_kernel_height)
        {
            std::vector<double> temp_j;
            temp_j.reserve(m_output_width);
            for(int j = 0; j < input_width; j += m_kernel_width)
                temp_j.push_back(calc(i, j, k, inputs));
            temp_i.push_back(temp_j);
        }
        outputs.push_back(temp_i);
    }

    return outputs;
}

#endif