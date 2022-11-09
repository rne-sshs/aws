#include <bits/stdc++.h>
#define double long double
#define EPSILON 0
const int MaxN = 50;
const int MaxM = 50;
int N, M;
using namespace std;

int Land[MaxN*MaxM];

double R[MaxN+MaxM][MaxN*MaxM][MaxM], Q[MaxN+MaxM][MaxN*MaxM][MaxM], QQ[MaxN*MaxM][MaxM];
int X[MaxN+MaxM];
set<int> NC[MaxN+MaxM], NS[MaxN*MaxM];
int Intersect[MaxN+MaxM][MaxN*MaxM];
vector<int> Cells[MaxN*MaxM];
double P[MaxN*MaxM][MaxM] = {0,};
int E[MaxN*MaxM] = {0,};
int H[MaxN*MaxM], B[MaxN*MaxM];
int TB[MaxN+MaxM][MaxN*MaxM][2];
vector<int> TC[MaxN+MaxM][MaxN*MaxM];

int HQ[MaxN*MaxM] = {0,};
int Board[MaxN*MaxM] = {0,};
int lefts[10010] = {0,};
int minlefts[10010] = {0,};
int BoardHistory[10010][MaxN*MaxM] = {0,};

int iternum = 0;
int lastSelect = -1;
int landcnt = 0;

bool isended = false;

void initAll() {
	for(int i = 0; i<MaxM+MaxN; i++) {
		for(int j = 0; j<MaxM*MaxN; j++) {
			for(int k = 0; k<MaxM; k++) {
				R[i][j][k] = 0;
				Q[i][j][k] = 0;
			}
			Intersect[i][j] = 0;
			TB[i][j][0] = 0; TB[i][j][1] = 0;
			TC[i][j].clear();
		}
		for(int k = 0; k<MaxM; k++) {
			QQ[i][k] = 0;
		}
		X[i] = 0;
		NC[i].clear();
	}
	for(int i = 0; i<MaxM*MaxN; i++) {
		NS[i].clear();
		for(int k = 0; k<MaxM; k++) {
			P[i][k] = 0;
		}
		E[i] = 0;
		Board[i] = 0;
		Land[i] = 0;
		B[i] = 0;
		Cells[i].clear();
		H[i] = 0; HQ[i] = 0;
	}
	
	for(int i = 0; i<10000; i++) {
		lefts[i] = 0;
		minlefts[i] = 0;
		for(int j = 0; j<MaxM*MaxN; j++) {
			BoardHistory[i][j] = 0;
		}		
	}
	iternum = 0;
	lastSelect = -1;
	landcnt = 0;
	isended = false;
}

void initN() {
	for(int i = 0; i < N*M; i++) Land[i]--;
	for(int i = 0; i<N*M; i++) {
		Cells[Land[i]].push_back(i);
		if(landcnt<Land[i]) landcnt = Land[i];
	}
	landcnt++;
	for(int i = 0; i<N; i++) {
		for(int j = 0; j<M; j++) {
			NC[i].insert(Land[i*M+j]);
			NS[Land[i*M+j]].insert(i);
			Intersect[i][Land[i*M+j]]++;
		}
	}
	for(int i = 0; i<M; i++) {
		for(int j = 0; j<N; j++) {
			NC[i+N].insert(Land[i+j*M]);
			NS[Land[i+j*M]].insert(i+N);
			Intersect[i+N][Land[i+j*M]]++;
		}
	}
	
	for(int i = 0; i<landcnt; i++) {
		H[i] = Cells[i][Cells[i].size()-1]/M-Cells[i][0]/M+1;
		B[i] = Cells[i][Cells[i].size()-1]/M;
	}
	
	for(int i = 0; i<N; i++) {
		for(int x: NC[i]) {
			TB[i][x][0] = B[x]-i+1;
			TB[i][x][1] = B[x]-i+1;
		}
	}
	
	for(int i = 0; i<M; i++) {
		for(int x: NC[N+i]) {
			int minh = -1, maxh = -1;
			for(int ce: Cells[x]) {
				if(ce%M==i) {
					if(minh==-1 | minh>ce/M) minh = ce/M;
					if(maxh==-1 | maxh<ce/M) maxh = ce/M;
				}
			}
			TB[N+i][x][0] = minh-B[x]+1;
			TB[N+i][x][1] = maxh-B[x]+1;
		}
	}
	for(int i = N-1; i >=0; i--) {
		for(int j = 0; j < M; j++) {
			TC[N+j][Land[M*i+j]].push_back(B[Land[M*i+j]]-i+1);
		}
	}
}

