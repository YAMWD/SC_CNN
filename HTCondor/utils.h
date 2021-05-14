#ifndef _UTILS_H_
#define _UTILS_H_

#include <random>
#include <unistd.h>

#include "Complement.h"
#include "boost/dynamic_bitset.hpp"

const unsigned int us = 10000;

void fwrite_vec(std::string path, double acc, double loss)
{
    FILE* fp;

    if((fp = freopen(path.c_str(), "w", stdout)) == NULL)
    {
        fprintf(stderr, "error redirecting file\n");
        exit(-1);
    }
    
    printf("acc is %lf, loss is %lf", acc, loss);

    fp = freopen("/dev/tty", "w", stdout);
}

void fwrite_vec(std::string path, std::vector< std::vector<double> > x)
{
    FILE* fp;

    if((fp = freopen(path.c_str(), "w", stdout)) == NULL)
    {
        fprintf(stderr, "error redirecting file\n");
        exit(-1);
    }

    int size1 = x.size();
    int size2 = x[0].size();

    for(int i = 0; i < size1; i++)
    {
        for(int j = 0; j < size2; j++)
            printf("%lf ", x[i][j]);
        printf("\n");
    }

    fp = freopen("/dev/tty", "w", stdout);
}

void fwrite_vec(std::string path, std::vector<double> x)
{
    FILE* fp;

    if((fp = freopen(path.c_str(), "w", stdout)) == NULL)
    {
        fprintf(stderr, "error redirecting file\n");
        exit(-1);
    }

    int size = x.size();

    for(int i = 0; i < size; i++)
        printf("%lf ", x[i]);

    fp = freopen("/dev/tty", "w", stdout);
}

int to_dec(int base, std::vector<int> x)
{
    int size = x.size();
    int now = 1;
    int ans = 0;

    for(int i = size - 1; i >= 0; i--, now *= base)
        ans += x[i] * now;

    return ans;
}

std::vector<int> vec_reverse(std::vector<int> x)
{
    std::reverse(x.begin(), x.end());

    return x;
}

boost::dynamic_bitset<> db_reverse(boost::dynamic_bitset<> x)
{
    int size = x.size();
    for(int i = 0; i < size / 2; i++)
    {
        bool tmp = x[i];
        x[i] = x[size - i - 1];
        x[size - i - 1] = tmp;
    }

    return x;
}

int find_least_3(int x)
{
    //return the minimum ans, which 3 ^ ans < 2 ^ x
    int limit = pow(2, x);
    int ans = 0;
    while(pow(3, ans + 1) < limit)
        ans++;

    return ans;
}

int get_n_bit_true_random(int n)
{
    //get n bit true random value
    std::random_device rd;
    return rd() % (1 << n);
}

int get_n_bit_pseudo_random(int n)
{
    //get n bit pseudo random value
    std::random_device rd;
    std::mt19937 gen(rd());
    return gen() % (1 << n);
}

double Mean(std::vector<double> x)
{
    int n = x.size();

    double mean = 0;
    for(int i = 0; i < n; i++)
        mean += x[i];

    mean /= n;

    return mean;
}

double Std_Dev(std::vector<double> x)
{
    int n = x.size();

    double mean = Mean(x);

    double ans = 0;
    for(int i = 0; i < n; i++)
        ans += (x[i] - mean) * (x[i] - mean);

    ans /= n;

    ans = sqrt(ans);

    return ans;
}

bool compare(Complement a, Complement b)
{
    bool flag = a.to_double() > b.to_double();

    return flag; //0 means the result of subtraction is positive and a > b, then true, vice versa.
}

bool compare(Complement a, Complement b, bool p)
{
    //if p = true, then the function will show the intermediate variables
    bool flag = a.to_double() > b.to_double();

    if(p)
    {
        a.show();
        std::cout << a.to_double() << std::endl;

        b.show();
        std::cout << b.to_double() << std::endl;

        c.show();

    }

    std::cout << flag << std::endl;

    return flag;
}

double ReLu(double x)
{
    return x > 0 ? x : 0;
}

