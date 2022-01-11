#include "connect4.h"
#include <bits/stdc++.h>
#define INF LLONG_MAX
#define NEG_INF LLONG_MIN
#define None -1
#define ROW 6
#define COL 7
#define EMPTY 0
#define PLAYER_PIECE 1
#define YP_PIECE 2
#define WINDOW_LENGTH 4
#define ll long long
using namespace std;

vector<vector<int> > board;

bool winning_move(vector<vector<int> > &board, int piece);
bool is_valid_location(vector<vector<int> > &board, int col);
bool is_terminal_node(vector<vector<int> > &board);
vector<int> get_valid_locations(vector<vector<int> > &board);
pair<int, ll> minmax(vector<vector<int> > &board, int depth, ll alpha, ll beta, bool maximizingPlayer);
ll score_position(vector<vector<int> > &board, int piece);
ll array_count(vector<int> &array, int obj);
ll evaluate_window(vector<int> &window, int piece);
int random_choice(vector<int> &array);
int get_next_open_row(vector<vector<int> > &board, int col);
vector<vector<int> > copyBoard(vector<vector<int> > &board);
void drop_piece(vector<vector<int> > &board, int row, int col, int piece);

bool winning_move(vector<vector<int> > &board, int piece)
{
    //Check horizontal locations for win
    for (int c = 0; c < COL - 3; c++)
        for (int r = 0; r < ROW; r++)
            if (board[r][c] == piece && board[r][c + 1] == piece && board[r][c + 2] == piece && board[r][c + 3] == piece)
                return true;

    //Check vertical locations for win
    for (int c = 0; c < COL; c++)
        for (int r = 0; r < ROW - 3; r++)
            if (board[r][c] == piece && board[r + 1][c] == piece && board[r + 2][c] == piece && board[r + 3][c] == piece)
                return true;

    //Check positively sloped diaganols
    for (int c = 0; c < COL - 3; c++)
        for (int r = 0; r < ROW - 3; r++)
            if (board[r][c] == piece && board[r + 1][c + 1] == piece && board[r + 2][c + 2] == piece && board[r + 3][c + 3] == piece)
                return true;

    //Check negatively sloped diaganols
    for (int c = 0; c < COL - 3; c++)
        for (int r = 3; r < ROW; r++)
            if (board[r][c] == piece && board[r - 1][c + 1] == piece && board[r - 2][c + 2] == piece && board[r - 3][c + 3] == piece)
                return true;
    return false;
}

bool is_valid_location(vector<vector<int> > &board, int col)
{
    return board[ROW - 1][col] == 0;
}
bool is_terminal_node(vector<vector<int> > &board)
{
    return (winning_move(board, PLAYER_PIECE) | winning_move(board, YP_PIECE) | int(get_valid_locations(board).size()) == 0);
}
vector<int> get_valid_locations(vector<vector<int> > &board)
{
    vector<int> res;
    for (int i = 0; i < COL; i++)
        if (is_valid_location(board, i))
            res.push_back(i);
    return res;
}
ll array_count(vector<int> &array, int obj)
{
    ll res = 0;
    for (auto it : array)
        if (it == obj)
            res++;
    return res;
}

ll evaluate_window(vector<int> &window, int piece)
{
    ll score = 0;
    int opp_piece = YP_PIECE;
    if (piece == YP_PIECE)
        opp_piece = PLAYER_PIECE;

    if (array_count(window, piece) == 4)
        score += 100;
    else if (array_count(window, piece) == 3 && array_count(window, EMPTY) == 1)
        score += 75;
    else if (array_count(window, piece) == 2 && array_count(window, EMPTY) == 2)
        score += 30;

    if (array_count(window, opp_piece) == 3 && array_count(window, EMPTY) == 1)
        score -= 10;

    return score;
}
void drop_piece(vector<vector<int> > &board, int row, int col, int piece)
{
    board[row][col] = piece;
}

