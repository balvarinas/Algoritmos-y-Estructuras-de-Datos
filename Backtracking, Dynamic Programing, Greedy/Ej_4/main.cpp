#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
using namespace std;

constexpr int INFTY = numeric_limits<int>::max() - 1;
vector <int> v;
int n, k;
vector<vector<vector<pair<int,vector<int>>>>> memo;

int costo(const vector <int> & v, int ant, int i)
{
    int res = 0;
    if (ant == -1) {
        for (int k = 0; k < i; k++) {
            res += abs(v[k] - v[i]);
        }
    } else {
        for (int k = ant; k < i; k++) {
            int mitad = (v[ant] + v[i]) / 2;
            if (v[k] <= mitad) res += (int) ceil(abs(v[k] - v[ant]));
            else res += (int) ceil(abs(v[i] - v[k]));
        }
    }

    return res;
}


int costo2(const vector<int> & v, int ant)
{
    int n = (int) v.size();
    int res = 0;
    for (int k = ant; k < n; k++) {
        res += abs(v[k] - v[ant]);
    }

    return res;
}
vector<int> provedurias;
pair<int,vector<int>> choris(const vector<int> & v, int i, int j, int ant)
{
    int n = v.size();
    if (j < 0) {
        return make_pair(100000,vector<int>());
    } else if (i == n) {
        return make_pair(costo2(v, ant),provedurias);
    } else if(memo[i][j][ant+1].first != -1) {
        return memo[i][j][ant+1];
    } else{
        int pongo = choris(v, i + 1, j - 1, i).first + costo(v, ant, i);
        int no_pongo = choris(v, i + 1, j, ant).first;
        if(no_pongo >= pongo){
            memo[i][j][ant+1].first = pongo;
            provedurias.push_back(v[i]);
            memo[i][j][ant+1].second = provedurias;
            provedurias.pop_back();
        } else{
            memo[i][j][ant+1].first = no_pongo;
            memo[i][j][ant+1].second = provedurias;
        }
        return memo[i][j][ant+1];
    }
}


int main() {
    int cases;
    cin >> cases;
    for (int cs = 0; cs < cases; cs++) {
        cin >> n >> k;
        v.assign(n, 0);
        for (auto &p: v) cin >> p;
        memo.resize(n, vector<vector<pair<int, vector<int>>>>(k + 1, vector<pair<int, vector<int>>>(n + 1, make_pair(-1, vector<int>()))));
        pair<int,vector<int>> costo_optimo = choris(v, 0, k, -1);
        vector<int> provedurias2 ;
        for(int i=0;i<n;i++){
            for(int j = 0;j<k;j++){
                for(int ant= i-1; ant<n;ant++){
                    if (memo[i][j][ant+1].first == costo_optimo.first){
                        provedurias2.push_back(v[i]);
                    }
                }
            }
        }


       // cout << costo_optimo.first << endl;
        for(int i=0;i< provedurias2.size();i++){
            cout << provedurias2[i] << " ";
        }


    }

    return 0;
}