void initP() {
    for(int i = 0; i<landcnt; i++)
    {
    	for(int j = 0; j<N+1; j++) {
    		if(j<=H[i]) P[i][j] = 1;
    		else P[i][j] = 0;
		}
    }
    for(int i = 0; i < N + M; i++)
    {
        for(int j = 0; j < landcnt; j++)
        {
        	for(int k = 0; k<=H[j]; k++) {
        		Q[i][j][k] = 1;
			}
        }
    }
}

vector<int> arrangeV;
void next_arrangement(int m) {
    int pt = -1;
    int cnt = arrangeV.size();
    int cnt2 = 0;
    for(int i = cnt-1; i>=0; i--) {
        if(i != cnt-1 && arrangeV[i] == 1 && arrangeV[i+1] == 0) {
            pt = i;
            break;
        }
        if(arrangeV[i]==1) cnt2++;
    }
    if(pt==-1) {
        arrangeV.clear();
        arrangeV.push_back(-1);
        return;
    }
    arrangeV[pt] = 0, arrangeV[pt+1] = 1;
    for(int i = pt + 2; i < pt+2+cnt2; i++) {
        arrangeV[i] = 1;
    }
    for(int i=pt+2+cnt2;i<cnt;i++) arrangeV[i] = 0;
}

vector<int> arrangeH;
void next_arrangementH(int m) {
    int pt = -1;
    int cnt = arrangeH.size();
    for(int i=cnt-1;i>=0;i--) {
        if(arrangeH[i] != 1) {
            pt = i;
            break;
        }
    }
    if(pt==-1) {
        arrangeH.clear();
        arrangeH.push_back(-1);
        return;
    }
    arrangeH[pt]++;
    for(int i = pt + 1; i < cnt; i++) {
        arrangeH[i] = 0;
    }
}

vector<int> row;
void genArr(int m, bool horizontal) {
    int pt = 0;
    for (int i = 0; i < (horizontal ? M : N); i++)
    {
        if (pt < arrangeV.size() && i == arrangeV[pt])
        {
            row[i] = 1;
            pt++;
        }
        else row[i] = 0;
    }
}

void printVec(vector<int> vec) {
    for (auto it : vec)
    {
        printf("%d ", it);
    }
    printf("\n");
    return;
}

vector<int> per;
bool satisfy(int res) {
	if(per.size()!=arrangeH.size()) return false;
	int perres = 0;
	for(int i = 0; i<per.size(); i++) {
		perres += per[i] * arrangeH[i];
	}
	return perres == res;
}

bool satisfy2(int res, int m) {
	int cnt = 0;
	for(int i = 0; i < arrangeV.size();i++) {
		cnt += arrangeV[i];
	}
	//cout << cnt << '\n';
	if(cnt != res) return false;
	for(int n : NC[m]) {
		for(int i = 0; i + 1 < TC[m][n].size();i++) {
			if(arrangeV[B[n]-TC[m][n][i]+1]==0&&arrangeV[B[n]-TC[m][n][i+1]+1]==1) return false;
		}
	}
	return true;
}


bool checkConverge() {
    if(iternum == 0) minlefts[iternum] = lefts[iternum];
    else minlefts[iternum] = minlefts[iternum - 1] < lefts[iternum] ? minlefts[iternum - 1] : lefts[iternum];
    if(iternum < 20) return false;
    if(iternum - lastSelect < 10) return false;
    return minlefts[iternum] == minlefts[iternum - 10];
}

int ConsSum1() {
	int sum1 = 0;
    for(int i=0;i<N;i++) {
        sum1 += X[i];
    }
    return sum1;
}