ll score_position(vector<vector<int> > &board, int piece)
{
    ll score = 0;

    //Score center column
    vector<int> center_array;
    for (int i = 0; i < ROW; i++)
        center_array.push_back(board[i][COL / 2]);
    ll center_count = array_count(center_array, piece);
    score += center_count * 3;

    //Score Horizontal
    for (int r = 0; r < ROW; r++)
    {
        vector<int> row_array;
        for (int i = 0; i < COL; i++)
            row_array.push_back(board[r][i]);
        for (int c; c < COL - 3; c++)
        {
            vector<int> window;
            window.assign(row_array.cbegin() + c, row_array.cbegin() + c + WINDOW_LENGTH);
            score += evaluate_window(window, piece);
        }
    }

    //Score Vertical
    for (int c = 0; c < COL; c++)
    {
        vector<int> col_array;
        for (int i = 0; i < ROW; i++)
            col_array.push_back(board[i][c]);
        for (int r = 0; r < ROW - 3; r++)
        {
            vector<int> window;
            window.assign(col_array.cbegin() + r, col_array.cbegin() + r + WINDOW_LENGTH);
            score += evaluate_window(window, piece);
        }
    }

    //Score posiive sloped diagonal
    for (int r = 0; r < ROW - 3; r++)
    {
        for (int c = 0; c < COL - 3; c++)
        {
            vector<int> window;
            for (int i = 0; i < WINDOW_LENGTH; i++)
                window.push_back(board[r + i][c + i]);
            score += evaluate_window(window, piece);
        }
    }

    for (int r = 0; r < ROW - 3; r++)
    {
        for (int c = 0; c < COL - 3; c++)
        {
            vector<int> window;
            for (int i = 0; i < WINDOW_LENGTH; i++)
                window.push_back(board[r + 3 - i][c + i]);
            score += evaluate_window(window, piece);
        }
    }

    return score;
}

int random_choice(vector<int> &array)
{
    int N = array.size();
    return array[rand() % N];
}
int get_next_open_row(vector<vector<int> > &board, int col)
{
    for (int r = 0; r < ROW; r++)
        if (board[r][col] == 0)
            return r;
}
vector<vector<int> > copyBoard(vector<vector<int> > &board)
{
    vector<vector<int> > newBoard(ROW, vector<int>(COL));
    for (int r = 0; r < ROW; r++)
    {
        for (int c = 0; c < COL; c++)
        {
            newBoard[r][c] = board[r][c];
        }
    }
    return newBoard;
}

pair<int, ll> minmax(vector<vector<int> > &board, int depth, ll alpha, ll beta, bool maximizingPlayer)
{
    vector<int> valid_locations = get_valid_locations(board);
    bool is_terminal = is_terminal_node(board);
    if (depth == 0 | is_terminal)
    {
        if (is_terminal)
        {
            if (winning_move(board, PLAYER_PIECE))
                return {None, INF};
            else if (winning_move(board, YP_PIECE))
                return {None, NEG_INF};
            else
                return {None, 0};
        }
        else
            return {None, score_position(board, PLAYER_PIECE)};
    }

    if (maximizingPlayer)
    {
        ll value = NEG_INF;
        int column = random_choice(valid_locations);
        for (auto col : valid_locations)
        {
            int row = get_next_open_row(board, col);
            vector<vector<int> > b_copy = copyBoard(board);
            drop_piece(b_copy, row, col, PLAYER_PIECE);
            ll new_score = minmax(b_copy, depth - 1, alpha, beta, false).second;
            if (new_score > value)
            {
                value = new_score;
                column = col;
            }
            alpha = max(alpha, value);
            if (alpha >= beta)
                break;
        }
        return {column, value};
    }
    else
    {
        ll value = INF;
        int column = random_choice(valid_locations);
        for (auto col : valid_locations)
        {
            int row = get_next_open_row(board, col);
            vector<vector<int> > b_copy = copyBoard(board);
            drop_piece(b_copy, row, col, YP_PIECE);
            ll new_score = minmax(b_copy, depth - 1, alpha, beta, true).second;
            if (new_score < value)
            {
                value = new_score;
                column = col;
            }
            beta = min(beta, value);
            if (alpha >= beta)
                break;
        }
        return {column, value};
    }
}

// The columns are numbered from 0 to 6
int decide(int yp_move)
{
    if (yp_move == -1)
    {
        // A new game starts
        // TODO: Please remember to reset everything here (if needed)
        srand(time(NULL));
        board.clear();
        board.resize(ROW);
        for (int i = 0; i < ROW; i++)
            board[i].resize(COL, 0);
        int col = 3;
        int row = get_next_open_row(board, col);
        drop_piece(board, row, col, PLAYER_PIECE);
        return col;
    }
    else
    {
        // YP placed his piece at column `yp_move`
        int YP_row = get_next_open_row(board, yp_move);
        drop_piece(board, YP_row, yp_move, YP_PIECE);
        pair<int, ll> res = minmax(board, 6, NEG_INF, INF, true);
        int col = res.first;
        ll minmax_score = res.second;
        if (col != None && is_valid_location(board, col))
        {
            int row = get_next_open_row(board, col);
            drop_piece(board, row, col, PLAYER_PIECE);
        }
        return col;
    }
}
