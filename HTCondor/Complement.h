#ifndef _COMPLEMENT_H_
#define _COMPLEMENT_H_

#include "boost/dynamic_bitset.hpp"

class Complement
{

private:
    int m_data_bits;
    int m_effective_bit;

public:
    std::vector<bool> v;
    Complement(int data_bits, double value);
    Complement(boost::dynamic_bitset<> x);
    void inc(); //+1 from the Lowest bit, including sign bit.
    void inc(int x); //+1 from the designated bit, without sign bit.
    void flip();
    void flip(int i);
    void flip(int x, int y);
    int count();
    int size();
    void show();
    double to_double();
    double to_double_unsigned();
    double to_SC_double(std::string SN_type);
};

Complement::Complement(int data_bits, double value)
{
    m_data_bits = data_bits;
    m_effective_bit = 0;

    v.reserve(data_bits);
    if(value < 0)
        v.push_back(1);
    else
        v.push_back(0);

    double tmp = abs(value);
    for(int i = 1; i < data_bits; i++)
    {
        //printf("%lf\n", tmp);
        if(tmp)
            m_effective_bit = i;

        bool x = floor(tmp * 2);
        tmp = tmp * 2 - floor(tmp * 2);

        v.push_back(x);
        
        std::cout << x;
    }
    
    std::cout << std::endl;
    
    std::cout << m_effective_bit << std::endl;

    if(value < 0)
    {
        flip(1, m_effective_bit);
        inc(m_effective_bit);
    }
    
}

Complement::Complement(boost::dynamic_bitset<> x)
{
    m_data_bits = x.size();
    m_effective_bit = x.size() - 1;

    v.reserve(m_data_bits);
    
    for(int i = 0; i < m_data_bits; i++)
        v.push_back(x[m_data_bits - 1 - i]);

}

void Complement::inc ()
{
    bool flag = true;
    //i >= 0 || i > 0 ??
    for(int i = m_data_bits - 1; i >= 0; i--)
    {
        int tmp = flag + v[i];
        flag = tmp / 2;
        v[i] = tmp % 2;
    }
}

void Complement::inc(int x)
{
    //inc start from position x, x--
    bool flag = true;
    for(int i = x; i > 0; i--)
    {
        int tmp = flag + v[i];
        flag = tmp / 2;
        v[i] = tmp % 2;
    }
}

void Complement::flip()
{
    //flip every bit, including sign bit
    for(int i = 0; i < m_data_bits; i++)
        v[i] = !v[i];
}

void Complement::flip(int i)
{
    //flip ith bit
    v[i] = !v[i];
}

void Complement::flip(int x, int y)
{
    //flip bit x to y
    for(int i = x; i <= y; i++)
        v[i] = !v[i];
}

int Complement::count()
{
    //count how many 1s in the sequence
    int ans = 0;
    for(int i = 0; i < m_data_bits; i++)
        ans += v[i];

    return ans;
}   

int Complement::size()
{
    return v.size();
}

void Complement::show()
{
    for(int i = 0; i < m_data_bits; i++)
        std::cout << v[i];
    std::cout << std::endl;
}

double Complement::to_double()
{
    //take the binary sequence as signed and compute the double value
    double base = 0.5;
    double ans = -1 * v[0];

    for(int i = 1; i < m_data_bits; i++, base /= 2)
        ans += base * v[i];

    return ans;
}

double Complement::to_double_unsigned()
{
    //take the binary sequence as unsigned and compute the double value
    double base = 1.0;
    double ans = 0;
    
    for(int i = 0; i < m_data_bits; i++, base /= 2)
        ans += base * v[i];

    return ans;
}

double Complement::to_SC_double(std::string SN_type)
{
    double N1 = count();

    double N0 = size() - count();

    double N = size();

    double ans;

    if(SN_type == "unipolar")
        ans = N1 / N;

    if(SN_type == "bipolar")
        ans = (N1 - N0) / N;

    return ans;
}

#endif
