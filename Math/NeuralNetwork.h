#pragma once
#include "Vector.h"
#include <vector>

typedef unsigned int ui;

class NeuralNetwork
{
private:
	ui m_operationCount, m_parametersCount;
	std::vector<math::Matrix> m_weightMatrices;
	std::vector<math::Vector> m_biasesVectors,
		m_layerActivations,
		m_layerActivationsWithoutSigm;

	void setLayer(ui index, ui input, ui output);

	void backPropagation(const math::Vector& input, const math::Vector& costPartialDerivative, double* gradient, ui layerIndex);

	static const double step;

public:
	NeuralNetwork(ui inputSize, ui outputSize, ui* layerSize, ui layerCount);
	NeuralNetwork(const char* filename);

	math::Vector process(const math::Vector& input) const;
	double averageCost(const std::vector<math::Vector>& inputs, const std::vector<math::Vector>& expectedOutputs) const;
	void learn(const std::vector<math::Vector>& inputs, const std::vector<math::Vector>& expectedOutputs);

	bool save(const char* filename) const;
};

