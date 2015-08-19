#include "Network.hh"

#include <iostream>
#include <cassert>

using namespace std;

// -----------------------------------------------------------------------------
Network::Network(const vector<unsigned> &topology) {

  unsigned numLayers = topology.size();

  for (unsigned layer = 0; layer < numLayers; ++layer) {
    layers_.push_back(Layer());

    unsigned numOutputs = layer == topology.size() -1 ? 0 : topology[layer+1];

    // Add neurons to layer
    for (unsigned neuron = 0; neuron <= topology[layer]; ++neuron) {
      // This also adds the bias neuron
      layers_.back().push_back(Neuron(numOutputs, neuron));
    }

    layers_.back().back().setOutputValue(1.0);
  }

  error_ = 0.0;
  recentAverageError_ = 0.0;
  recentAverageSmoothingFactor_ = 0.5;
}

// -----------------------------------------------------------------------------
void Network::feedForward(const vector<double> &inputValues) {

  if (inputValues.size() != layers_[0].size() - 1) {

    cout << "input values length " << inputValues.size();
    cout << " does not match the number of input neurons ";
    cout << layers_[0].size() - 1 << endl;

    exit(-1);
  }

  // assign (latch) the input values into the input neurons
  for (unsigned i = 0; i < inputValues.size(); ++i) {
    layers_[0][i].setOutputValue(inputValues[i]);
  }

  // Forward propagate
  for (unsigned layer = 1; layer < layers_.size(); ++layer) {
    Layer &prevLayer = layers_[layer-1];
    for (unsigned n = 0; n < layers_[layer].size() - 1; ++n) {
      layers_[layer][n].feedForward(prevLayer);
    }
  }
}

// -----------------------------------------------------------------------------
void Network::backwardPropagation(const vector<double> &targetValues) {

  // Calculate overall net error (RMS of output neuron errors)

  Layer &outputLayer = layers_.back();

  error_ = 0.0;

  for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
    double delta = targetValues[n] - outputLayer[n].getOutputValue();
    error_ += delta*delta;
  }

  error_ /= outputLayer.size() - 1;
  error_ = sqrt(error_); // RMS

  // Implement a recent average measurement
  recentAverageError_ = (recentAverageError_ * recentAverageSmoothingFactor_
    + error_) / (recentAverageSmoothingFactor_ + 1.0);

  // Calculate output layer gradients
  for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
    outputLayer[n].calculateOutputGradients(targetValues[n]);
  }

  // Calculate gradients on hidden layers
  for (unsigned layer = layers_.size() - 2; layer > 0; --layer) {
    Layer &hiddenLayer = layers_[layer];
    Layer &nextLayer = layers_[layer+1];

    for (unsigned n = 0; n < layers_[layer].size() - 1; ++n) {
      hiddenLayer[n].calculateHiddenGradients(nextLayer);
    }
  }

  // For all layers from outputs to first hidden layer, update connection weights
  for (unsigned layerNum = layers_.size() - 1; layerNum > 0; --layerNum) {
    Layer &layer = layers_[layerNum];
    Layer &prevLayer = layers_[layerNum-1];

    for (unsigned n = 0; n < layer.size() - 1; ++n) {
      layer[n].updateInputWeights(prevLayer);
    }
  }
}

// -----------------------------------------------------------------------------
void Network::getResults(vector<double> &resultValues) const {

  resultValues.clear();

  for (unsigned n = 0; n < layers_.back().size() - 1; ++n) {
    resultValues.push_back(layers_.back()[n].getOutputValue());
  }
}
