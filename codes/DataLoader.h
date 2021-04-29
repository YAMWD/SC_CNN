#ifndef _DATALOADER_H_
#define _DATALOADER_H_

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>

const std::string base_dir = "/Users/yaozhiyan/desktop/projects/SC_CNN/data/MNIST";

const std::string train_images_dir = base_dir + "/train_images/";

const std::string train_labels_dir = base_dir + "/train_labels/";

const std::string test_images_dir = base_dir + "/test_images/";

const std::string test_labels_dir = base_dir + "/test_labels/";

const int TRAIN_IMAGES_MAXN = 6e4;

const int TRAIN_LABELS_MAXN = 6e4;

const int TEST_IMAGES_MAXN = 1e4;

const int TEST_LABELS_MAXN = 1e4;

const int width = 28;

const int length = 28;

std::vector< std::vector< std::vector<double> > > train_images;

std::vector< std::vector< std::vector<double> > > test_images;

std::vector<int> train_labels;

std::vector<int> test_labels;

void load_training_set()
{
    FILE* fp;

    if((fp = freopen((train_images_dir + "train_images.out").c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting training_images\n");
        exit(-1);
    }

    printf("loading training set...\n");

    train_images.reserve(TRAIN_IMAGES_MAXN);
    for(int i = 0; i < TRAIN_IMAGES_MAXN; i++)
    {
        std::vector< std::vector<double> > temp_i;
        temp_i.reserve(width);
        for(int j = 0; j < width; j++)
        {
            std::vector<double> temp_j;
            temp_j.reserve(length);
            for(int k = 0; k < length; k++)
            {
                double temp;
                std::cin >> temp;
                //temp_j.push_back(temp); //original pic
                temp_j.push_back(temp / 255); //pre-processing
            }
            temp_i.push_back(temp_j);
        }
        train_images.push_back(temp_i);
    }

    fp = freopen("/dev/tty", "r", stdin);

    if((fp = freopen((train_labels_dir + "train_labels.out").c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting training_labels\n");
        exit(-1);
    }

    train_labels.reserve(TRAIN_LABELS_MAXN);
    for(int i = 0; i < TRAIN_LABELS_MAXN; i++)
    {
        int temp;
        std::cin >> temp;
        train_labels.push_back(temp);
    }

    fp = freopen("/dev/tty", "r", stdin);

    printf("training set loaded\n");
}

void load_test_set()
{
    FILE* fp;

    if((fp = freopen((test_images_dir + "test_images.out").c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting test_images\n");
        exit(-1);
    }

    printf("loading test set...\n");

    test_images.reserve(TEST_IMAGES_MAXN);
    for(int i = 0; i < TEST_IMAGES_MAXN; i++)
    {
        std::vector< std::vector<double> > temp_i;
        temp_i.reserve(width);
        for(int j = 0; j < width; j++)
        {
            std::vector<double> temp_j;
            temp_j.reserve(length);
            for(int k = 0; k < length; k++)
            {
                double temp;
                std::cin >> temp;
                //temp_j.push_back(temp); //original pic
                temp_j.push_back(temp / 255); //pre-processing
            }
            temp_i.push_back(temp_j);
        }
        test_images.push_back(temp_i);
    }

    fp = freopen("/dev/tty", "r", stdin);

    if((fp = freopen((test_labels_dir + "test_labels.out").c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting test_labels\n");
        exit(-1);
    }

    test_labels.reserve(TEST_LABELS_MAXN);
    for(int i = 0; i < TEST_LABELS_MAXN; i++)
    {
        int temp;
        std::cin >> temp;
        test_labels.push_back(temp);
    }

    fp = freopen("/dev/tty", "r", stdin);

    printf("test set loaded\n");
}
#endif
