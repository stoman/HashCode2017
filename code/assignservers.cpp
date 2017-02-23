#include "util.cpp"

/*
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
  vector< vector<int> > servers; // for each server store all id-s of videos
  vector <int> data_stored; // for each server stored how much volume is already used
};
*/

void compute_max_saving(Input &input, int video_id, vector<double> &max_savings, vector<int> &id_max_savings)	{
	for (int i = 0; i < input.c; i++)
	{
		if ( ((double)input.savings[i][video_id]) / ((double)input.videosize[video_id]) > max_savings[video_id] )
		{
			   max_savings[video_id] = ((double)input.savings[i][video_id]) / ((double)input.videosize[video_id]);
			id_max_savings[video_id] = i;
		}
	}
}

void assignservers(Input& input) {
	vector<double> max_savings(input.v, 0);
	vector<int> id_max_savings(input.v, 0);
	
	for (int j = 0; j < input.v; j++)
		compute_max_saving(input,j,max_savings,id_max_savings);
	
	for (int k = 0; k < input.v*input.c; k++)	{
		//choose video with maximal savings
		int maxvideo = 0;
		for (int j = 1; j < input.v; j++)
			if (max_savings[j] > max_savings[maxvideo])
				maxvideo = j;
			
		int server_id = id_max_savings[maxvideo];
		
		input.savings[server_id][maxvideo] = 0;
		max_savings[maxvideo] = 0;
		
		if (input.data_stored[server_id] + input.videosize[maxvideo] <= input.x)
		{
			input.data_stored[server_id] += input.videosize[maxvideo];
			input.servers[server_id].push_back(maxvideo);
		}
		update(input,maxvideo,server_id);
	
		compute_max_saving(input,maxvideo,max_savings,id_max_savings);
	}
}