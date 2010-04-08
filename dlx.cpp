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
    header* C;

    int row;

    virtual ~node() {}
};

struct header : node {
    int size;
    ~header() {}
};

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


void cover(header* head) {
    head->R->L = head->L;
    head->L->R = head->R;
    for(node* i = head->D; i != head; i = i->D) {
        for(node* j = i->R; j != i; j = j->R) {
            j->D->U = j->U;
            j->U->D = j->D;
        }
    }
}

void uncover(header* head) {
    for(node* i = head->U; i != head; i = i->U) {
        for(node* j = i->L; j != i; i = i->L) {
            j->D->U = j;
            j->U->D = j;
        }
    }
    head->R->L = head;
    head->L->R = head;
}

vector<int> sol;
vector<node*> ot;
bool halt = false;

void exact(header* root) {
	cout<<"jou"<<endl;
	if(root->R == root) {
		//cout<<"zxx"<<endl;
        // print solution
        for(int i = 0; i < sol.size(); ++i) {
            cout<<sol[i]<<", ";
        }
        cout<<endl;
        halt = true;
        return;
    }
    //cout<<"po"<<endl;
    // choose column
    header* head = dynamic_cast<header*>(root->R);

    cover(head);

    for(node* r = head->D; r != head; r = r->D) {
    	//cout<<"you"<<endl;
        sol.push_back(r->row); 
        int k = ot.size();
        ot.push_back(r);
        for(node* j = r->R; j != r; j = j->R) {
            cover(dynamic_cast<header*>(j));
        }
        exact(root);
        if(halt) return;
        r = ot[k];
        head = r->C;
        for(node* j = r->L; j != r; r = r->L) {
            uncover(dynamic_cast<header*>(j));
        }
        sol.pop_back();
    }

    uncover(head);
}

vector<vector<node*> > rows;

int main() {
	int N;
	cin>>N;
	VVI S(N,vector<int>(N));

    for(int i = 0; i < N; ++i)
    	for(int j = 0; j < N; ++j)
            cin>>S[i][j];

    VVI ecm = exactify(S,N); // exact cover matrix

    // initialize root node

    header* root = new header;
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
                if(!rows[i].size()) {
                    nd->L = nd;
                    nd->R = nd;
                } else {
                    nd->R = rows[i][0];
                    nd->L = rows[i].back();
                }
                nd->C = headers[i];
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

    exact(root);

    return 0;
}
