#ifndef _UTILS_H_
#define _UTILS_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

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
    double temp = -0x3f3f3f3f;
    int index;

    for(int i = 0; i < size; i++)
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

void show_image(std::vector< std::vector<double> > inputs)
{
    int height = inputs.size();
    int width = inputs[0].size();

    cv::Mat img = cv::Mat(height, width, CV_8UC1);

    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++)
            img.data[i * width + j] = inputs[i][j];

    cv::imshow("image", img);

    cv::waitKey(0);
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