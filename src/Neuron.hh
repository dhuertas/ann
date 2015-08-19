#ifndef __NEURON_HH__
#define __NEURON_HH__

#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;

struct Connection {
  double weight;
  double deltaWeight;
};

class Neuron;

typedef vector<Neuron> Layer;

class Neuron {

 private:

  unsigned index_;

  double outputValue_;

  double gradient_;

  vector<Connection> outputWeights_;

  static double eta_; // [0.0..1.0] overall net training rate

  static double alpha_; // [] multiplier of last weight change (momentum)

  static double randomWeight(void) { return rand()/double(RAND_MAX); }

  static double transferFunction(double x);

  static double transferFunctionDerivative(double x);

 protected:

 public:

  Neuron(unsigned numOutputs, unsigned index);

  void feedForward(const Layer &prevLayer);

  void setOutputValue(double value) { outputValue_ = value; }

  double getOutputValue() const { return outputValue_; }

  void calculateOutputGradients(double targetValue);

  void calculateHiddenGradients(const Layer &nextLayer);

  double sumDOW(const Layer &nextLayer) const;

  void updateInputWeights(Layer &prevLayer);

};

#endif
