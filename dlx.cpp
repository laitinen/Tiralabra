#include <iostream>
#include <vector>

using namespace std;

typedef vector<vector<int> > VVI;

struct header;

struct node {
    node* U;
    node* D;
    node* L;
    node* R;
    header* head;
};

struct header : node {
    int size;
};

header* root;

int region(int r, int c, int N) {
    int S = N == 16 ? 4 : (N == 9 ? 3 : 2);
    return r/S * S + c / S;
}

VVI exactify(const VVI& S, const int N) {
	VVI vec;
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
    return vec;
}

int main() {
	int N;
	cin>>N;
	VVI S(N,vector<int>(N));

    for(int i = 0; i < N; ++i)
    	for(int j = 0; j < N; ++j)
            cin>>S[i][j];

    VVI ecm = exactify(S,N); // exact cover matrix

    // initialize root node

    root = new header;
    root->L = root;
    root->R = root;

    // initialize header nodes

    vector<header*> headers;

    for(int i = 0; i < ecm[0].size(); ++i) {
    	header* nh = new header;
    	headers.push_back(nh);

    	nh->D = nh;
    	nh->U = nh;

    	nh->L = root->L;
    	root->L->R = nh;
    	root->L = nh;
    	nh->R = root;
    }


    // initialize rows
    
    vector<vector<node*> > rows(ecm.size());
    vector<vector<node*> > matrix(ecm.size(),vector<node*>(ecm[0].size()));

    for(int i = 0; i < matrix.size(); ++i) {
        for(int j = 0; j < matrix[i].size(); ++j) {
            matrix[i][j] = NULL;
        }
    }

    for(int i = 0; i < ecm.size(); ++i) {
    	for(int j = 0; j < ecm[i].size(); ++j) {
            if(ecm[i][j]) {
                node* nd = new node;
                if(!rows[i].size()) {
                    nd->L = nd;
                    nd->R = nd;
                } else {
                    nd->R = rows[i][0];
                    nd->L = rows[i].back();
                }
                rows[i].push_back(nd);
                matrix[i][j] = nd;
            }
        }
    }

    // rows not needed any more

    for(int x = 0; x < matrix[0].size(); ++x) {
    	int found = 0;
    	node* prev;
        for(int y = 0; y < matrix.size(); ++y) {
            if(matrix[y][x] != NULL) {
                if(!found) {
                    matrix[y][x]->U = headers[x];
                    matrix[y][x]->D = headers[x];
                } else {
                    matrix[y][x]->U = prev;
                    matrix[y][x]->D = headers[x];
                }
            	prev = matrix[y][x];
            }
        }
    }

    // 


    return 0;
}
