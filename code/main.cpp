#include "util.cpp"
#include "assignservers.cpp"
#include "buildtable.cpp"

//input/output code
int main(int argc, char* argv[]) {
	ios::sync_with_stdio(false);
	srand(time(NULL));

	//read input
	Input input;
	readInput(input, cin);

	//read command line args
	string algorithm = "";
	if(argc > 1) {
		algorithm = argv[1];
	}
		
	//solve problem
	cerr << "using algorithm " << algorithm << endl;
	if(algorithm == "mock") {
		buildtable(input);
		assignservers(input);
	}
	else {
		cerr << "unknown algorithm" << endl;
		return 1;
	}
	
	//print output
	cout << input.c << endl;
	for(int i = 0; i < input.c; i++) {
		cout << i;
		for(int v: input.servers[i]) {
			cout << " " << v;
		}
		cout << endl;
	}
};
