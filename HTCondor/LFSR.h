#ifndef _LFSR_H_
#define _LFSR_H_

#include "utils.h"
#include "boost/dynamic_bitset.hpp"

class LFSR
{

private:
    int m_len;
    std::vector<int> v; 
    boost::dynamic_bitset<> status;

public:
    LFSR() {};
    LFSR(int len);
    LFSR(int len, int init_value);
    boost::dynamic_bitset<> next(); //generate next status of LFSR
    void show();
};

LFSR::LFSR(int len)
{
    m_len = len;

    int rd_value;

    while(1)
    {
        rd_value = get_n_bit_true_random(m_len);

        if(!rd_value)
            continue;

        break;
    }

    boost::dynamic_bitset<> tmp_db(m_len, rd_value);
    status = tmp_db;

    switch(m_len)
    {
        case 2:
        {
            std::vector<int> tmp = {2, 1, 0};
            v = tmp;
            break;
        }

        case 3:
        {
            std::vector<int> tmp = {3, 2, 0};
            v = tmp;
            break;
        }

        case 4:
        {
            std::vector<int> tmp = {4, 3, 0};
            v = tmp;
            break;
        }

        case 5:
        {
            std::vector<int> tmp = {5, 3, 0};
            v = tmp;
            break;
        }

        case 6:
        {
            std::vector<int> tmp = {6, 5, 0};
            v = tmp;
            break;
        }

        case 7:
        {
            std::vector<int> tmp = {7, 6, 0};
            v = tmp;
            break;
        }

        case 8:
        {
            std::vector<int> tmp = {8, 6, 5, 4, 0};
            v = tmp;
            break;
        }

        case 9:
        {
            std::vector<int> tmp = {9, 5, 0};
            v = tmp;
            break;
        }

        case 10:
        {
            std::vector<int> tmp = {10, 7, 0};
            v = tmp;
            break;
        }

        case 11:
        {
            std::vector<int> tmp = {11, 9, 0};
            v = tmp;
            break;
        }

        case 12:
        {
            std::vector<int> tmp = {12, 11, 8, 6, 0};
            v = tmp;
            break;
        }

        case 13:
        {
            std::vector<int> tmp = {13, 12, 10, 9, 0};
            v = tmp;
            break;
        }

        case 14:
        {
            std::vector<int> tmp = {14, 13, 8, 4, 0};
            v = tmp;
            break;
        }

        case 15:
        {
            std::vector<int> tmp = {15, 14, 0};
            v = tmp;
            break;
        }

        case 16:
        {
            std::vector<int> tmp = {16, 15, 13, 4, 0};
            v = tmp;
            break;
        }
    }
}

LFSR::LFSR(int len, int init_value)
{
    m_len = len;

    boost::dynamic_bitset<> tmp_db(m_len, init_value);
    status = tmp_db;

    switch(m_len)
    {
        case 2:
        {
            std::vector<int> tmp = {2, 1, 0};
            v = tmp;
            break;
        }

        case 3:
        {
            std::vector<int> tmp = {3, 2, 0};
            v = tmp;
            break;
        }

        case 4:
        {
            std::vector<int> tmp = {4, 3, 0};
            v = tmp;
            break;
        }

        case 5:
        {
            std::vector<int> tmp = {5, 3, 0};
            v = tmp;
            break;
        }

        case 6:
        {
            std::vector<int> tmp = {6, 5, 0};
            v = tmp;
            break;
        }

        case 7:
        {
            std::vector<int> tmp = {7, 6, 0};
            v = tmp;
            break;
        }

        case 8:
        {
            std::vector<int> tmp = {8, 6, 5, 4, 0};
            v = tmp;
            break;
        }

        case 9:
        {
            std::vector<int> tmp = {9, 5, 0};
            v = tmp;
            break;
        }

        case 10:
        {
            std::vector<int> tmp = {10, 7, 0};
            v = tmp;
            break;
        }

        case 11:
        {
            std::vector<int> tmp = {11, 9, 0};
            v = tmp;
            break;
        }

        case 12:
        {
            std::vector<int> tmp = {12, 11, 8, 6, 0};
            v = tmp;
            break;
        }

        case 13:
        {
            std::vector<int> tmp = {13, 12, 10, 9, 0};
            v = tmp;
            break;
        }

        case 14:
        {
            std::vector<int> tmp = {14, 13, 8, 4, 0};
            v = tmp;
            break;
        }

        case 15:
        {
            std::vector<int> tmp = {15, 14, 0};
            v = tmp;
            break;
        }

        case 16:
        {
            std::vector<int> tmp = {16, 15, 13, 4, 0};
            v = tmp;
            break;
        }
    }
}

boost::dynamic_bitset<> LFSR::next()
{
    //generate next status of LFSR

    int size = v.size();
    bool tmp = status[m_len - v[0]];
    for(int i = 1; i < size; i++)
        tmp ^= status[m_len - v[i]];

    status >>= 1;
    status[m_len - 1] = tmp;

    return status;
}

void LFSR::show()
{
    std::cout << status << std::endl;
}

#endif