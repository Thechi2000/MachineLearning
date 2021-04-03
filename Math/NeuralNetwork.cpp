#include "NeuralNetwork.h"
#include "macro.hpp"
#include "math.h"
#include "CompressedFile.h"
#include <fstream>

using namespace math;
using namespace std;

const double NeuralNetwork::step = 1;

void NeuralNetwork::setLayer(ui index, ui input, ui output)
{
    m_weightMatrices[index] = Matrix::random(input, output);
    m_biasesVectors[index] = Vector::random(output);
}

void NeuralNetwork::backPropagation(const math::Vector& input, const math::Vector& costPartialDerivative, double* gradient, ui layerIndex)
{
    const Matrix& weights = m_weightMatrices[layerIndex];
    const Vector& biases = m_biasesVectors[layerIndex],
        & previousLayer = layerIndex ? m_layerActivations[layerIndex - 1] : input,
        & z = m_layerActivationsWithoutSigm[layerIndex];

    ui currentGradientElementIndex = -1; 

    for_in_range(i, 0, weights.height())
        for_in_range(j, 0, weights.width())
            gradient[++currentGradientElementIndex] += costPartialDerivative.get(i) * sigmd(z.get(i)) * previousLayer.get(j);

    for_in_range(i, 0, biases.height())
        gradient[++currentGradientElementIndex] += costPartialDerivative.get(i) * sigmd(z.get(i));

    Vector previousLayerCostPartialDerivative(previousLayer.height());
    for_in_range(k, 0, previousLayer.height())
    {
        for_in_range(j, 0, weights.height())
            previousLayerCostPartialDerivative.at(k) += weights.get(j, k) * sigmd(z.get(j)) * costPartialDerivative.get(j);
    }

    if (layerIndex)
        backPropagation(input, previousLayerCostPartialDerivative, gradient + currentGradientElementIndex, layerIndex - 1);
}

NeuralNetwork::NeuralNetwork(ui inputSize, ui outputSize, ui* layerSize, ui layerCount) :
    m_operationCount(layerCount + 1),
    m_parametersCount(0),
    m_weightMatrices(m_operationCount),
    m_biasesVectors(m_operationCount),
    m_layerActivations(m_operationCount),
    m_layerActivationsWithoutSigm(m_operationCount)
{
    if (!layerCount)
    {
        setLayer(0, inputSize, outputSize);
        goto count_parameter;
    }

    setLayer(0, inputSize, layerSize[0]);

    for_in_range(i, 0, layerCount - 1)
        setLayer(i + 1, layerSize[i], layerSize[i + 1]);

    setLayer(layerCount, layerSize[layerCount - 1], outputSize);

    count_parameter:
    for_in_range(i, 0, m_operationCount)
    {
        m_parametersCount += m_weightMatrices[i].width() * m_weightMatrices[i].height();
        m_parametersCount += m_biasesVectors[i].height();
    }
}
NeuralNetwork::NeuralNetwork(const char* filename) :
    m_operationCount(CompressedFile(filename) >> m_operationCount),
    m_weightMatrices(m_operationCount),
    m_biasesVectors(m_operationCount),
    m_layerActivations(m_operationCount),
    m_layerActivationsWithoutSigm(m_operationCount),
    m_parametersCount(0)
{
    CompressedFile file(filename);
    ui previousLayerSize, nextLayerSize;

    file >> previousLayerSize;
    file >> nextLayerSize;

    for_in_range(i, 0, m_operationCount)
    {
        m_weightMatrices[i] = Matrix(previousLayerSize, nextLayerSize);
        m_biasesVectors[i] = Vector(nextLayerSize);

        previousLayerSize = nextLayerSize;
        file >> nextLayerSize;
    }

    for_in_range(k, 0, m_operationCount)
    {
        Matrix& weights = m_weightMatrices[k];
        Vector& biases = m_biasesVectors[k];

        m_parametersCount += weights.width() * weights.height();
        m_parametersCount += biases.height();
       
        for_in_range(i, 0, weights.height())
            for_in_range(j, 0, weights.width())
            file >> weights(i, j);

        for_in_range(i, 0, biases.height())
            file >> biases[i];
    }
}

math::Vector NeuralNetwork::process(const math::Vector& input) const
{
    THROW_IF(input.height() != m_weightMatrices[0].width());

    Vector result(input);

    for_in_range(i, 0, m_operationCount)
    {
        result *= m_weightMatrices[i];
        result += m_biasesVectors[i];
        result.applySigmoid();
    }

    return result;
}
double NeuralNetwork::averageCost(const std::vector<math::Vector>& inputs, const std::vector<math::Vector>& expectedOutputs) const
{
    THROW_IF(inputs.size() != expectedOutputs.size());

    double cost = 0;
    for_in_range(i, 0, inputs.size())
    {
        Vector output(process(inputs[i]));
        for_in_range(j, 0, output.height())
            cost += pow(output[j] - expectedOutputs[i].get(j), 2);
    }

    return cost / inputs.size();
}
void NeuralNetwork::learn(const std::vector<math::Vector>& inputs, const std::vector<math::Vector>& expectedOutputs)
{
    double* gradient = NEWARR(double, m_parametersCount);
    memset(gradient, 0, m_parametersCount * sizeof(double));

    for_in_range(iter, 0, inputs.size())
    {
        const Vector& expectedOutput = expectedOutputs[iter],
            & input = inputs[iter];

        for_in_range(i, 0, m_operationCount)
        {
            m_layerActivations[i] = m_weightMatrices[i] * (i ? m_layerActivations[i - 1] : input);
            m_layerActivations[i] += m_biasesVectors[i];
            m_layerActivationsWithoutSigm[i] = m_layerActivations[i];
            m_layerActivations[i].applySigmoid();
        }

        Vector& actualOutput = m_layerActivations[m_operationCount - 1];
        THROW_IF(expectedOutput.height() != actualOutput.height());

        Vector costPartialDerivative(actualOutput.height());
        for_in_range(i, 0, actualOutput.height())
            costPartialDerivative[i] = 2 * (actualOutput.get(i) - expectedOutput.get(i));

        backPropagation(input, costPartialDerivative, gradient, m_operationCount - 1);
    }

    ui currentGradientCompononentIndex = -1;
    for (int k = m_operationCount - 1; k >= 0; --k)
    {
        Matrix& weights = m_weightMatrices[k];
        Vector& biases = m_biasesVectors[k];

        for_in_range(i, 0, weights.height())
            for_in_range(j, 0, weights.width())
            weights(i, j) -= step * gradient[++currentGradientCompononentIndex] / inputs.size();

        for_in_range(i, 0, biases.height())
            biases[i] -= step * gradient[++currentGradientCompononentIndex] / inputs.size();
    }

    DEL(gradient);
}

bool NeuralNetwork::save(const char* filename) const
{
    CompressedFile file(filename, std::ios_base::out | std::ios_base::trunc);
    if (!file.isOpen())

        return false;
    file << m_operationCount;
    file << m_weightMatrices[0].width();
    for_in_range(i, 0, m_operationCount)
        file << m_biasesVectors[i].height();

    for_in_range(k, 0, m_operationCount)
    {
        const Matrix& weights = m_weightMatrices[k];
        const Vector& biases = m_biasesVectors[k];

        for_in_range(i, 0, weights.height())
            for_in_range(j, 0, weights.width())
            file << weights.get(i, j);

        for_in_range(i, 0, biases.height())
            file << biases.get(i);
    }

    return true;
}
