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
std::vector<double> SC_DC_RMSE;

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
    SC_DC_RMSE.reserve(size);

    std::vector<double> tmp(1);

    for(int i = 0; i < size; i++)
    {
        SC_true_abs_err.push_back(tmp);
        SC_LFSR_abs_err.push_back(tmp);
        SC_LD_abs_err.push_back(tmp);

        SC_true_RMSE.push_back(0);
        SC_LFSR_RMSE.push_back(0);
        SC_LD_RMSE.push_back(0);
        SC_DC_RMSE.push_back(0);
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

void Test_DC_err(std::string RNG_type, int data_bits, int scaling_factor, int num)
{
    //return abs error and RMSE(Root Mean Squared Error) of different type of SN

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-8.0, 8.0);

    //printf("%d start\n", data_bits);

    std::vector<double> tmp_abs;
    std::vector<double> tmp_RMSE;

    tmp_abs.reserve(num);
    tmp_RMSE.reserve(num);

    for(int i = 0; i < num; i++)
    {
        double a = dis(gen);
        double b = dis(gen);
        int a_int,b_int;
        double ans;
        if(data_bits <= 4){
            a_int = (int)round( a / (1 << -(data_bits - scaling_factor - 1)));
            b_int = (int)round( b / (1 << -(data_bits - scaling_factor - 1)));
        }
        else {
            a_int = (int)round( a * (1 << (data_bits - scaling_factor - 1)));
            b_int = (int)round( b * (1 << (data_bits - scaling_factor - 1)));

        }
        if(a_int == 1<<(data_bits-1)) a_int = a_int-1;//in case of overflow
        if(b_int == 1<<(data_bits-1)) b_int = b_int-1;

        //printf("%lf,%d,%d,bit:%d\n",a,(int)a,a_int,data_bits);//check the mode of quantization
        if(data_bits <= 6){
            ans = (double)LD_mult_ap(a_int, b_int, data_bits) * (1 << -(data_bits - scaling_factor * 2 - 1));

        }else{
            ans = (double)LD_mult_ap(a_int, b_int, data_bits) / (1 << (data_bits - scaling_factor * 2 - 1));
        }
        //printf("%lf  * %lf, %d * %d : ori = %lf, SC = %lf, bit:%d\n", a, b,a_int, b_int,  a * b, ans,data_bits);

        double abs_err = std::abs(ans - a * b);
        tmp_abs.push_back(abs_err);
        tmp_RMSE.push_back(abs_err * abs_err);
    }

    SC_DC_RMSE[data_bits] = Std_Dev(tmp_abs);

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

        //printf("%lf * %lf: ori = %lf, SC = %lf\n", a, b, a * b, ans);

        double abs_err = std::abs(ans - a * b);
        tmp_abs.push_back(abs_err);
        tmp_RMSE.push_back(abs_err * abs_err);
    }

    //printf("%d one done\n", data_bits);

    if(RNG_type == "true")
    {
        SC_true_abs_err[data_bits] = tmp_abs;
        SC_true_RMSE[data_bits] = Std_Dev(tmp_abs);
    }

    if(RNG_type == "LFSR")
    {
        SC_LFSR_abs_err[data_bits] = tmp_abs;
        SC_LFSR_RMSE[data_bits] = Std_Dev(tmp_abs);
    }

    if(RNG_type == "LD")
    {
        SC_LD_abs_err[data_bits] = tmp_abs;
        SC_LD_RMSE[data_bits] = Std_Dev(tmp_abs);
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
    for(int i = x; i <= y; i++)
        printf("%lf ", SC_true_RMSE[i]);

    printf("\n");

    //LFSR
    for(int i = x; i <= y; i++)
        printf("%lf ", SC_LFSR_RMSE[i]);

    printf("\n");

    //LD
    for(int i = x; i <= y; i++)
        printf("%lf ", SC_LD_RMSE[i]);

    printf("\n");

    //DC DownCounter
    for(int i = x; i <= y; i++)
        printf("%lf ", SC_DC_RMSE[i]);

    printf("\n");

    //print_vector(SC_LFSR_abs_err);

    /*
    //for test
    //fwrite abs err
    //fwrite_vec("../data/results/true_abs_err_test.out", SC_true_abs_err);

    fwrite_vec("../data/results/LFSR_abs_err_test.out", SC_LFSR_abs_err);

    fwrite_vec("../data/results/Halton_abs_err_test.out", SC_LD_abs_err);

    //fwrite RMSE
    //fwrite_vec("../data/results/true_RMSE_test.out", SC_true_RMSE);

    fwrite_vec("../data/results/LFSR_RMSE_test.out", SC_LFSR_RMSE);

    fwrite_vec("../data/results/Halton_RMSE_test.out", SC_LD_RMSE);
    */
}

int main(int argc, char *argv[])
{
    init();
    
    //Test_LD();

    //Test_Complement();
    
    int trials = 10000;

    /*
    std::string RNG_type = std::string(argv[1]);
    int bits = atoi(argv[2]);
    double a = atof(argv[3]);
    double b = atof(argv[4]);

    SC sc("bipolar", RNG_type, bits, 3);

    printf("%lf\n", sc.SC_Mul(a, b));

    return 0;
    */
    
    int s = 4;
    int e = 16;

    for(int i = s; i <= e; i++)
    {
        //Test_SC_err("true", i, 3, trials);
        Test_SC_err("LFSR", i, 3, trials);
        Test_SC_err("LD", i, 3, trials);
        Test_DC_err("DC", i, 3, trials); //downcounter
    }
    
    write(s, e);

    return 0;

    /*
    //fwrite abs err
    fwrite_vec("../data/results/true_abs_err_10k.out", SC_true_abs_err);

    fwrite_vec("../data/results/LFSR_abs_err_10k.out", SC_LFSR_abs_err);

    fwrite_vec("../data/results/Halton_abs_err_10k.out", SC_LD_abs_err);
    */

    //fwrite RMSE
    //fwrite_vec("../data/results/true_RMSE_10k.out", SC_true_RMSE);

    fwrite_vec("../data/results/LFSR_RMSE_10k.out", SC_LFSR_RMSE);

    fwrite_vec("../data/results/Halton_RMSE_10k.out", SC_LD_RMSE);

    fwrite_vec("../data/results/Halton_DC_RMSE_10k.out", SC_DC_RMSE);

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
