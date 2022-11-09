#include "AquariumSolverBP.hpp"

int main() {
	for(int q = 0; q<100; q++) {
		initAll();
		ifstream readFile;
		readFile.open("Testcases/aquarium_10_test/test"+to_string(q)+".txt");
		string s;
		readFile>>M>>N;
		cout << M << ' ' << N << '\n';
	    for(int i = 0; i < N+M; i++) {
	        readFile >> X[i];
	    }
	    for(int i = 0; i < N*M; i++) {
	    	readFile >> Land[i];
		}
	    readFile.close();
	    if(!checkCons()) {
	    	cout << ConsSum1() << '\n' << ConsSum2() << '\n';
	        cout << "Wrong Input\n";
	        return 0;
	    }
	    cout << fixed;
		cout.precision(6);
		
	    AquariumSolve(true);
	    
	    ofstream writeFile;
	    writeFile.open("BPResult/aquarium_10_result/test" + to_string(q) + "_res.txt");
	    for(int i = 0; i<=iternum; i++) {
	    	writeFile << lefts[i] << '\n';
		}
		writeFile.close();
	}
}
