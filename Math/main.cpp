#include <iostream>
#include <chrono>
#include "macro.hpp"
#include "NeuralNetwork.h"

using namespace math;
using namespace std;
using namespace chrono;

int main()
{
	try
	{
		GC_DEBUG;
		srand(time(NULL));

		unsigned int layers[3] = { 4, 4, 4 };
		NeuralNetwork nn(4, 4, layers, 0);

		vector<Vector> vectors(100);
		for_in_range(i, 0, 50)
		{
			for_in_range(j, 0, 100)
				vectors[j] = Vector::random(4);

			nn.learn(vectors, vectors);
			cout << "Average cost after " << i + 1 << " batches learned: " << nn.averageCost(vectors, vectors) << endl;
		}

		return 0;
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return -1;
	}
}