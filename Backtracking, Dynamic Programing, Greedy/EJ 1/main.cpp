#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <limits>


#pragma GCC optimize("Ofast,unroll-loops")
#pragma GCC target("avx2,tune=native")


using namespace std;

constexpr int UNDEFINED = -1;
constexpr char HASH = '#';
const vector<pair<int, int> > direc = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };



struct Pieza {
    char tipo;
    int rotacion;

    Pieza() = default;
};


bool parse_diagonal(const std::vector < std::vector <Pieza> > & T)
{
    bool res = false;
    const int N = T.size();
    if (N < 1) return false;
    for (int i = 0; i < N && !res; i++) {
        if (T[i][i].tipo != HASH) res = true;
    }

    return res;
}

bool parse_rows(const std::vector< std::vector <Pieza> > & T)
{
    bool res = false;
    const int M = T[0].size();
    if (M < 1) return false;
    for (int j = 0; j < M && !res; j++) {
        for (const auto& fila : T) {
            auto it = fila.begin() + j;
            if (fila[j].tipo != HASH) res = true;
        }
    }

    return res;
}

bool parse_columns(const std::vector < std::vector <Pieza> > & T)
{
    bool res = false;
    const int N = T.size();
    if (N < 1) return false;
    if (std::any_of(std::begin(T), std::end(T),
                    [](auto
                       const& fila) {
                        return std::all_of(std::begin(fila), std::end(fila),
                                           [](Pieza p) {
                                               return p.tipo != HASH;
                                           });
                    })) {
        res = true;
    }

    return res;
}

static inline bool piezasCompatibles(const char & t1, const int r1, const char & t2, const int r2, pair<int, int> d)
{
    if (d == make_pair(0, 1)) {
        if (((t1 == '+' && r1 == 1) || (t1 == 'L' && r1 == 1) || (t1 == 'L' && r1 == 2) || (t1 == 'I' && r1 == 2))
            && ((t2 == '+' && r2 == 1) || (t2 == 'L' && r2 == 3) || (t2 == 'L' && r2 == 4) || (t2 == 'I' && r2 == 2))) {
            return true;
        }
    }

    if (d == make_pair(0, -1)) {
        if (((t1 == '+' && r1 == 1) || (t1 == 'L' && r1 == 3) || (t1 == 'L' && r1 == 4) || (t1 == 'I' && r1 == 2))
            && ((t2 == '+' && r2 == 1) || (t2 == 'L' && r2 == 2) || (t2 == 'L' && r2 == 1) || (t2 == 'I' && r2 == 2))) {
            return true;
        }
    }

    if (d == make_pair(-1, 0)) {
        if (((t1 == '+' && r1 == 1) || (t1 == 'L' && r1 == 1) || (t1 == 'L' && r1 == 4) || (t1 == 'I' && r1 == 1))
            && ((t2 == '+' && r2 == 1) || (t2 == 'L' && r2 == 3) || (t2 == 'L' && r2 == 2) || (t2 == 'I' && r2 == 1))) {
            return true;
        }
    }

    if (d == make_pair(1, 0)) {
        if (((t1 == '+' && r1 == 1) || (t1 == 'L' && r1 == 2) || (t1 == 'L' && r1 == 3) || (t1 == 'I' && r1 == 1))
            && ((t2 == '+' && r2 == 1) || (t2 == 'L' && r2 == 1) || (t2 == 'L' && r2 == 4) || (t2 == 'I' && r2 == 1))) {
            return true;
        }
    }

    return false;
}



static inline bool senderoValido(const int i, const int j, const int n, const int m)
{
    return (i >= 0 && i < n && j >= 0 && j < m);
}


static inline int rotacion_pieza(const char & c)
{
    int rotacion = UNDEFINED;

    switch (c) {
        // la rotacion tiene uno mas de lo que deberia por como esta implementado senderoPosible();
        case 'I':
            rotacion = 3;
            break;

        case '+':
            rotacion = 2;
            break;

        case 'L':
            rotacion = 5;
            break;

        case '#':
            rotacion = 1;

        default:
            break;
    }

    return rotacion;
}

bool senderoPosible(const int i, const int j, vector<vector<bool> > & visited, const vector<vector<Pieza> > & T,
                    const int n, const int m, int & max_long, int & min_long, int long_act, int ult_rot)
{
    if (i == n - 1 && j == m - 1) {
        max_long = max(max_long, long_act);
        min_long = min(min_long, long_act);

        return true;
    }

    if (T[n - 1][m - 1].tipo == '#' || T[i][j].tipo == '#') {
        return false;
    }

    if (visited[i][j] && (i > 0 && j > 0)) {
        return true;
    }

    Pieza currPiece = T[i][j];

    bool foundPath = false;
    int cantidad_posible_rotaciones_inicial = rotacion_pieza(currPiece.tipo);

    for (int r = 1; r < cantidad_posible_rotaciones_inicial; r++) {
        if (i == 0 && j == 0 && visited[i][j] == false) {
            currPiece.rotacion = r;
            visited[i][j] = true;
            foundPath |= senderoPosible(i, j, visited, T, n, m, max_long, min_long, long_act, r);
        }
    }
    visited[i][j] = true;

    for (const auto & d : direc) {
        int nextI = i + d.first;
        int nextJ = j + d.second;
        if (senderoValido(nextI, nextJ, n, m) && !visited[nextI][nextJ]) {
            Pieza nextPiece = T[nextI][nextJ];

            int cantidad_posible_rotaciones = rotacion_pieza(nextPiece.tipo);

            for (int r = 1; r < cantidad_posible_rotaciones; r++) {
                if (piezasCompatibles(currPiece.tipo, ult_rot, nextPiece.tipo, r, d)) {
                    nextPiece.rotacion = r;
                    foundPath |= senderoPosible(nextI, nextJ, visited, T, n, m, max_long, min_long, long_act + 1, r);
                }
            }
        }
    }


    visited[i][j] = false;

    return foundPath;
}



int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int casos;
    cin >> casos;
    vector <vector<Pieza> > T = vector <vector<Pieza> > ();
    vector <vector<bool> > visited = vector <vector<bool> > ();

    for (int i = 0; i < casos; ++i) {
        int n, m;
        cin >> n >> m;
        T.resize(n, vector<Pieza> (m));
        visited.resize(n, vector<bool> (m, false));

        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < m; ++k) {
                char tipo;
                cin >> tipo;
                T[j][k] = { tipo, 1 };
            }
        }

        int max_long = std::numeric_limits<int>::min();
        int min_long = std::numeric_limits<int>::max();

        int long_act = 0;

        bool pathFound = false;

        if (parse_columns(T) && parse_diagonal(T) && parse_rows(T)) {
            pathFound = senderoPosible(0, 0, visited, T, n, m, max_long, min_long, long_act, 1);
        }
        // bool pathFound = senderoPosible(0, 0, visited, T, n, m, max_long, min_long, long_act, 1);

        if (!pathFound) {
            cout << "IMPOSIBLE" << endl;
        }
        else {
            cout << "POSIBLE" << " " << min_long << " " << max_long << endl;
        }

        T.clear();
        visited.clear();
    }

    return 0;
}
