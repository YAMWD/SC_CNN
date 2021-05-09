#ifndef _LD_H_
#define _LD_H_

#include "utils.h"
#include "boost/dynamic_bitset.hpp"

//implement halton sequence(one kind of low discrepancy sequence)
class LD
{
private:
    int m_now; //indicating ith number in the sequence
    int m_base; //base 
    int m_LD_len; //length of LD sequence
    int m_data_bits; //length of LD sequence needed for comparing with BN
    std::vector<int> status;

public:
    LD(int base, int m_data_bits);
    boost::dynamic_bitset<> next();
    void update_status();
    void show_status();
    std::vector<int> return_status();

};

LD::LD(int base, int data_bits)
{
    m_now = 0;
    m_base = base;
    m_data_bits = data_bits;

    if(base == 3)
        m_LD_len = find_least_3(m_data_bits);
    else
        m_LD_len = data_bits;

    status.reserve(m_LD_len);

    std::vector<int> tmp(m_LD_len);

    status = tmp;
}

boost::dynamic_bitset<> LD::next()
{
    boost::dynamic_bitset<> ans;

    if(m_base == 2)
    {
        boost::dynamic_bitset<> tmp(m_data_bits, m_now);
        ans = db_reverse(tmp); 

    }

    if(m_base == 3)
    {
        std::vector<int> vec_imp = vec_reverse(status);
        int value = to_dec(3, vec_imp); //the decimal value of therany

        //print_vector(vec_imp);
        //printf("%d\n", value);

        int new_value = round(value * pow(2, m_data_bits) / (double)pow(3, m_LD_len)); //scaling from therany to binary

        //printf("%d\n\n", new_value);

        boost::dynamic_bitset<> tmp(m_data_bits, new_value);
        ans = tmp;
    }

    update_status();

    m_now = (m_now + 1) % (int)pow(m_base, m_LD_len);

    return ans;
}

void LD::update_status()
{
    int size = status.size();

    bool flag = true;

    for(int i = size - 1; i >= 0; i--)
    {
        int tmp = status[i];
        status[i] = (tmp + flag) % m_base;
        flag = (tmp + flag) / m_base;
    }
}

void LD::show_status()
{
    int size = status.size();

    for(int i = 0; i < size; i++)
        std::cout << status[i];

    std::cout << std::endl;
}

std::vector<int> LD::return_status()
{
    return status;
}


#endif