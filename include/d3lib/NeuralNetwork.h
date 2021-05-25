#ifndef __NEURALNETWORK_H__
#define __NEURALNETWORK_H__

#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>
#include <fstream>

struct Layer { virtual double get(double input, std::vector<double> out) = 0; };

namespace NNLayer
{
    template<typename Func> struct Default : Layer 
    { 
        double get(double input, std::vector<double> out) { return Func()(input); } 
    };

    template<typename Func> struct MaxPooling : Layer 
    {
        double get(double input, std::vector<double> out) { return Func()(*std::max_element(out.begin(), out.end())); } 
    };   

    template<typename Func> struct AvgPooling : Layer 
    { 
        double get(double input, std::vector<double> out) { return Func()(std::accumulate(out.begin(), out.end(), 0.0) / out.size()); } 
    }; 

    template<typename Func> struct FullyConnected : Layer 
    { 
        double get(double input, std::vector<double> out) { return Func()(input); } 
    };   

    template<typename Func> struct Convolutional : Layer 
    { 
        double get(double input, std::vector<double> out) { return Func()(input); } 
    };   
    
    template<typename Func> struct Recurrent : Layer 
    { 
        double get(double input, std::vector<double> out) { return Func()(input); } 
    };  
}

namespace NNFunc
{
    struct sigmoid { double operator()(double x) { return (1 / (1 + exp(-x))); } };
    struct tanh { double operator()(double x) { return (exp(x) - exp(-x)) / (exp(x) + exp(-x)); } };
    struct softmax { double operator()(double x) { return exp(x) / (1 + exp(x)); } };
    struct softplus { double operator()(double x) { return log(1 + exp(x)); } };
    struct reLU { double operator()(double x) { return x > 0 ? x : 0; } };
    struct softSign {  double operator()(double x) { return x / (1 + abs(x)); } };
    struct Linner { double operator()(double x) { return x; } };   
    struct bentIdentity { double operator()(double x) { return ((sqrt(x * x + 1) - 1) / 2) + x; } };
    struct binaryStep { double operator()(double x) { return x < 0 ? 0 : 1; } };
    struct step { double operator()(double x) { return x > 0.9 ? 1.0 : x < 0.1 ? 0.0 : x; } };
    struct nstep { double operator()(double x) { return x > 0 ? 1 : -1; } };
    struct sinc { double operator()(double x) { return x == 0 ? 0 : sin(x) / x; } };  
    struct siLU { double operator()(double x) { return x / (1 + exp(-x)); } };
    struct elu { double operator()(double x) { return x > 0 ? (1.0 * (exp(x) - 1)) : x; } };
    struct gelu { double operator()(double x) { return x * (1 + erf(x / sqrt(2))) / 2; } };
    struct gaussian { double operator()(double x) { return exp(-x * x); } };
    
    struct sigmoid_grad { double operator()(double x) { return (1 - x) * x; } };
    struct relu_grad { double operator()(double x) { return (x <= 0 ? 0.001 : 1); } };
    struct tanh_grad { double operator()(double x) { return 1 - (x * x); } };

    struct sigmoid_d { double operator()(double x) { return x * (1 - x); } };
    struct tanh_d { double operator()(double x) { return 1.0f - x * x; } };
    struct elu_d { double operator()(double x) { return x > 0 ? 1.0 : 1.0 * exp(x); } };
}

template<typename T> using Matrix = std::vector<std::vector<T>>;

struct MatrixFunc 
{
	static std::vector<double> mul(const Matrix<double> &m, const std::vector<double> &v) 
	{
		std::vector<double> res(v.size());

		for (size_t i = 0; i < res.size(); ++i) {
			for (size_t j = 0; j < res.size(); ++j)
				res[i] += m[i][j] * v[j];
		}

		return res;
	}

	static std::vector<double> add(const Matrix<double> &m, const std::vector<double> &v) 
	{
		std::vector<double> res(v.size());

		for (size_t i = 0; i < res.size(); ++i) {
			for (size_t j = 0; j < res.size(); ++j)
				res[i] += m[i][j] + v[j];
		}

		return res;
	}

	static double dot(const Matrix<double> &m, const std::vector<double> &v)
	{
		double result = 0;
		for (auto& vm : m)
			result += std::inner_product(vm.begin(), vm.end(), v.begin(), 0.0);
		return result;
	}

	static std::vector<double> transpose(const Matrix<double> &m, const int Col, const int Row) 
	{
		std::vector<double> mT(Col*Row);
		for(unsigned n = 0; n != Col*Row; n++) 
			mT[n] = m[0][Row * n % Col + n / Col];
		return mT;
    }
};

struct Data 
{
	std::vector<double> vector;
	std::string result;

	int operator[](size_t i) const { return vector[i]; }
	size_t size() const { return vector.size(); }
};

namespace d3lib
{
    class NeuralNetwork
    {
    private:
        Matrix<double> Weights, bias;
        std::vector<int> topology;
        std::vector<Layer*> _layer;
        std::vector<double> values;
        int inputlayers, hiddenlayers, outputlayers;
        double learningRate, error = 0;

    public:
        NeuralNetwork(std::vector<int> topology) : topology(topology) 
        {
            inputlayers = this->topology[0];
            hiddenlayers = this->topology[1];
            outputlayers = this->topology[this->topology.size()-1];
        }

        void SetWeights(const Matrix<double> W) { Weights = W; }
        void Setbias(const Matrix<double> bias) { this->bias = bias; }
        
        double GetError() { return error; }

        template<typename Layer> void AddLayer()
        { 	
            Layer* lr = new Layer;
            _layer.push_back(lr);
        }

        std::vector<Data> ImageDeserialize(const std::string &filename) 
        {
            std::ifstream file(filename);
            size_t image_count, vector_size;
            file >> image_count >> vector_size;

            std::vector<Data> data(image_count);

            for (size_t i = 0; i < image_count; ++i) 
            {
                file >> data[i].result;

                for (size_t j = 0; j < vector_size; ++j) 
                {
                    char c;
                    file >> c;
                    data[i].vector.push_back('0' == c ? -1 : 1);
                }
            }

            return data;
        }

        template<typename Data>
        Matrix<double> train(const std::vector<Data> &a) 
        {
            size_t size = a[0].size();
            Matrix<double> w(size, std::vector<double>(size));

            for (size_t i = 0; i < size; ++i)
                for (size_t j = 0; j < size; ++j)
                    if (i != j)
                        for (size_t k = 0; k < a.size(); ++k)
                            w[i][j] += a[k][i] * a[k][j];

            return w;
        }

        std::vector<double> predict(const std::vector<double> &v) 
        {			
            std::vector<double> out = MatrixFunc::mul(Weights, v);

            if(_layer.empty()) 
                return out;

            std::vector<double> output;
            for(size_t i = 0; i < out.size(); i++)
                output.push_back({ _layer[0]->get(out[i], out) });
            return output;
        }
    };
}

#endif