#include "util.cpp"

void buildtable(Input& input) {
  //resize
  input.savings.resize(input.c);
  for(int i = 0; i < input.c; i++) {
    input.savings[i].resize(input.v);
  }
  input.requestsavings.resize(input.c);
  for(int i = 0; i < input.c; i++) {
    input.requestsavings[i].resize(input.v);
  }
  //fill vectors
	for(int e = 0; e < input.e; e++) {
    for(pair<const int, int>& request: input.endpoints[e].requests) {
      for(pair<const int, int>& connection: input.endpoints[e].connections) {
        input.savings[connection.first][request.first] += request.second * (input.endpoints[e].s - connection.second);
        input.requestsavings[connection.first][request.first].push_back(e);
      }
    }
  }
}