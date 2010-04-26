#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <cstdlib>
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

        		vec.resize(vec.size()+1);

                int reg = region(r,c,N);
                vec.back().push_back(r*N + c);
                vec.back().push_back(N*N + r*N + cand);
                vec.back().push_back(2*N*N + N*c + cand);
                vec.back().push_back(3*N*N + N*reg + cand);
            }
        }
    }
    return vec;
}


void cover(node* head) {
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
        for(int i = 0; i < int(sol.size()); ++i) {
        	int V = row_cand[sol[i]];
        	int r = V / N / N;
        	int c = (V - r*N*N) / N;
        	int cand = V - c*N - r*N*N;
        	solved[r][c] = cand + 1;
        }

        for(int i = 0; i < int(solved.size()); ++i) {
            for(int j = 0; j < int(solved[i].size()); ++j) {
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

int main() {
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

    for(int i = 0; i < N*N*4; ++i) {
    	header* nh = new header;
    	headers.push_back(nh);

    	nh->D = nh;
    	nh->U = nh;

    	nh->L = root->L;
    	root->L->R = nh;
    	root->L = nh;
    	nh->R = root;
    }

    int cc = 0;
    for(int i = 0; i < int(ecm.size()); ++i) {
    	node* first;
    	node* prev = new node;
    	first = prev;
    	prev->R = prev;
    	prev->L = prev;
    	prev->D = headers[ecm[i][0]];
    	prev->U = headers[ecm[i][0]]->U;
    	prev->U->D = prev;
    	prev->C = headers[ecm[i][0]];
    	prev->row = i;
    	prev->col = ecm[i][0];
    	headers[ecm[i][0]]->U = prev;
        for(int j = 1; j < int(ecm[i].size()); ++j) {
            node* nd = new node;
            nd->R = prev->R;
            nd->L = prev;
            prev->R->L = nd;
            prev->R = nd;
            prev = nd;
            prev->C = headers[ecm[i][j]];
            prev->D = headers[ecm[i][j]];
            prev->U = headers[ecm[i][j]]->U;
            prev->U->D = prev;
            prev->row = i;
            prev->col = ecm[i][j];
            headers[ecm[i][j]]->U = prev;
        }

        ++cc;
        for(node* f = first->R; f != first; f = f->R) {
            ++cc;
        }
    }

    int count = 0;

    for(int i = 0; i < int(headers.size()); ++i) {
    	int counter = 0;
        header* head = headers[i];
        for(node* n = head->D; n != head; n = n->D) {
            ++counter; 
        }
        count += counter;
        head->size = counter;
    }

    exact(root);

    return 0;
}
