#ifndef _KERNEL_H_
#define _KERNEL_H_

#include <vector>
#include <random>
#include <utility>

class Kernel
{
    
private:
    int m_kernel_height;
    int m_kernel_width;

public:
    std::vector< std::vector<double> > v;

    Kernel(int kernel_height, int kernel_width);
    std::pair<int, int> shape();
    void showValue();
};

Kernel::Kernel(int kernel_height, int kernel_width)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d{0, 1};

    m_kernel_height = kernel_height;
    m_kernel_width = kernel_width;

    v.reserve(m_kernel_height);

    for(int i = 0; i < m_kernel_height; i++)
    {
        std::vector<double> obj;
        obj.reserve(m_kernel_width);
        for(int j = 0; j < m_kernel_width; j++)
            obj.push_back(d(gen));
        v.push_back(obj);
    }
}

std::pair<int, int> Kernel::shape()
{
    return std::make_pair(m_kernel_height, m_kernel_width);
}

void Kernel::showValue()
{
    for(int i = 0; i < m_kernel_height; i++)
    {
        for(int j = 0; j < m_kernel_width; j++)
            printf("%lf ", v[i][j]);
        printf("\n");
    }
}

#endif
