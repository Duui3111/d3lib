#include <iostream>

#include "../d3lib/NeuralNetwork.h"
#include "../d3lib/ImgProcessing.h"

template<typename T>
void PrintMatrix(Matrix<T> matrix)
{
	for(int i = 0; i < matrix.size(); i++)
	{
		for(int j = 0; j < matrix.size(); j++)
			std::cout << (matrix[i][j] == 1 ? 1 : 0);
		std::cout << std::endl;
	}
}

Matrix<double> Read_Bmp(std::string filename) 
{
	d3lib::Image bmp(filename);

	Matrix<double> a = bmp.ToMatrix();

	PrintMatrix(a);

	Matrix<double> aa(10);

	for(int i = 0; i < a.size(); i++)
		for(int j = 0; j < a[i].size(); j++)
			aa[0].push_back(a[i][j]);

	return aa;
}

int main() 
{
	Matrix<double> img = Read_Bmp("char_test.bmp");

	d3lib::NeuralNetwork nn({10*10, 200, 1});

	auto digits = nn.ImageDeserialize("data/dataset/letters.txt");
	
	nn.SetWeights(nn.train(digits));

	nn.AddLayer<NNLayer::Default<NNFunc::nstep>>();

	for (const auto &digit : digits)
		if (nn.predict(img[0]) == digit.vector) 
			std::cout << "result " << digit.result << std::endl;

	return 0;
}