#include <iostream>
#include <chrono>
#include "macro.hpp"
#include "NeuralNetwork.h"

//#define SAVE

using namespace math;
using namespace std;
using namespace chrono;

int main()
{
	try
	{
		GC_DEBUG;
		srand(time(NULL));

#ifdef SAVE
		unsigned int layers[3] = { 4, 4, 4 };
		NeuralNetwork nn(4, 4, layers, 3);

		vector<Vector> vectors(100);
		for_in_range(i, 0, 50)
		{
			for_in_range(j, 0, 100)
				vectors[j] = Vector::random(4);

			nn.learn(vectors, vectors);
			cout << "Average cost after " << i + 1 << " batches learned: " << nn.averageCost(vectors, vectors) << endl;
		}

		nn.save("test.nn");
#else
		NeuralNetwork nn("test.nn");
		vector<Vector> vectors(100);
		for_in_range(j, 0, 100)
			vectors[j] = Vector::random(4);
		cout << "Average cost: " << nn.averageCost(vectors, vectors) << endl;

		nn.save("test1.nn");
#endif

		return 0;
	}
	catch (exception e)
	{
		cout << e.what() << endl;
		return -1;
	}
}