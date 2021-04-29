#ifndef _FC_H_
#define _FC_H_

class FC
{

private:
    int m_input_size;
    int m_output_size;
    double lr;
    std::string m_activation;
    std::vector<double> m_inputs;
    std::vector<double> m_outputs;
    std::vector<double> m_outputs_after_activated;

public:
    std::vector< std::vector<double> > weights; //shape(output_size, input_size)
    std::vector<double> bias;

    FC(int input_size, int output_size, std::string activation);
    std::vector<double> forward(std::vector<double> inputs);
    void backprop(std::vector<double> dL_dout);
    void show_weights();
    void show_bias();
    void show_error_weights();
    void load_weights(std::string save_path);
    void load_bias(std::string save_path);
    void show_weights_range();
    void show_bias_range();

};

FC::FC(int input_size, int output_size, std::string activation)
{

    m_activation = activation;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d{0, 1};

    m_input_size = input_size;
    m_output_size = output_size;
    m_activation = activation;

    lr = 1e-3;

    weights.reserve(m_output_size);
    bias.reserve(m_output_size);

    for(int i = 0; i < m_output_size; i++)
    {
        std::vector<double> temp;
        temp.reserve(m_output_size);

        for(int j = 0; j < m_input_size; j++)
            temp.push_back(d(gen));

        weights.push_back(temp);
        bias.push_back(0);
    }
}

std::vector<double> FC::forward(std::vector<double> inputs)
{
    m_inputs = inputs;

    std::vector<double> v;
    for(int i = 0; i < m_output_size; i++)
    {
        double temp = 0;

        for(int j = 0; j < m_input_size; j++)
            temp += weights[i][j] * inputs[j];

        temp += bias[i];
        v.push_back(temp);
    }

    m_outputs = v;

    if(m_activation == "softmax")
        m_outputs_after_activated = softmax(m_outputs);

    return m_outputs_after_activated;
}

void FC::backprop(std::vector<double> dL_dout)
{

    int size = dL_dout.size();

    for(int i = 0; i < size; i++)
    {
        //only 1 element of dL_dout would be non-zero
        if(!dL_dout[i]) //(10, 1)
            continue;

        std::vector<double> t_exp;

        // e^totals
        t_exp = Exp(m_outputs);

        //sum of all e^totals
        double S = sum(t_exp);

        std::vector<double> dout_dt;
        dout_dt.reserve(size);

        //gradients of out[i] against totals
        for(int j = 0; j < size; j++)
            if(i != j)
                dout_dt.push_back(-t_exp[i] * t_exp[j] / (S * S));
            else
                dout_dt.push_back(t_exp[i] * (S - t_exp[i]) / (S * S));


        //gradients of totals against weights/biases/input
        std::vector<double> dt_dw = m_inputs; //(1, 13 * 13 * 8)
        double dt_db = 1;
        std::vector< std::vector<double> > dt_dinputs = weights; //(10, 13 * 13 * 8)

        //gradients of loss against totals
        std::vector<double> dL_dt = vec_dot(dL_dout, dout_dt); //(10, 1)

        printf("here is raw outputs\n\n\n");

        for(int j = 0; j < m_outputs.size(); j++)
            printf("%lf ", m_outputs[j]);

        printf("\n\n\n");

        printf("here is dL_dout\n\n\n");

        for(int j = 0; j < dL_dout.size(); j++)
            printf("%lf ", dL_dout[j]);

        printf("\n\n\n");

        printf("here is dout_dt\n\n\n");
        
        for(int j = 0; j < dout_dt.size(); j++)
            printf("%lf ", dout_dt[j]);

        printf("\n\n\n");
        

        std::vector< std::vector<double> > dL_dw = mat_mul(dL_dt, dt_dw); //should be shape of (10, 13 * 13 * 8)
        std::vector<double> dL_db = dL_dt; //should be dL_dt * dt_db, but since dt_db = 1, we ommit it.
        //std::vector<double> dL_dinput = mat_mul(dL_dt, dt_dinputs);

        weights = mat_fraction_minus(weights, dL_dw, lr);
        bias = mat_fraction_minus(bias, dL_db, lr);
    }
}

void FC::show_weights()
{
    int size1 = weights.size();
    int size2 = weights[0].size();

    for(int i = 0; i < size1; i++)
    {
        for(int j = 0; j < size2; j++)
            printf("%lf ",weights[i][j]);
        printf("\n");
    }
}

void FC::show_bias()
{
    for(int i = 0; i < m_output_size; i++)
        printf("%lf ", bias[i]);
}

void FC::show_error_weights()
{
    int size1 = weights.size();
    int size2 = weights[0].size();

    for(int i = 0; i < size1; i++)
        for(int j = 0; j < size2; j++)
            if(weights[i][j] >= 100000)
                printf("error weight? weights[%d][%d] %lf\n", i, j, weights[i][j]);
}

void FC::load_weights(std::string save_path)
{
    FILE* fp;

    if((fp = freopen(save_path.c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting FC layer weights\n");
        exit(-1);
    }

    for(int i = 0; i < m_output_size; i++)
        for(int j = 0; j < m_input_size; j++)
        {
            double now;
            std::cin >> now;
            weights[i][j] = now;
        }

    fp = freopen("/dev/tty", "r", stdin);

    printf("FC layer weights loaded\n");
}

void FC::load_bias(std::string save_path)
{
    FILE* fp;

    if((fp = freopen(save_path.c_str(), "r", stdin)) == NULL)
    {
        fprintf(stderr, "error redirecting FC layer bias\n");
        exit(-1);
    }

    for(int i = 0; i < m_output_size; i++)
    {
        double now;
        std::cin >> now;
        bias[i] = now;
    }

    fp = freopen("/dev/tty", "r", stdin);

    printf("FC layer bias loaded\n");
}

void FC::show_weights_range()
{
    double weights_min = 0x3f3f3f3f;
    double weights_max = -0x3f3f3f3f;
    for(int i = 0; i < m_output_size; i++)
        for(int j = 0; j < m_input_size; j++)
        {
            weights_min = std::min(weights_min, weights[i][j]);
            weights_max = std::max(weights_max, weights[i][j]);
        }

    printf("FC layer weights min: %lf\n", weights_min);
    printf("FC layer weights max: %lf\n", weights_max);
}

void FC::show_bias_range()
{
    double bias_min = 0x3f3f3f3f;
    double bias_max = -0x3f3f3f3f;
    for(int i = 0; i < m_output_size; i++)
    {
        bias_min = std::min(bias_min, bias[i]);
        bias_max = std::max(bias_max, bias[i]);
    }

    printf("FC layer bias min: %lf\n", bias_min);
    printf("FC layer bias max: %lf\n", bias_max);
}

#endif