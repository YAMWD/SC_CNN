#include <iostream>
#include <vector>
#include <bitset>
#include <random>
#include "utils.h"
#include "SC.h"
 
//using Eigen::MatrixXd;

std::vector< std::vector<double> > SC_true_abs_err;
std::vector< std::vector<double> > SC_LFSR_abs_err;
std::vector< std::vector<double> > SC_LD_abs_err;

std::vector<double> SC_true_RMSE;
std::vector<double> SC_LFSR_RMSE;
std::vector<double> SC_LD_RMSE;

void init()
{
    //fill the vector till position x

    int size = 32;

    SC_true_abs_err.reserve(size);
    SC_LFSR_abs_err.reserve(size);
    SC_LD_abs_err.reserve(size);

    SC_true_RMSE.reserve(size);
    SC_LFSR_RMSE.reserve(size);
    SC_LD_RMSE.reserve(size);

    std::vector<double> tmp(1);

    for(int i = 0; i < size; i++)
    {
        SC_true_abs_err.push_back(tmp);
        SC_LFSR_abs_err.push_back(tmp);
        SC_LD_abs_err.push_back(tmp);

        SC_true_RMSE.push_back(0);
        SC_LFSR_RMSE.push_back(0);
        SC_LD_RMSE.push_back(0);
    }
}

void Test_utils_compare()
{    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    for(int i = 0; i < 1000; i++)
    {
        double tmp1 = dis(gen);
        double tmp2 = dis(gen);

        Complement a(16, tmp1);
        Complement b(16, tmp2);

        bool flag_true = tmp1 > tmp2 ? 1 : 0;
        bool flag_compare = compare(a, b);

        if(flag_true != flag_compare)
        {
            std::cout << flag_true << ' ' << flag_compare << std::endl;
            std::cout << tmp1 << ' ' << tmp2 << std::endl;
            compare(a, b, true);
            return;
        }
    }

    printf("unit test utils compare() done, all green\n");

}

void Test_SC_err(std::string RNG_type, int data_bits, int scaling_factor, int num)
{
    //return abs error and RMSE(Root Mean Squared Error) of different type of SN

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-8.0, 8.0);

    SC sc("bipolar", RNG_type, data_bits, scaling_factor);

    //printf("%d start\n", data_bits);

    std::vector<double> tmp_abs;
    std::vector<double> tmp_RMSE;

    tmp_abs.reserve(num);
    tmp_RMSE.reserve(num);

    for(int i = 0; i < num; i++)
    {
        double a = dis(gen);
        double b = dis(gen);

        double ans = sc.SC_Mul(a, b);

        double abs_err = abs(ans - a * b);
        tmp_abs.push_back(abs_err);
        tmp_RMSE.push_back(abs_err * abs_err);
    }

    //printf("%d one done\n", data_bits);

    if(RNG_type == "true")
    {
        SC_true_abs_err[data_bits] = tmp_abs;
        SC_true_RMSE[data_bits] = Std_Dev(tmp_RMSE);
    }

    if(RNG_type == "LFSR")
    {
        SC_LFSR_abs_err[data_bits] = tmp_abs;
        SC_LFSR_RMSE[data_bits] = Std_Dev(tmp_RMSE);
    }

    if(RNG_type == "LD")
    {
        SC_LD_abs_err[data_bits] = tmp_abs;
        SC_LD_RMSE[data_bits] = Std_Dev(tmp_RMSE);
    }
}

void Test_LFSR()
{
    LFSR lfsr(8);

    for(int i = 0; i < 10; i++)
        std::cout << lfsr.next() << std::endl;

}

void Test_LD()
{
    LD ld(2, 8);
    for(int i = 0; i < 10; i++)
	std::cout << ld.next() << std::endl;
}

void Test_SC_LFSR()
{
    SC sc("bipolar", "LFSR", 16, 3);
}

void Test()
{
    SC sc("bipolar", "LFSR", 3, 3);

    double a = -4;
    double b = 4;

    double ans = sc.SC_Mul(a, b);

    //printf("%lf\n", ans);
}

void Test_Complement()
{
    Complement a(8, -0.25);
    a.show();
}

void write(int x, int y)
{
    //write results of data_bits x to data_bits y to file for storing

    //true random
    for(int i = 2; i <= 16; i++)
        printf("%lf ", SC_true_RMSE[i]);

    printf("\n");

    //LFSR
    for(int i = 2; i <= 16; i++)
        printf("%lf ", SC_LFSR_RMSE[i]);

    printf("\n");

    //LD
    for(int i = 2; i <= 16; i++)
        printf("%lf ", SC_LD_RMSE[i]);

    printf("\n");
}

int main()
{
    init();
    
    //Test_LD();

    //Test_Complement();
    
    //return 0;

    int trials = 10000;

    SC sc("bipolar", "LD", 16, 3);

    printf("%lf\n", sc.SC_Mul(4, 3));

    return 0;
    
    for(int i = 2; i <= 16; i++)
    {
        Test_SC_err("true", i, 3, trials);
        Test_SC_err("LFSR", i, 3, trials);
        Test_SC_err("LD", i, 3, trials);
    }
    
    write(2, 16);

    //fwrite abs err
    fwrite_vec("../data/results/true_abs_err_10k.out", SC_true_abs_err);

    fwrite_vec("../data/results/LFSR_abs_err_10k.out", SC_LFSR_abs_err);

    fwrite_vec("../data/results/Halton_abs_err_10k.out", SC_LD_abs_err);

    //fwrite RMSE
    fwrite_vec("../data/results/true_RMSE_10k.out", SC_true_RMSE);

    fwrite_vec("../data/results/LFSR_RMSE_10k.out", SC_LFSR_RMSE);

    fwrite_vec("../data/results/Halton_RMSE_10k.out", SC_LD_RMSE);

    //Test();

    //Test_SC_LFSR();

    //Test_LFSR();

    /*LD ld(3, 5); //ld(base, sequence_length)

    for(int i = 0; i < 30; i++)
    {
        //ld.show_status();
        //ld.update_status();
        //ld.next();
        std::cout << ld.next() << std::endl;
    }
    */

    return 0;

}
