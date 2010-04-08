#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

using namespace std;

int N;

vector<vector<int> > S;

vector<vector<int> > RE;
vector<vector<int> > RO;
vector<vector<int> > CO;

vector<pair<int,int> > next;

int region(const pair<int,int>& sq) {
	int c = sq.second;
	int r = sq.first;
	int S = N == 16 ? 4 : (N == 9 ? 3 : 2);
    return r / S * S + c / S;
}

bool valid(const pair<int,int>& sq, int C) {
    return !RE[region(sq)][C] && !RO[sq.first][C] && !CO[sq.second][C];
}

void set(const pair<int,int>& sq, int C) {
    RE[region(sq)][C] = 1;
    RO[sq.first][C] = 1;
    CO[sq.second][C] = 1;
    S[sq.first][sq.second] = C;
}

void unset(const pair<int,int>& sq, int C) {
    RE[region(sq)][C] = 0;
    RO[sq.first][C] = 0;
    CO[sq.second][C] = 0;
    S[sq.first][sq.second] = 0;
}

bool halt = false;

void solve(int ind) {
    if(ind == next.size()) {
        halt = true;
        return;
    }

    for(int i = 1; i <= N; ++i) {
    	int C = i;
        if(valid(next[ind],C)) {
        	set(next[ind],C);
        	solve(ind+1);
        	if(halt) return;
        	unset(next[ind],C);
        }
    }
}

int main() {
	cin>>N;
	S.resize(N);
	RE.resize(N+1);
	RO.resize(N+1);
	CO.resize(N+1);
	for(int i = 0; i < N; ++i) {
        S[i].resize(N);
        RE[i].resize(N+1);
        RO[i].resize(N+1);
        CO[i].resize(N+1);
        for(int j = 0; j < S[i].size(); ++j) {
            cin>>S[i][j];
        }
    }

    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            if(!S[i][j]) next.push_back(make_pair(i,j));
        }
    }

    for(int i = 0; i < N; ++i) {
        for(int j = 0; j < N; ++j) {
            RE[region(make_pair(i,j))][S[i][j]] = 1;
            RO[i][S[i][j]] = 1;
            CO[j][S[i][j]] = 1;
        }
    }

    solve(0);

    for(int i = 0; i < S.size(); ++i) {
        for(int j = 0; j < S[i].size(); ++j) {
            cout<<S[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
}
