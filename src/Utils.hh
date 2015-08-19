#include <vector>
#include <sstream>
#include <string>

vector<string> split(string str, char delim) {

  stringstream ss(str);
  vector<string> result;

  while(ss.good()) {
      string substr;
      getline(ss, substr, delim);
      result.push_back(substr);
  }

  return result;
}

void vector2double(vector<string> &in, vector<double> &out) {

  for (unsigned i = 0; i < in.size(); ++i) {
    out.push_back(stod(in[i]));
  }
}
