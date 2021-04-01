#pragma once
#include "Vector.h"

typedef unsigned int ui;

class NeuralNetwork
{
private:
	math::Matrix* m_weightMatrices;
	math::Vector* m_biasesVectors,
		* m_layerActivations,
		* m_layerActivationsWithoutSigm;
	ui m_operationCount,
		m_parametersCount;

	void setLayer(ui index, ui input, ui output);

	void backPropagation(const math::Vector& input, const math::Vector& costPartialDerivative, double* gradient, ui layerIndex);

	static const double step;

public:
	NeuralNetwork(ui inputSize, ui outputSize, ui* layerSize, ui layerCount);
	~NeuralNetwork();

	math::Vector process(const math::Vector& input) const;
	double averageCost(const std::vector<math::Vector>& inputs, const std::vector<math::Vector>& expectedOutputs) const;
	void learn(const std::vector<math::Vector>& inputs, const std::vector<math::Vector>& expectedOutputs);
};