int ConsSum2() {
	int sum2 = 0;
    for(int i=N;i<N+M;i++) {
        sum2 += X[i];
    }
    return sum2;
}

bool checkCons() {
    return ConsSum1() == ConsSum2();
}

vector<string> split(string str, char delimiter) {
    vector<string> ans;
    stringstream ss(str);
    string temp;
    while(getline(ss,temp,delimiter)) {
    	ans.push_back(temp);
    }
    return ans;
}



void IterSolve(bool willPrint) {
    for(int m = 0; m < N; m++) {
        // R update
        for(int r: NC[m]) {
        	for(int k = 0; k<=H[r]; k++) {
        		R[m][r][k] = 0;
			}
        }

        arrangeH.clear();
        per.clear();
        for(int r: NC[m]) {
            arrangeH.push_back(0);
            per.push_back(Intersect[m][r]);
        }
       	while(true) {
        	if(satisfy(X[m])) {
	            int cnt = 0;
	            double val = 1;
	            vector<double> qsums;
	            int id2 = 0;
	            for(int r: NC[m]) {
	                double qsum = 0;
	                if(arrangeH[id2]==1) {
	                	for(int k = TB[m][r][0]; k<=H[r]; k++) {
	                		qsum += Q[m][r][k];
						}
					}
					else {
						for(int k = 0; k<TB[m][r][0]; k++) {
                			qsum += Q[m][r][k];
						}
					}
	                id2++;
	                val *= qsum;
	                qsums.push_back(qsum);
	            }
	
	            if(val>EPSILON) {
	            	int id = 0;
	                for(int r: NC[m]) {
	                	if(arrangeH[id]==1) {
	                		for(int k = TB[m][r][0]; k<=H[r]; k++) {
	                			R[m][r][k] += val / qsums[id];
							}
						}
						else {
							for(int k = 0; k<TB[m][r][0]; k++) {
	                			R[m][r][k] += val / qsums[id];
							}
						}
	                    id++;
	                }
	            }
			}
            next_arrangementH(m);

            if(arrangeH[0] == -1) break;
        }

        for(int r: NC[m]) {
            double sumR = 0;
            for(int k = 0; k<=H[r]; k++) {
            	sumR += R[m][r][k];
			}
            if(sumR > EPSILON) {
                for(int k = 0; k<=H[r]; k++) {
	            	R[m][r][k] /= sumR;
				}
            }
        }
    }
    
    for(int m = N; m < N+M; m++) {
        // R update2
        for(int r: NC[m]) {
        	for(int k = 0; k<=H[r]; k++) {
        		R[m][r][k] = 0;
			}
        }
        arrangeV.clear();
        per.clear();
        for(int i = 0; i < X[m]; i++) arrangeV.push_back(1);
        for(int i = X[m]; i < N; i++) arrangeV.push_back(0);
        while(1) {
        	if(satisfy2(X[m], m)) {
	            int cnt = 0;
	            double val = 1;
	            vector<double> qsums;
	            vector<int> mis, mas;
	            int id2 = 0;
	            for(int n: NC[m]) {
	                double qsum = 0;
	                int mi = 0, ma = H[n] + 1;
					for(int i = 0; i< TC[m][n].size();i++) {
						if(arrangeV[B[n]-TC[m][n][i]+1]==0) {
							ma = min(ma, TC[m][n][i]);
						}
						else {
							mi = max(mi, TC[m][n][i]);
						}
					}
					for(int k = mi; k < ma; k++) {
						qsum += Q[m][n][k];
					}
	                id2++;
	                val *= qsum;
	                mis.push_back(mi);
	                mas.push_back(ma);
	                qsums.push_back(qsum);
	            }
	            if(val>EPSILON) {
	            	int id = 0;
	                for(int r: NC[m]) {
	                	for(int k = mis[id]; k < mas[id]; k++) {
	                		R[m][r][k] += val / qsums[id];
						}
	                    id++;
	                }
	            }
			}
            next_arrangement(m);
            if(arrangeV[0] == -1) break;
        }
        for(int r: NC[m]) {
            double sumR = 0;
            for(int k = 0; k<=H[r]; k++) {
            	sumR += R[m][r][k];
			}
            if(sumR > EPSILON) {
                for(int k = 0; k<=H[r]; k++) {
	            	R[m][r][k] /= sumR;
				}
            }
        }
    }
    // cout << "2 done\n";
    
    // Q update
    for(int n = 0; n < landcnt; n++) {
        for(int k = 0; k<=H[n]; k++) {
            double sumQ = 0;
            double res = 1;
            for(int m: NS[n]) {
        		res *= R[m][n][k];
			}
			res *= P[n][k];
            for(int m: NS[n]) {
            	Q[m][n][k] = res / R[m][n][k];
                sumQ += Q[m][n][k];
            }

            if(sumQ >EPSILON) {
            	for(int m: NS[n]) {
            		Q[m][n][k] /= sumQ;
				}
            }
        }
    }

    bool fl = false;

    for(int n = 0; n < landcnt; n++) {
    	double sumQQ = 0;
    	for(int k = 0; k<=H[n]; k++) {
    		QQ[n][k] = 1;
    		for(int x: NS[n]) {
    			QQ[n][k] *= R[x][n][k];
			}
			QQ[n][k] *= P[n][k];
			sumQQ += QQ[n][k];
		}
        if(sumQQ != 0) {
        	for(int k = 0; k<=H[n]; k++) {
        		QQ[n][k] /= sumQQ;
			}
        }
    }
    int left = 0;
    for(int n = 0; n < landcnt; n++) {
    	bool fl = false;
    	for(int k = 0; k<=H[n]; k++) {
    		if(0.99<QQ[n][k] && QQ[n][k]<=1) {
    			E[n] = 1;
    			fl = true;
    			break;
			}
		}
		if(!fl) {
			E[n] = 0;
			left++;
		}
    }
    if(willPrint) printf("%d\n", left);

	for(int i = 0; i<landcnt; i++) {
		int maxj = 0;
		for(int j = 0; j<=H[i]; j++) {
			if(QQ[i][maxj]<QQ[i][j]) maxj=j;
		}
		HQ[i] = maxj;
		for(int x: Cells[i]) {
			if(x/M>B[i]-maxj) Board[x] = 1;
			else Board[x] = 0;
		}
	}

    lefts[iternum] = left;

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < M; j++) {
            BoardHistory[iternum][i * M + j] = Board[i * M + j];
        }
    }
    
    if(checkConverge()) {
    	isended = true;
        for(int n = 0; n < landcnt; n++) {
        	if(E[n] == 1) continue;
            if(E[n] == 0) {
            	int maxk = 0;
            	for(int k = 0; k<=H[n]; k++) {
            		if(QQ[n][maxk] < QQ[n][k]) maxk = k;
				}
                for(int k = 0; k<=H[n]; k++) {
                	if(k==maxk) P[n][k] = 1;
                	else P[n][k] = 0;
				}
                if(willPrint) cout << "!!\n";
                lastSelect = iternum;
                break;
            }
        }
    }
	
    iternum++;
}

void AquariumSolve(bool willPrint) {
    initN();
    initP();
    while(true) {
        IterSolve(willPrint);
        if (iternum > 500 || lefts[iternum - 1] == 0 || (iternum >= 100 && lefts[iternum - 1] == landcnt))
            break;
//        for(int i = 0; i<N; i++) {
//	        for(int j = 0; j<M; j++) {
//	            string ma = "бр";
//	            if(Board[i*M+j]==1) ma = "бс";
//	            else if(Board[i*M+j]==2) ma = "Y ";
//	            cout << ma;
//	        }
//	        cout << '\n';
//	    }
    }

    iternum -= 1;

    int minleftiter = 0;
    for (int i = 0; i <= iternum; i++)
    {
        if (lefts[i] <= lefts[minleftiter])
            minleftiter = i;
    }

    if (lefts[iternum] == 0)
        minleftiter = iternum;

    for (int i = 0; i < N * M; i++)
    {
        Board[i] = BoardHistory[minleftiter][i];
    }

    return;
}