std::vector<double> flatten(std::vector< std::vector< std::vector<double> > > inputs)
{
    std::vector<double> temp;

    int size_1 = inputs.size();
    int size_2 = inputs[0].size();
    int size_3 = inputs[0][0].size();

    temp.reserve(size_1 * size_2 * size_3);

    for(int i = 0; i < size_1; i++)
        for(int j = 0; j < size_2; j++)
            for(int k = 0; k < size_3; k++)
                temp.push_back(inputs[i][j][k]);

    return temp;
}

double sum(std::vector<double> inputs)
{
    int size = inputs.size();
    double tot = 0;
    for(int i = 0; i < size; i++)
        tot += inputs[i];

    return tot / size;
}

std::vector<double> softmax(std::vector<double> inputs)
{
    int size = inputs.size();
    std::vector<double> temp;
    temp.reserve(size);

    double e_sum = 0;
    for(int i = 0; i < size; i++)
        e_sum += std::exp(inputs[i]);

    for(int i = 0; i < size; i++)
        temp.push_back(std::exp(inputs[i]) / e_sum);

    return temp;
}

std::vector<double> Exp(std::vector<double> inputs)
{
    int size = inputs.size();
    std::vector<double> outputs;
    outputs.reserve(size);

    for(int i = 0; i < size; i++)
        outputs.push_back(std::exp(inputs[i]));

    return outputs;
}

int argmax(std::vector<double> inputs)
{
    int size = inputs.size();
    double temp = inputs[0];
    int index = 0;

    for(int i = 1; i < size; i++)
    {
        if(inputs[i] > temp)
        {
            temp = inputs[i];
            index = i;
        }
        //printf("%lf ", inputs[i]); //for debugging, visualize the value of softmax inference
    }

    //printf("\n%d %d\n", size, index); //for debugging, show the inference

    return index;
}

double cross_entropy_loss(double x)
{
    return -std::log(x);
}

std::vector<double> vec_dot(std::vector<double> a, std::vector<double> b)
{
    int size1 = a.size();
    int size2 = b.size();

    if(size1 != size2)
        throw "operand sizes do not match";

    std::vector<double> temp;
    temp.reserve(size1);
    for(int i = 0; i < size1; i++)
        temp.push_back(a[i] * b[i]);

    return temp;
}

std::vector< std::vector<double> > mat_mul(std::vector<double> a, std::vector<double> b)
{
    //(a.size, 1) @ (1, b.size)
    int size1 = a.size();
    int size2 = b.size();

    std::vector< std::vector<double> > temp;
    temp.reserve(size1);
    for(int i = 1; i <= size1; i++)
    {
        std::vector<double> object;
        object.reserve(size2);
        for(int j = 1; j <= size2; j++)
        {
            object.push_back(a[i] * b[j]);
            //printf("%lf ", a[i]);
        }
        //printf("\n");
        temp.push_back(object);
    }

    return temp;
}

std::vector< std::vector<double> > mat_fraction_minus(std::vector< std::vector<double> > a, std::vector< std::vector<double> > b, double lr)
{
    if(a.size() != b.size() || a[0].size() != b[0].size())
        throw "operand sizes do not match";

    int size1 = a.size();
    int size2 = a[0].size();

    std::vector< std::vector<double> > ans;
    ans.reserve(size1);
    for(int i = 0; i < size1; i++)
    {
        std::vector<double> temp;
        temp.reserve(size2);
        for(int j = 0; j < size2; j++)
        {
            temp.push_back(a[i][j] - lr * b[i][j]);
            if(a[i][j] - lr * b[i][j] >= 100000)
            {
                continue;
                printf("erorr int weight[%d][%d]?!!!\n", i, j);
                printf("original weight %lf\n", a[i][j]);
                printf("learning rate %lf\n", lr);
                printf("dL_dw %lf\n", b[i][j]);
            }
            //if(b[i][j] != 0)
                //printf("%.18lf\n", b[i][j]);
        }
        ans.push_back(temp);
    }

    return ans;
}

