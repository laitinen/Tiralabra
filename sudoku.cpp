#include <iostream>
#include <vector>
#include <set>

using namespace std;

typedef vector<vector<int> > VVI;

VVI glob;
vector<int> sol;
int selected = 0;

set<set<int> > sols;

void exact(VVI v) {
	if(v.size() == 0) {
		if(selected < glob[0].size() - 1) return;
		set<int> newelem;
		for(int i = 0; i < sol.size(); ++i) {
            newelem.insert(sol[i]);
        }
        sols.insert(newelem);
        return;
    }

    for(int r = 0; r < v.size(); ++r) {
        if(v[r][0] == 1) {
            set<int> dc;
            set<int> dr;
            for(int j = 0; j < v[r].size() - 1; ++j) {
                if(v[r][j]) {
                    for(int i = 0; i < v.size(); ++i) {
                        if(v[i][j]) {
                            dr.insert(i);
                        }
                    }
                    dc.insert(j);
                }
            }

            VVI nv;

            for(int i = 0; i < v.size(); ++i) {
                if(dr.count(i)) continue;
                nv.resize(nv.size() + 1);
                for(int j = 0; j < v[i].size(); ++j) {
                    if(!dc.count(j)) {
                        nv.back().push_back(v[i][j]);
                    }
                }
            }

            int sel = 0;
            for(int i = 0; i < v[r].size() - 1; ++i) {
                if(v[r][i]) ++sel;
            }

            selected += sel;
            sol.push_back(v[r].back());
            exact(nv);
            sol.pop_back();
            selected -= sel;
        }
    }
}
vector<vector<int> > vec;
vector<vector<int> > S;

int region(int r, int c, int N) {
    int S = N == 16 ? 4 : (N == 9 ? 3 : 2);
    return r/S * S + c / S;
}

int main() {
	int N;
	cin>>N;

    S.resize(N);
    for(int i = 0; i < S.size(); ++i) S[i].resize(N);

    for(int i = 0; i < S.size(); ++i) {
        for(int j = 0; j < S[i].size(); ++j) {
            cin>>S[i][j];
        }
    }

    for(int r = 0; r < N; ++r) {
        for(int c = 0; c < N; ++c) {
        	for(int cand = 0; cand < N; ++cand) {
        		if(S[r][c] != 0 && S[r][c] != cand + 1) {
                    continue;
                }
        		vector<int> L(4*N*N + 1);
        		vec.push_back(L);
                
                vec.back().back() = r * N * N + c * N + cand;

                int reg = region(r,c,N);
                vec.back()[r*N + c] = 1;
                vec.back()[N*N + r*N + cand] = 1;
                vec.back()[2*N*N + 4*c + cand] = 1;
                vec.back()[3*N*N + 4*reg + cand] = 1;
            }
        }
    }

    for(int i = 0; i < vec.size(); ++i) {
        for(int j = 0; j < vec[i].size(); ++j) {
            cout<<vec[i][j]<<" ";
        }
        cout<<endl;
    }

    glob = vec;
    exact(vec);

    for(set<set<int> >::iterator it = sols.begin(); it != sols.end(); ++it) {
        cout<<"Solution: "<<endl;
        for(set<int>::iterator i = (*it).begin(); i != (*it).end(); ++i) {
        	cout<<*i<<endl;
        }
        cout<<"--------"<<endl;
    }

    for(int i = 0; i < vec.size(); ++i) {
        for(int j = 0; j < vec[i].size(); ++j) {
            cout<<vec[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
}
