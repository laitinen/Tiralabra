#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>

using namespace std;

typedef vector<vector<int> > VVI;

VVI glob;
vector<int> sol;
int selected = 0;

set<set<int> > sols;

void print(VVI& v) {
    for(int i = 0; i < v.size(); ++i) {
        for(int j = 0; j < v[i].size() - 1; ++j) {
            cout<<v[i][j]<<" ";
        }
        cout<<endl;
    }
}

void exact(VVI v) {
	if(v.size() == 0) {
		if(selected < glob[0].size() - 1) return;
		set<int> newelem;
		for(int i = 0; i < sol.size(); ++i) {
            newelem.insert(sol[i]);
        }
        sols.insert(newelem);
        /*
		cout<<"Solution:"<<endl;
        vector<int> sum(glob[0].size() - 1);
		for(int i = 0; i < sol.size(); ++i) {
            cout<<sol[i]<<endl;
            for(int j = 0; j < glob[sol[i]].size() - 1; ++j) {
                sum[j] += glob[sol[i]][j];
            }
        }
        for(int i = 0; i < sum.size(); ++i) {
            cout<<sum[i]<<" ";
        }
        cout<<endl;
        cout<<"---------"<<endl;
        */
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

            /*
               set<int>::iterator it;
               cout<<"Banned columns: "<<endl;
               for(it = dc.begin(); it != dc.end(); ++it) {
               cout<<*it<<" ";
               }
               cout<<endl;
               cout<<"Banned rows: "<<endl;
               for(it = dr.begin(); it != dr.end(); ++it) {
               cout<<*it<<" ";
               }
               cout<<endl;
             */

            /*
               for(int i = 0; i < v[r].size() - 1; ++i) {
               if(v[r][i]) {
               dc.insert(i);
               for(int j = 0; j < v.size(); ++j) {
               if(v[j][i]) dr.insert(j);
               }
               }
               }
             */

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


            /*
               print(v);
               cout<<"Removing row "<<r<<endl;
               print(nv);
             */

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

int main() {
    int R,C;
    cin>>R>>C;
    VVI V(R);
    for(int i = 0; i < R; ++i) {
        for(int j = 0; j < C; ++j) {
            int a;
            cin>>a;
            V[i].push_back(a);
        }
        V[i].push_back(i);
    }
    glob = V;
    exact(V);

    for(set<set<int> >::iterator it = sols.begin(); it != sols.end(); ++it) {
        cout<<"Solution: "<<endl;
        for(set<int>::iterator i = (*it).begin(); i != (*it).end(); ++i) {
            cout<<*i<<endl;
        }
        cout<<"--------"<<endl;
    }

    return 0;
}