std::vector<double> mat_fraction_minus(std::vector<double> a, std::vector<double> b, double lr)
{
    if(a.size() != b.size())
        throw "operand sizes do not match";

    int size = a.size();

    std::vector<double> ans;
    ans.reserve(size);

    for(int i = 0; i < size; i++)
        ans.push_back(a[i] - lr * b[i]);

    return ans;
}

void print_vector(std::vector<int> inputs)
{
    int size1 = inputs.size();

    for(int i = 0; i < size1; i++)
        printf("%d", inputs[i]);
    printf("\n");

}

void print_vector(std::vector<double> inputs)
{
    int size1 = inputs.size();

    for(int i = 0; i < size1; i++)
        printf("%lf ", inputs[i]);
    printf("\n");

}

void print_vector(std::vector< std::vector<double> > inputs)
{
    int size1 = inputs.size();
    int size2 = inputs[0].size();

    for(int i = 0; i < size1; i++)
    {
        for(int j = 0; j < size2; j++)
            printf("%lf ", inputs[i][j]);
        printf("\n");
    }

}

void print_vector(std::vector< std::vector< std::vector<double> > > inputs)
{
    int size1 = inputs.size();
    int size2 = inputs[0].size();
    int size3 = inputs[0][0].size();

    for(int i = 0; i < size1; i++)
    {
        printf("channel %d\n", i);
        for(int j = 0; j < size2; j++)
        {
            for(int k = 0; k < size3; k++)
                printf("%lf ", inputs[i][j][k]);
            printf("\n");
        }
        printf("\n");
    }

}

void check(std::string path, std::vector< std::vector< std::vector<double> > > inputs)
{
    FILE* fp;

    if((fp = freopen(path.c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting file needed to be checked\n");
        exit(-1);
    }

    int size1 = inputs.size();
    int size2 = inputs[0].size();
    int size3 = inputs[0][0].size();

    printf("checking...\n");

    for(int i = 0; i < size1; i++)
        for(int j = 0; j < size2; j++)
            for(int k = 0; k < size3; k++)
            {
                double now;
                std::cin >> now;
                if(abs(now - inputs[i][j][k]) >= 1e-6)
                {
                    printf("%lf\n", abs(now - inputs[i][j][k]));
                    printf("mismatch detected in (%d, %d, %d)\n", i, j, k);
                    printf("should be %lf, found %lf\n", now, inputs[i][j][k]);
                    fp = freopen("/dev/tty", "r", stdin);
                    return;
                }
            }

    fp = freopen("/dev/tty", "r", stdin);

    printf("match perferctly!\n");
}

void check(std::string path, std::vector< std::vector<double> > inputs)
{
    FILE* fp;

    if((fp = freopen(path.c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting file needed to be checked\n");
        exit(-1);
    }

    int size1 = inputs.size();
    int size2 = inputs[0].size();

    printf("checking...\n");

    for(int i = 0; i < size1; i++)
        for(int j = 0; j < size2; j++)
        {
            double now;
            std::cin >> now;
            if(abs(now - inputs[i][j]) >= 1e-6)
            {
                printf("mismatch detected in (%d, %d)\n", i, j);
                printf("should be %lf, found %lf\n", now, inputs[i][j]);
                fp = freopen("/dev/tty", "r", stdin);
                return;
            }
        }

    fp = freopen("/dev/tty", "r", stdin);

    printf("match perferctly!\n");
}

void check(std::string path, std::vector<double> inputs)
{
    FILE* fp;

    if((fp = freopen(path.c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting file needed to be checked\n");
        exit(-1);
    }

    int size1 = inputs.size();

    printf("checking...\n");

    for(int i = 0; i < size1; i++)
    {
        double now;
        std::cin >> now;
        if(abs(now - inputs[i]) >= 1e-6)
        {
            printf("mismatch detected in (%d,)\n", i);
            printf("should be %lf, found %lf\n", now, inputs[i]);
            fp = freopen("/dev/tty", "r", stdin);
            return;
        }
    }

    fp = freopen("/dev/tty", "r", stdin);

    printf("match perferctly!\n");
}

#endif
