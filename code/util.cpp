#ifndef utils
#define utils

#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <string>
#include <time.h>
#include <utility>
#include <vector>

using namespace std;

//util classes
struct Endpoint {
  int s;//latency to data center
  map<int, int> requests;//first int video id, second int number of requests
  map<int, int> connections;//first int is cache server id, second int is latency
};

struct Input {
  int v, e, r, c, x;
  vector<Endpoint> endpoints;
  vector<int> videosize;
  vector<vector<long long>> savings;//first int is cache server id, second int is video id
  vector<vector<vector<int>>> requestsavings;//first int is cache server id, second int is video id, contains list of endpoints
  vector<vector<vector<int>>> requestlatency;//first int is cache server id, second int is video id, contains used latency
  vector< vector<int> > servers; // for each server store all id-s of videos
  vector <int> data_stored; // for each server stored how much volume is already used
};

//input handling
void readInput(Input& input, istream& in) {
	in >> input.v >> input.e >> input.r >> input.c >> input.x;
  input.endpoints.resize(input.e);
  input.videosize.resize(input.v);
  
  input.servers.resize(input.c);
  input.data_stored.resize(input.c);
  
  for(int i = 0; i < input.v; i++) {
    in >> input.videosize[i];
  }
  for(int i = 0; i < input.e; i++) {
    int k;
    in >> input.endpoints[i].s >> k;
    for(int j = 0; j < k; j++) {
      int c, l;
      in >> c >> l;
      input.endpoints[i].connections[c] = l;
    }
  }
  for(int i = 0; i < input.r; i++) {
    int v, e, n;
    in >> v >> e >> n;
    input.endpoints[e].requests[v] += n;
  }
}

void update(Input& input, int v, int c) 
{
  // ids of end points affected
  vector<int> &ep_ids = input.requestsavings.at(c).at(v);
  for (int i : ep_ids)
  {
    Endpoint &ep = input.endpoints[i];
    int numRequests = ep.requests[v];
    int usedLatDiff = ep.s - ep.connections[c];

    // iterate over all connected cache servers for given endpoint
    for (map<int,int>::iterator it = ep.connections.begin(); it != ep.connections.end(); it++)
    {

      int conServer = it->first;
      int otherLatDiff = ep.s - it->second;

      // chosen server is faster than conServer
      if (otherLatDiff < usedLatDiff) {
        input.savings[conServer][v] -= numRequests * otherLatDiff;
      }
      // other server is faster
      else {
        input.savings[conServer][v] -= numRequests * otherLatDiff;
        input.savings[conServer][v] += numRequests * (otherLatDiff - usedLatDiff);
      }
    }
  }
  input.savings.at(c).at(v) = 0;
}

#endif
