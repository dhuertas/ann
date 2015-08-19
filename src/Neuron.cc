#include "Neuron.hh"

using namespace std;

double Neuron::eta_ = 0.15;
double Neuron::alpha_ = 0.5;

// -----------------------------------------------------------------------------
Neuron::Neuron(unsigned numOutputs, unsigned index) {

  for (unsigned conn = 0; conn < numOutputs; ++conn) {
    outputWeights_.push_back(Connection());
    outputWeights_.back().weight = randomWeight();
  }

  index_ = index;
}

// -----------------------------------------------------------------------------
void Neuron::feedForward(const Layer &prevLayer) {

  double sum = 0.0;

  // Sum the previous layer's outputs (which are inputs)
  // Include the bias node from the previous layer
  for (unsigned n = 0; n < prevLayer.size(); ++n) {
    sum += prevLayer[n].getOutputValue() *
      prevLayer[n].outputWeights_[index_].weight;
  }

  outputValue_ = Neuron::transferFunction(sum);
}

// -----------------------------------------------------------------------------
double Neuron::transferFunction(double x) {

  // tanh - output range [-1.0..1.0]
  return tanh(x);
}

// -----------------------------------------------------------------------------
double Neuron::transferFunctionDerivative(double x) {

  // tanh derivative
  return 1.0 - tanh(x)*tanh(x);
}

// -----------------------------------------------------------------------------
void Neuron::calculateOutputGradients(double targetValue) {

  double delta = targetValue - outputValue_;
  gradient_ = delta * Neuron::transferFunctionDerivative(outputValue_);
}

// -----------------------------------------------------------------------------
void Neuron::calculateHiddenGradients(const Layer &nextLayer) {

  double dow = sumDOW(nextLayer);

  gradient_ = dow * Neuron::transferFunctionDerivative(outputValue_);
}

// -----------------------------------------------------------------------------
double Neuron::sumDOW(const Layer &nextLayer) const {

  double sum = 0.0;

  // Sum our contributions of the errors at the nodes we feed
  for (unsigned n = 0; n < nextLayer.size() - 1; ++n) {
    sum += outputWeights_[n].weight * nextLayer[n].gradient_;
  }

  return sum;
}

// -----------------------------------------------------------------------------
void Neuron::updateInputWeights(Layer &prevLayer) {

  // The weights to be update are in the Connection container
  // in the neurons in the preceding later
  for (unsigned n = 0; n < prevLayer.size(); ++n) {
    Neuron &neuron = prevLayer[n];
    double oldDeltaWeight = neuron.outputWeights_[index_].deltaWeight;
    double newDeltaWeight =
      // Individual input, magnified by the gradient and train rate:
      eta_
      * neuron.getOutputValue()
      * gradient_
      // Also add momentum = a fraction of the previous delta weight
      + alpha_
      * oldDeltaWeight;

      neuron.outputWeights_[index_].deltaWeight = newDeltaWeight;
      neuron.outputWeights_[index_].weight += newDeltaWeight;
  }
}
