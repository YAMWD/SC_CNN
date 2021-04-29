#include <iostream>
#include "DataLoader.h"
#include "Conv.h"
#include "Pooling.h"
#include "FC.h"
#include "utils.h"

int main()
{
    std::string base = "/Users/yaozhiyan/desktop/projects/SC_CNN/data/stored_weights/";

    //load_training_set();

    load_test_set();

    Conv2D a(8, 3, 3, 1);

    a.load_weights(base + "conv1.0.weight");

    a.load_bias(base + "conv1.0.bias");

    a.show_weights_range();

    a.show_bias_range();

    //a.show_weights();

    //a.show_bias();

    Pooling b(2, 2, "max");

    FC c(13 * 13 * 8, 10, "softmax");

    c.load_weights(base + "FC.weight");

    c.load_bias(base + "FC.bias");

    c.show_weights_range();

    c.show_bias_range();

    return 0;
    
    //c.show_weights();

    //c.show_error_weights();

    std::vector< std::vector< std::vector<double> > > out;

    double num = train_images.size();
    double pos = 0;
    double loss = 0;
    int cnt = 0;
    int batch_size = 128;

    //print_vector(train_images[0]);
    
    for(int i = 0; i < TEST_IMAGES_MAXN; i++, cnt++)
    {
        printf("processing case %d\n", i);

        out = a.forward(test_images[i]);

        out = b.forward(out);

        std::vector<double> temp;

        temp = flatten(out);

        temp = c.forward(temp);

        int prediction = argmax(temp);

        if(prediction == test_labels[i])
            pos++;

        loss += cross_entropy_loss(temp[test_labels[i]]);
    }

    printf("overall accuracy is %lf%%, loss is %lf\n", pos / TEST_IMAGES_MAXN * 100, loss / TEST_IMAGES_MAXN);

    return 0;

    for(int i = 0; i < num; i++, cnt++)
    {
        //forward prop

        printf("processing case %d\n", i);

        //print_vector(out);

        out = a.forward(train_images[i]);

        //print_vector(out);

        //check("/Users/yaozhiyan/desktop/projects/SC_CNN/data/check_file/test_fc_output", out);

        //return 0;

        out = b.forward(out);

        //print_vector(out);

        //check("/Users/yaozhiyan/desktop/projects/SC_CNN/data/check_file/test_fc_output", out);

        //return 0;

        std::vector<double> temp;

        temp = flatten(out);

        //check("/Users/yaozhiyan/desktop/projects/SC_CNN/data/check_file/test_fc_output", temp);

        //return 0;

        temp = c.forward(temp);

        //print_vector(temp);

        //c.show_error_weights();

        int prediction = argmax(temp);

        if(prediction == train_labels[i])
            pos++;

        loss += cross_entropy_loss(temp[train_labels[i]]);

        printf("prediction is %d, probability of the prediction is %lf%%, loss is %lf\n", prediction, temp[prediction] * 100, cross_entropy_loss(temp[train_labels[i]]));

        //return 0;

        /*
        //backward prop
        std::vector<double> gradient(10);

        gradient[train_labels[i]] = -1 / temp[train_labels[i]];

        c.backprop(gradient);

        //gradient = b.backprop(gradient);

        //gradient = a.backprop(gradient);
        */

        if(cnt == batch_size)
        {
            printf("past 128 samples overall accuracy is %lf%%, loss is %lf\n", pos / batch_size * 100, loss / batch_size);
            loss = 0;
            pos = 0;
            cnt = 0;
        }
    }

    return 0;
}