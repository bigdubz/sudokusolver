#include <stdio.h>

int notIn(const int size, const char arr[], const char num) {
    for (int i = 0; i < size; i++) {
        if (arr[i] == num) {
            return 0;
        }
    }
    return 1;
}

int checkSolved(char board[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == '.') return 0;
        }
    }
    return 1;
}

void fillMissingNums(const int size, const char used[size], char unused[]) {
    const char allNums[9] = "123456789";
    int currSize = 0;
    for (int i = 0; i < 9; i++) {
        int found = 0;
        for (int j = 0; j < size; j++) {
            if (used[j] == allNums[i]) {
                found = 1;
                break;
            }
        }
        if (!found) {
            unused[currSize++] = allNums[i];
        }
    }
}

int fillUsedNums(char board[9][9], const int i, const int j, char arrToFill[]) {
    int size = 0;
    for (int c = 0; c < 9; c++) {
        if (board[i][c] != '.' && notIn(size, arrToFill, board[i][c])) {
            arrToFill[size++] = board[i][c];
        }
    }

    // check column
    for (int r = 0; r < 9; r++) {
        if (board[r][j] != '.' && notIn(size, arrToFill, board[r][j])) {
            arrToFill[size++] = board[r][j];
        }
    }

    // check box
    const int boxRow = i/3;
    const int boxCol = j/3;
    for (int y = boxRow * 3; y < boxRow*3 + 3; y++) {
        for (int x = boxCol * 3; x < boxCol*3 + 3; x++) {
            if (board[y][x] != '.' && notIn(size, arrToFill, board[y][x])) {
                arrToFill[size++] = board[y][x];
            }
        }
    }

    return size;
}


char solveNumber(char board[9][9], const int i, const int j, const int use_recursion) {
    char used_nums[9];
    const int size = fillUsedNums(board, i, j, used_nums);

    if (size == 9) return board[i][j];

    // stage 1: check 'last remaining cell'
    char unused_nums[9-size];
    fillMissingNums(size, used_nums, unused_nums);
    const int boxRow = i/3;
    const int boxCol = j/3;
    for (int n = 0; n < 9 - size; n++) {
        const char number = unused_nums[n];

        // in the current box
        int canSolve = 1;
        for (int y = boxRow * 3; y < boxRow*3 + 3; y++) {
            for (int x = boxCol * 3; x < boxCol*3 + 3; x++) {
                if (board[y][x] == '.' && (y != i || x != j)) {
                    char curr_sq_used_nums[9];
                    const int s = fillUsedNums(board, y, x, curr_sq_used_nums);
                    if (notIn(s, curr_sq_used_nums, number)) {
                        canSolve = 0;
                    }
                }
            }
        }

        if (canSolve) {
            return number;
        }

        // in the current row:
        canSolve = 1;
        for (int x = 0; x < 9; x++) {
            if (board[i][x] == '.' && x != j) {
                char curr_sq_used_nums[9];
                const int s = fillUsedNums(board, i, x, curr_sq_used_nums);
                if (notIn(s, curr_sq_used_nums, number)) {
                    canSolve = 0;
                }
            }
        }

        if (canSolve) {
            return number;
        }

        // in the current column:
        canSolve = 1;
        for (int y = 0; y < 9; y++) {
            if (board[y][j] == '.' && y != i) {
                char curr_sq_used_nums[9];
                const int s = fillUsedNums(board, y, j, curr_sq_used_nums);
                if (notIn(s, curr_sq_used_nums, number)) {
                    canSolve = 0;
                }
            }
        }

        if (canSolve) {
            return number;
        }
    }

    // stage 2: brute force this bitch
    if (use_recursion) {
        char brute_force_board[9][9];

        for (int y = 0; y < 9; y++) {
            for (int x = 0; x < 9; x++) {
                brute_force_board[y][x] = board[y][x];
            }
        }

        for (int y = 0; y < 9 - size; y++) {
            const char number_brute_forcing = unused_nums[y];
            brute_force_board[i][j] = number_brute_forcing;
            for (int iter = 0; iter < 100; iter++) {
                for (int i1 = 0; i1 < 9; i1++) {
                    for (int j1 = 0; j1 < 9; j1++) {
                        if (brute_force_board[i1][j1] == '.') {
                            brute_force_board[i1][j1] = solveNumber(brute_force_board, i1, j1, 0);
                        }
                    }
                }
            }
            int solved = 1;
            for (int i1 = 0; i1 < 9; i1++) {
                for (int j1 = 0; j1 < 9; j1++) {
                    if (brute_force_board[i1][j1] == '.') solved = 0;
                }
            }
            if (solved) {
                return number_brute_forcing;
            }
        }
    }
    return '.';
}

void solveSudoku(char board[9][9], const int boardSize, const int* boardColSize) {
    int on_recursion = 0;
    int d_count_compare = 0;
    for (int iter = 0; iter < 100; iter++) {
        int d_count = 0;
        int skip_iter = 0;
        for (int i = 0; i < boardSize; i++) {
            if (skip_iter) break;
            for (int j = 0; j < *boardColSize; j++) {
                if (skip_iter) break;
                if (board[i][j] == '.') {
                    const char c = solveNumber(board, i, j, on_recursion);
                    board[i][j] = c;
                    if (c == '.')
                        d_count++;
                    else {
                        skip_iter = 1;
                    }
                }
            }
        }
        if (skip_iter) continue;
        on_recursion = 0;
        if (d_count_compare == d_count && d_count != 0) {
            on_recursion = 1;
        }
        d_count_compare = d_count;
        if (d_count == 0) {
            return;
        }
    }
}



int main(void) {
    const int s = 9;
    char a[9][9] = {
            {'.','.','9',    '7','4','8',    '.','.','.'},
            {'7','.','.',    '.','.','.',    '.','.','.'},
            {'.','2','.',    '1','.','9',    '.','.','.'},

            {'.','.','7',    '.','.','.',    '2','4','.'},
            {'.','6','4',    '.','1','.',    '5','9','.'},
            {'.','9','8',    '.','.','.',    '3','.','.'},

            {'.','.','.',    '8','.','3',    '.','2','.'},
            {'.','.','.',    '.','.','.',    '.','.','6'},
            {'.','.','.',    '2','7','5',    '9','.','.'}
    };
    solveSudoku(a, s, &s);
    for (int i = 0; i < s; i++) {
        if (!(i % 3)) {
            printf("\n");
        }
        for (int j = 0; j < s; j++) {
            printf("%c ", a[i][j]);
            if (!((j+1) % 3)) {
                printf("\t");
            }
        }
        printf("\n");
    }
    return 0;
}
