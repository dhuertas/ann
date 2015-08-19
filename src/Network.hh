#ifndef __NETWORK_HH__
#define __NETWORK_HH__

#include <vector>

#include "Neuron.hh"

using namespace std;

class Network {

 private:

  vector<Layer> layers_;

  double error_;

  double recentAverageError_;

  double recentAverageSmoothingFactor_;

 protected:

 public:

  Network(const vector<unsigned> &topology);

  void feedForward(const vector<double> &inputValues);

  void backwardPropagation(const vector<double> &targetValues);

  void getResults(vector<double> &resultValues) const;

  double getError() const { return error_; }

  double getRecentAverageError() const { return recentAverageError_; }

};

#endif
