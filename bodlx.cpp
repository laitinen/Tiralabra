#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <set>
#include <map>


using namespace std;

typedef vector<vector<int> > VVI;

struct header;

struct node {
    node* U;
    node* D;
    node* L;
    node* R;
    header* C;

    int row;
    int col;

    virtual ~node() {}
};

struct header : node {
    int size;
    ~header() {}
};

int N;
header* PR;

int cover_called = 0;

vector<vector<int> > given_solution;

set<int> hdrs;
map<int,int> row_cand;

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

        		row_cand[vec.size()] = r*N*N + c*N + cand;

        		vector<int> L(4*N*N);
        		vec.push_back(L);

                
                int reg = region(r,c,N);
                vec.back()[r*N + c] = 1;
                vec.back()[N*N + r*N + cand] = 1;
                vec.back()[2*N*N + N*c + cand] = 1;
                vec.back()[3*N*N + N*reg + cand] = 1;
            }
        }
    }

    return vec;
}


void cover(node* head) {
	++cover_called;
    head->R->L = head->L;
    head->L->R = head->R;
    // how is an infinite loop even possible?
    for(node* i = head->D; i != head; i = i->D) {
        for(node* j = i->R; j != i; j = j->R) {
            j->D->U = j->U;
            j->U->D = j->D;
            --j->C->size;
        }
    }
}

void uncover(node* head) {
    for(node* i = head->U; i != head; i = i->U) {
        for(node* j = i->L; j != i; j = j->L) {
            j->D->U = j;
            j->U->D = j;
            ++j->C->size;
        }
    }
    head->R->L = head;
    head->L->R = head;
}

vector<int> sol;
vector<node*> ot;
bool halt = false;

void exact(header* root) {
	if(root->R == root) {
        // print solution
        vector<vector<int> > solved(N,vector<int>(N));
        for(int i = 0; i < sol.size(); ++i) {
        	int V = row_cand[sol[i]];
        	int r = V / N / N;
        	int c = (V - r*N*N) / N;
        	int cand = V - c*N - r*N*N;
        	solved[r][c] = cand + 1;
        }

        for(int i = 0; i < solved.size(); ++i) {
            for(int j = 0; j < solved[i].size(); ++j) {
                cout<<solved[i][j]<<" ";
            }
            cout<<endl;
        }

        halt = true;
        return;
    }
    // choose column
    header* head = dynamic_cast<header*>(root->R);
    int val = head->size;
    header* cur = dynamic_cast<header*>(head->R);

    while(cur != root) {
    	if(cur->size < val) {
            val = cur->size;
            head = cur;
        }
        cur = dynamic_cast<header*>(cur->R);
    }
    
    cover(head);

    for(node* r = head->D; r != head; r = r->D) {
        sol.push_back(r->row); 
        int k = ot.size();
        ot.push_back(r);
        for(node* j = r->R; j != r; j = j->R) {
            cover(j->C);
        }
        exact(root);
        if(halt) return;
        r = ot[k];
        head = r->C;
        for(node* j = r->L; j != r; j = j->L) {
            uncover(j->C);
        }
        sol.pop_back();
    }

    uncover(head);
}

vector<vector<node*> > rows;

int main(int argc, char** argv) {
	cin>>N;
	VVI S(N,vector<int>(N));

    for(int i = 0; i < N; ++i)
    	for(int j = 0; j < N; ++j)
            cin>>S[i][j];

	if(argc > 1) {
        ifstream fin(argv[1]);
        given_solution.resize(N);
        for(int i = 0; i < N; ++i) {
            given_solution[i].resize(N);
            for(int j = 0; j < N; ++j) {
                fin>>given_solution[i][j];
            }
        }
    }

    VVI ecm = exactify(S,N); // exact cover matrix

    // initialize root node

    header* root = new header;
    root->L = root;
    root->R = root;

    PR = root;

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
    
    rows.resize(ecm.size());
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
                nd->row = i;
                nd->col = j;
                if(!rows[i].size()) {
                    nd->L = nd;
                    nd->R = nd;
                } else {
                    nd->R = rows[i][0];
                    nd->L = rows[i].back();
                    rows[i][0]->L = nd;
                    rows[i].back()->R = nd;
                }
                nd->C = headers[j];
                rows[i].push_back(nd);
                matrix[i][j] = nd;
            }
        }
    }

    // rows not needed any more

    for(int x = 0; x < matrix[0].size(); ++x) {
    	node* prev = headers[x];
        for(int y = 0; y < matrix.size(); ++y) {
            if(matrix[y][x] != NULL) {
                matrix[y][x]->U = prev;
                matrix[y][x]->D = headers[x];
                prev->D = matrix[y][x];
                headers[x]->U = matrix[y][x];
                prev = matrix[y][x];
            }
        }
    }


    for(int i = 0; i < headers.size(); ++i) {
    	int counter = 0;
        header* head = headers[i];
        for(node* n = head->D; n != head; n = n->D) {
            ++counter; 
        }
        head->size = counter;
    }

    exact(root);

    return 0;
}
