#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <tuple>
using namespace std;

//vector<vector<bool>> memo;

bool saldoPosible(vector<int> v,int i, int final,int l,vector<vector<bool>> &memo) {
    bool suma, resta = false;
    int n = v.size();
    if (final == 0 && i == n) {
        return true;
    } else if (final != 0 && i == n) {
        return false;
    }
    if(memo[i][final+l]) {
        return true;
    } else{
        suma = saldoPosible(v, i + 1, final + v[i],l,memo);
        resta = saldoPosible(v, i + 1, final - v[i],l,memo);
    } memo[i][final+l] = suma || resta;
    return memo[i][final+l];
}

int main() {
    int cases;
    cin >> cases;
    for (int k = 0; k < cases; k++) {
        int n, final;
        cin >> n >> final;
        int rango = 0;
        final = final / 100;
        vector<int> v;
        for (int j = 0; j < n; j++) {
            int a;
            cin >> a;
            rango += a;
            v.push_back(a / 100);
        }

        int max_rango = 0;
        if (2 * rango + 1 < 1000) {
            max_rango = 2 * rango + 1;;
        } else {
            max_rango = 1000;
        }
        //vector<vector<bool>> memo(n, vector<bool>(max_rango, false));

        for (int i = 0; i < n; i++) {
            bool suma,resta = false;
            vector<vector<bool>> memo(n, vector<bool>(max_rango, false));
            swap(v[0], v[i]);
             suma = saldoPosible(v, 1, final-v[0], (max_rango/2)+1,memo);
             resta = saldoPosible(v,1,final+ v[0],(max_rango/2)+1,memo);
            if(suma && resta){
                cout << '?';
            }else if(suma){
                cout << '+';
            }else if(resta){
                cout << '-';
            }
        }
        cout << endl;
    }

    return 0;
}


/*
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <tuple>
using namespace std;

vector<vector<pair<int,int>>> memo;


pair<int,int> saldoPosible(vector<int> v,int i, int final,int l) {
    pair<int,int> flag = make_pair(-1,-1);
    int n = v.size();
    if (final == 0 && i == n) {
        flag = make_pair(1,0); //suma
    } else if (final != 0 && i == n) {
        flag = make_pair(0,1); //resta
    }
    if((memo[i][final+l].first && memo[i][final+l].second )!= -1){
        return memo[i][final+l];
    }else {
        pair<bool, bool> resta = saldoPosible(v, i + 1, final + v[i], l);
        pair<bool, bool> suma = saldoPosible(v, i + 1, final - v[i], l);

        flag.first = suma.first || suma.second;
        flag.second = resta.first || resta.second;
    }
    memo[i][final+l] = flag;
    return flag;
    }


bool saldoPosible(vector<int> v,int i, int final) {
    bool flag = false;
    bool suma, resta = false;
    int n = v.size();
    if (final == 0 && i == n) {
        flag = true;
    } else if (final != 0 && i == n) {
        flag = false;
    } else {
        suma = saldoPosible(v, i + 1, final + v[i]);
        resta = saldoPosible(v, i + 1, final - v[i]);
    }
    return flag;
}

int main() {
    int cases;
    cin >> cases;
    for (int k = 0; k < cases; k++) {
        int n, final;
        cin >> n >> final;
        final = final /100;
        vector<int> v;
        for (int j = 0; j < n; j++) {
            int a;
            cin >> a;
            v.push_back(a/100);
        }
        int rango = 0;
        for (int i =0; i<n; i++){
            rango += v[i];
        }
        int max = 2* rango+1;
        int limite = 1000;
        int max_rango = 0;
        if(max < limite){
            max_rango = max;
        } else{
            max_rango = limite;
        }
        vector<vector<pair<int, int>>> memo(n, vector<pair<int, int>>(max_rango, make_pair(-1,-1)));
        // pair<int,int> res = saldoPosible(0, final,max_rango/2);

        for (int i=0;i<n;i++){
            memo.clear();
            swap(v[0],v[i]);
            pair<int,int> res = saldoPosible(v,i,final,rango/2);
            if(res.first == 1 && res.second ==1){
                cout <<'?';
            }else if(res.first == 1){
                cout <<'+';
            }else if(res.second == 1)
                cout <<'-';
        }
        cout << endl;
    }

    return 0;
}
*/