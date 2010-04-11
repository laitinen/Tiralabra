#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>
#include <set>

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

header* PR;
set<int> ptrs;
set<int> hdrs;

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
        		vector<int> L(4*N*N);
        		vec.push_back(L);
                
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


void cover(node* head) {
	assert(head != NULL);
	assert(head->L != NULL);
	assert(head->R != NULL);

	cout<<"Called cover"<<endl;
    head->R->L = head->L;
    head->L->R = head->R;
    // how is an infinite loop even possible?
    for(node* i = head->D; i != head; i = i->D) {
        assert(!hdrs.count(int(i)));
    	assert(ptrs.count(int(i)));
        cout<<"new round"<<endl;
        assert(i != NULL);
        for(node* j = i->R; j != i; j = j->R) {
        	assert(!hdrs.count(int(j)));
            assert(ptrs.count(int(j)));
        	cout<<(int(j))<<endl;
        	cout<<"R, C: "<<j->row<<", "<<j->col<<endl;
        	//cout<<"mo2"<<endl;
        	assert(j != NULL);
        	assert(j->U != NULL);
        	assert(j->D != NULL);
            j->D->U = j->U;
            j->U->D = j->D;
        }
    }
}

void uncover(node* head) {
	assert(head != NULL);
	cout<<"Called uncover"<<endl;
    for(node* i = head->U; i != head; i = i->U) {
        for(node* j = i->L; j != i; j = j->L) {
        	assert(j != NULL);
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
	assert(root->R != NULL);
	if(root->R == root) {
		cout<<"zxx"<<endl;
        // print solution
        for(int i = 0; i < sol.size(); ++i) {
            cerr<<sol[i]<<", ";
        }
        cerr<<endl;
        halt = true;
        return;
    }
    //cout<<"po"<<endl;
    // choose column
    header* head = dynamic_cast<header*>(root->R);
    assert(head != NULL);

    cout<<"terve"<<endl;
    cover(head);
    cout<<"boo"<<endl;

    for(node* r = head->D; r != head; r = r->D) {
    	assert(r != NULL);
    	cout<<"you"<<endl;
        sol.push_back(r->row); 
        int k = ot.size();
        ot.push_back(r);
        for(node* j = r->R; j != r; j = j->R) {
        	assert(j != NULL);
            //cover(j);
            cover(j->C);
        }
        exact(root);
        if(halt) return;
        r = ot[k];
        head = r->C;
        for(node* j = r->L; j != r; j = j->L) {
        	assert(j != NULL);
            //uncover(j);
            uncover(j->C);
        }
        sol.pop_back();
    }

    cout<<"mo"<<endl;
    uncover(head);
    cout<<"ended"<<endl;
}

vector<vector<node*> > rows;

int main() {
	int N,M;
	cin>>N>>M;

    VVI ecm(N,vector<int>(M));

    for(int i = 0; i < N; ++i)
    	for(int j = 0; j < M; ++j)
            cin>>ecm[i][j];


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
        for(int j = 0; j < ecm[0].size(); ++j) {
            cout<<ecm[i][j]<<" ";
        }
        cout<<endl;
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


    int counter = 0;
    for(int i = 0; i < headers.size(); ++i) {
        header* head = headers[i];
        for(node* n = head->D; n != head; n = n->D) {
            ++counter; 
        }
        hdrs.insert(int(head));
    }

    for(int i = 0; i < matrix.size(); ++i) {
        for(int j = 0; j < matrix[i].size(); ++j) {
            if(matrix[i][j] != NULL) {
            	ptrs.insert(int(matrix[i][j]));
                assert(matrix[i][j]->D != NULL);
                assert(matrix[i][j]->U != NULL);
                assert(matrix[i][j]->L != NULL);
                assert(matrix[i][j]->R != NULL);
                assert(matrix[i][j]->C != NULL);
            }
        }
    }

    set<int> used;

    used.insert(int(root)); 

    vector<node*> upc;

    upc.push_back(root->R);

    int ind = 0;


    while(ind < upc.size()) {
    	assert(upc.back() != NULL);
    	assert(hdrs.count(int(upc.back())) + ptrs.count(int(upc.back())));
        if(used.count(int(upc.back()))) {
            ++ind;
            continue;
        }

        used.insert(int(upc.back()));

        assert(upc.back()->R != NULL);
        assert(upc.back()->L != NULL);
        assert(upc.back()->D != NULL);
        assert(upc.back()->U != NULL);
        //assert(upc.back()->C != NULL);

        upc.push_back(upc.back()->R);
        upc.push_back(upc.back()->L);
        upc.push_back(upc.back()->U);
        upc.push_back(upc.back()->D);
        //upc.push_back(upc.back()->C);

    }


    exact(root);

    return 0;
}
