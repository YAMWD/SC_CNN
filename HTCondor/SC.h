#ifndef _SC_H_
#define _SC_H_

#include <bitset>
#include "utils.h"
#include "boost/dynamic_bitset.hpp"
#include "Complement.h"
#include "LFSR.h"
#include "LD.h"

class SC
{

private:
    std::string m_SN_type; //unipolar, bipolar or inversed bipolar, to be extended
    std::string m_RNG_type; //true random, LFSR, LD or more, to be extended
    int m_data_bits;
    int m_SC_LEN;
    int m_scaling_factor;
    int m_if_revert;

public:
    SC(std::string SN_type, std::string RNG_type, int data_bits, int scaling_factor);
    SC(std::string SN_type, std::string RNG_type, int data_bits, int scaling_factor, int if_revert);
    double normalize(double x);
    double normalize_back(double x);
    double normalize_back_twice(double x);
    int float2fixed_normalize(double x);
    double float2fixed_normalize_back(double x);
    Complement binary2SN(double ori_value, std::string RNG_type, int base); //base is used for LD sequence
    double SC_Mul(double a, double b);

};

SC::SC(std::string SN_type, std::string RNG_type, int data_bits, int scaling_factor)
{
    m_SN_type = SN_type;
    m_RNG_type = RNG_type;
    m_data_bits = data_bits;

    if(RNG_type == "LFSR")
        m_SC_LEN = (1 << m_data_bits) - 1;
    else
        m_SC_LEN = 1 << m_data_bits;

    m_scaling_factor = scaling_factor;

}

SC::SC(std::string SN_type, std::string RNG_type, int data_bits, int scaling_factor, int if_revert)
{
    m_SN_type = SN_type;
    m_RNG_type = RNG_type;
    m_data_bits = data_bits;
    m_if_revert = if_revert;

    if(RNG_type == "LFSR")
        m_SC_LEN = (1 << m_data_bits) - 1;
    else
        m_SC_LEN = 1 << m_data_bits;

    m_scaling_factor = scaling_factor;

}

double SC::normalize(double x)
{
    for(int i = 0; i < m_scaling_factor; i++)
        x /= 2;

    return x;
}

double SC::normalize_back(double x)
{
    for(int i = 0; i < m_scaling_factor; i++)
        x *= 2;

    return x;
}

double SC::normalize_back_twice(double x)
{
    for(int i = 0; i < m_scaling_factor; i++)
        x *= 4;

    return x;
}

int SC::float2fixed_normalize(double x)
{
    for(int i = 0; i < (m_data_bits - m_scaling_factor); i++)
        x *= 2; // >> m_scaling_factor, normalize to [-1, +1]; << m_data_bits, convert to fixed point

    return (int)x;
}

double SC::float2fixed_normalize_back(double x)
{
    for(int i = 0; i < (m_data_bits - m_scaling_factor); i++)
        x /= 2; // << m_data_bits, convert fo float point; >> m_scaling_factor, normalize back

    return x;
}


Complement SC::binary2SN(double ori_value, std::string RNG_type, int base)
{
    //if the length of binary sequence is m_data_bits(signed bit + m_data_bits - 1 bits), then the length of SC sequence should be 2 ^ (m_data_bits - 1)
    int rd_value;

    Complement BN(m_data_bits, ori_value);

    Complement SN(m_SC_LEN, 0);

    /*
    BN.show();
    printf("binary value is %lf\n", BN.to_double());
    */

    LFSR lfsr(m_data_bits);

    LD ld(base, m_data_bits);

    for(int i = 0; i < m_SC_LEN; i++)
    {
        if(RNG_type == "true" || RNG_type == "pseudo")
        {
            if(RNG_type == "true")
                rd_value = get_n_bit_true_random(m_data_bits);

            if(RNG_type == "pseudo")
                rd_value = get_n_bit_pseudo_random(m_data_bits);

            boost::dynamic_bitset<> RN_INT(m_data_bits, rd_value);
            Complement RN(RN_INT);

            SN.v[i] = compare(BN, RN);

            /*
            printf("%d\n", rd_value);
            printf("Binary: ");
            BN.show();
            printf("Random: ");
            RN.show();
            */

        }

        if(RNG_type == "LFSR")
        {
            boost::dynamic_bitset<> RN_INT = lfsr.next();
            Complement RN(RN_INT);

            SN.v[i] = compare(BN, RN);

            /*
            printf("Binary: ");
            BN.show();
            printf("Random: ");
            RN.show();
            */
        }

        if(RNG_type == "LD")
        {
            boost::dynamic_bitset<> RN_INT = ld.next();
            
            Complement RN(RN_INT, m_if_revert);

            SN.v[i] = compare(BN, RN);

            //for debug use
            //SN.v[i] = compare(BN, RN, 1);

            /*
            printf("Binary: ");
            BN.show();
            printf("Random: ");
            RN.show();
            */
        }
    }

    return SN;
}  

double SC::SC_Mul(double a, double b)
{
    std::string type = "bipolar";

    /*
    int a_int = float2fixed_normalize(a);
    int b_int = float2fixed_normalize(b);

    printf("%d %d\n", a_int, b_int);
    */

    double a_normalized = normalize(a);
    double b_normalized = normalize(b);

    //printf("%lf %lf\n", a_normalized, b_normalized);

    Complement a_SN = binary2SN(a_normalized, m_RNG_type, 2);
    Complement b_SN = binary2SN(b_normalized, m_RNG_type, 3);
    Complement ans_SN(m_SC_LEN, 0);

    //intermediate variable, for debug use
    
    /*
    printf("SN a: ");
    //a_SN.show();
    printf("value is %lf\n", a_SN.to_SC_double(type));

    printf("SN b: ");
    //b_SN.show();
    printf("value is %lf\n", b_SN.to_SC_double(type));
    */

    if(a_SN.size() != b_SN.size())
    {
        printf("size do not match!");
        exit(0);
    }
    
    int size = a_SN.size();

    for(int i = 0; i < size; i++)
    {
        if(m_SN_type == "unipolar")
            ans_SN.v[i] = a_SN.v[i] & b_SN.v[i]; //for unipolar, multiplication is AND gate

        if(m_SN_type == "bipolar")
            ans_SN.v[i] = !(a_SN.v[i] ^ b_SN.v[i]); // for bipolar, multiplication is XNOR gate
    }

    /*
    printf("SN ans: ");
    ans_SN.show();
    printf("value is %lf\n", ans_SN.to_SC_double(type));
    */

    //printf("%lf\n", ans_SN.to_SC_double());

    double ans = ans_SN.to_SC_double(type);

    return normalize_back_twice(ans);
}


#endif
