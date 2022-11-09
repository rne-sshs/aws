#include "AquariumSolverBP.hpp"

int main() {
	for(int pctg = 0; pctg<=100; pctg+=5) {
		cout << pctg << '\n';
		for(int q = 0; q<200; q++) {
			initAll();
			ifstream readFile;
			readFile.open("Testcases/newaquarium_10_"+to_string(pctg)+"_test/test"+to_string(q)+".txt");
			string s;
			readFile>>M>>N;
//			cout << M << ' ' << N << '\n';
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
			
		    AquariumSolve(false);
		    
		    ofstream writeFile;
		    writeFile.open("BPPercentage/newaquarium_10_"+to_string(pctg)+"_result/test" + to_string(q) + "_res.txt");
		    writeFile << iternum << '\n';
			writeFile.close();
		}
	}
}
