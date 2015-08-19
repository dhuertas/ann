#include <iostream>
#include <fstream>
#include <ctime>

#include "Network.hh"
#include "Utils.hh"

#define DELIMITER ' '

using namespace std;

typedef struct Arguments {
  double eta;                 // -e
  double alpha;               // -a
  vector<unsigned> topology;  // -n
  char *trainingFile;         // -t
  char delimiter;             // -d
  char *input;                // -i
  bool quiet;                 // -q
} args_t;

args_t options;

void printUsage(int argc, char *argv[]) {

  cout << "Usage: " << argv[0] << " ";
  cout << "-n topology -t training-file [-a alpha] [-e eta] [-i input-file] ";
  cout << "[-d delimiter]" << endl;
  cout << "Description: " << endl;
  cout << "\t-n topology:      the topology of the network (example: 3;2;1)" << endl;
  cout << "\t-t training-file: file with training data" << endl;
  cout << "\t-a alpha:         the overall training rate (default: 0.5)" << endl;
  cout << "\t-e eta:           multiplier of last weight change (default: 0.15)" << endl;
  cout << "\t-i input-file:    the source of input data (default: stdin)" << endl;
  cout << "\t-d delimiter:     the delimiter of input data (default: space)" << endl;
  cout << "\t-q quiet:         silent output" << endl;
  cout << endl;
}

void getArguments(int argc, char *argv[]) {

  for (unsigned i = 1; i < argc; ++i) {

    if (strcmp("-n", argv[i]) == 0) {
      vector<string> tmp = split(argv[++i], ',');
      for (unsigned i = 0; i < tmp.size(); ++i) {
        options.topology.push_back(stoul(tmp[i]));
      }
    } else if (strcmp("-t", argv[i]) == 0) {
      options.trainingFile = argv[++i];
    } else if (strcmp("-a", argv[i]) == 0) {
      options.alpha = stod(argv[++i]);
    } else if (strcmp("-e", argv[i]) == 0) {
      options.eta = stod(argv[++i]);
    } else if (strcmp("-i", argv[i]) == 0) {
      options.input = argv[++i];
    } else if (strcmp("-d", argv[i]) == 0) {
      sscanf(argv[++i], "%c", &options.delimiter);
    } else if (strcmp("-q", argv[i]) == 0) {
      options.quiet = true;
    }
  }
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printUsage(argc, argv);
    exit(1);
  }

  // Defaults
  options.eta = 0.15;
  options.alpha = 0.5;
  options.trainingFile = NULL;
  options.delimiter = DELIMITER;
  options.input = NULL;
  options.quiet = false;

  getArguments(argc, argv);

  if (options.topology.size() == 0) {
    printf("Missing network topology\n\n");
    printUsage(argc, argv);
    exit(1);
  }

  if (options.trainingFile == NULL) {
    printf("Missing traingin data\n\n");
    printUsage(argc, argv);
    exit(1);
  }

  vector<double> inputValues;
  vector<double> targetValues;
  vector<double> resultValues;

  srand(time(NULL));

  // Note: first layer is the input layer
  Network net(options.topology);

  // Training

  // Read training file
  string line;
  ifstream infile(options.trainingFile);

  unsigned count = 0;
  while (getline(infile, line)) {

    vector<string> csv = split(line, options.delimiter);

    for (unsigned i = 0; i < csv.size(); i++) {

      if (i < options.topology[0]) {
        inputValues.push_back(stod(csv[i]));
      } else {
        targetValues.push_back(stod(csv[i]));
      }
    }

    net.feedForward(inputValues);
    net.backwardPropagation(targetValues);

    if ( ! options.quiet) {

      net.getResults(resultValues);

      cout << "round: " << count+1 << endl;
      cout << "input: ";
      for (unsigned i = 0; i < inputValues.size(); ++i) {
        cout << inputValues[i] << (i < inputValues.size() - 1 ? options.delimiter : '\n');
      }

      cout << "target: ";
      for (unsigned i = 0; i < targetValues.size(); ++i) {
        cout << targetValues[i] << (i < targetValues.size() - 1 ? options.delimiter : '\n');
      }

      cout << "output: ";
      for (unsigned i = 0; i < resultValues.size(); ++i) {
        cout << resultValues[i] << (i < resultValues.size() - 1 ? options.delimiter : '\n');
      }

      cout << "error: " << net.getError() << endl;
      cout << endl;

      resultValues.clear();
    }

    // Clear previous training data
    inputValues.clear();
    targetValues.clear();

    count++;
  }

  streambuf *cinbuf;

  if (options.input != NULL) {
    ifstream in(options.input);
    cinbuf = cin.rdbuf(); // save old buf
    cin.rdbuf(in.rdbuf()); // redirect std::cin to in.txt!
  }

  while (getline(cin, line)) {

    vector<string> csv = split(line, options.delimiter);

    for (unsigned i = 0; i < csv.size(); i++) {
      inputValues.push_back(stod(csv[i]));
    }

    net.feedForward(inputValues);
    net.getResults(resultValues);

    for (unsigned i = 0; i < resultValues.size(); ++i) {
      cout << resultValues[i] << (i < resultValues.size() - 1 ? options.delimiter : '\n');
    }
  }

  if (options.input != NULL) {
    cin.rdbuf(cinbuf);   // reset to standard input again
  }

  return 0;
}
