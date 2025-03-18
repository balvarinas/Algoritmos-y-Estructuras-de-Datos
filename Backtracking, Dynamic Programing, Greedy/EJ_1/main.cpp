#include <iostream>
#include <cstdint>
#include <vector>
#include <utility>
#include <array>
#include <limits>

#pragma GCC optimize("O3,unroll-loops")
// #pragma GCC target("avx2,bmi,bmi2,popcnt,lzcnt,sse")

#define INVALID_CHAR            '#'
#define REACHABLE_POSITIONS     4


enum Positions {DOWN = -1, UP = 1, LEFT = 0, RIGHT = 0};

std::pair<int, int> res = {0, 0};

static inline bool valid_tile(const char c)
{
    return c != INVALID_CHAR;
}

static inline bool in_range(const int i, const int j, const int M, const int N)
{
    return i >= 0 and i < M and j >= 0 and j < N;
}

template <typename T>
uint32_t bt(const std::vector<T> & board, std::vector<std::vector<bool> > & visited, int32_t i, int32_t j, const int32_t M, const int32_t N, int & minimum_moves_required, int current_move)
{
    if ( i == 0 and j == N - 1)  {
        res.first = minimum_moves_required = std::min(minimum_moves_required, current_move);
        return 1;
    }

    visited[i][j] = true;
    int32_t acc = 0;

    constexpr std::array<int, REACHABLE_POSITIONS> moves_horizontal = {DOWN, RIGHT, UP, LEFT};
    constexpr std::array<int, REACHABLE_POSITIONS> moves_vertical   = {LEFT, UP, RIGHT, DOWN};

    int k = 0;
    while (k < REACHABLE_POSITIONS) {
        int32_t next_horizontal = i + moves_horizontal[k];
        int32_t next_vertical   = j + moves_vertical[k];

        if (in_range(next_horizontal, next_vertical, M, N) and not visited[next_horizontal][next_vertical]
            and valid_tile(board[next_horizontal][next_vertical])) {
            res.second = acc += bt(board, visited, next_horizontal, next_vertical, M, N, minimum_moves_required, current_move + 1);
        }
        k++;
    }

    visited[i][j] = false;

    return acc;
}


template <typename T>
int32_t bt_helper(const std::vector<T> & board, const int32_t M, const uint32_t N)
{
    std::vector<std::vector<bool> > visited =
            std::vector<std::vector<bool > > (M, std::vector<bool> (N, 0));

    int32_t INFTY = std::numeric_limits<int32_t>::max() - 1;

    return bt(board, visited, M - 1, 0, M, N, INFTY, 0);
}


int main(void)
{
    int c;
    std::cin >> c;

    std::vector<std::string> board = std::vector<std::string> ();
    while (c--) {
        int32_t N, M; std::cin >> N >> M;
        board.resize(N);

        for (auto & p : board) std::cin >> p;

        bt_helper(board, N, M);
        board.clear();
        if (res.first < 1 or res.second < 1) std::cout << "IMPOSIBLE" << std::endl;
        else std::cout << "POSIBLE " << res.first << " " << res.second << std::endl;

    }

    return 0;
}
