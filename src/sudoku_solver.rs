pub fn solve_sudoku(board: &mut Vec<Vec<char>>) {
    fn check_solved(board: &mut Vec<Vec<char>>) -> bool {
        for row in board {
            for val in row {
                if val == &'.' {
                    return false
                }
            }
        }
        true
    }

    fn used_numbers(board: &mut Vec<Vec<char>>, y: usize, x: usize) -> Vec<char> {
        let mut used: Vec<char> = Vec::new();
        for srv in &board[y] {
            if srv != &'.' {
                used.push(*srv)
            }
        }
        for scv in 0..9 {
            if board[scv][x] != '.' && !used.contains(&board[scv][x]) {
                used.push(board[scv][x])
            }
        }
        let box_r: usize = ((y / 3) as i32) as usize;
        let box_c: usize = ((x / 3) as i32) as usize;
        for i in (box_r*3)..(box_r*3+3) {
            for j in (box_c*3)..(box_c*3+3) {
                if board[i][j] != '.' && !used.contains(&board[i][j]) {
                    used.push(board[i][j])
                }
            }
        }

        used
    }

    fn unused_numbers(used_numbers: Vec<char>) -> Vec<char> {
        let all: Vec<char> = vec!['1','2','3','4','5','6','7','8','9'];
        let mut unused: Vec<char> = Vec::new();
        for c in all {
            if !used_numbers.contains(&c) {
                unused.push(c);
            }
        }
        unused
    }

    fn solve_number(board: &mut Vec<Vec<char>>, y: usize, x: usize, brute: bool) -> char  {
        let used_num = used_numbers(board, y, x);
        let unused_nums = unused_numbers(used_num);
        let box_r: usize = ((y / 3) as i32) as usize;
        let box_c: usize = ((x / 3) as i32) as usize;
        for number in unused_nums {
            let mut can_solve = true;
            for i in (box_r*3)..(box_r*3+3) {
                for j in (box_c*3)..(box_c*3+3) {
                    if board[i][j] == '.' && (i != y || j != x) {
                        let used_z = used_numbers(board, i as usize, j as usize);
                        if !used_z.contains(&number) {
                            can_solve = false;
                        }
                    }
                }
            }
            if can_solve {
                return number
            }
            can_solve = true;
            for srv in 0..9 {
                if board[y][srv] == '.' && srv != x {
                    let used_z = used_numbers(board, y, srv);
                    if !used_z.contains(&number) {
                        can_solve = false;
                    }
                }
            }
            if can_solve {
                return number
            }
            can_solve = true;
            for scv in 0..9 {
                if board[scv][x] == '.' && scv != y {
                    let used_z = used_numbers(board, scv, x);
                    if !used_z.contains(&number) {
                        can_solve = false;
                    }
                }
            }
            if can_solve {
                return number
            }
        }
        if brute {
            let unused = unused_numbers(used_numbers(board, y, x));
            for number in unused {
                let board_cpy = &mut board.to_vec();
                board_cpy[y][x] = number;
                for _iter in 0..5 {
                    for i in 0..9 {
                        for j in 0..9 {
                            if board_cpy[i][j] == '.' {
                                board_cpy[i][j] = solve_number(board_cpy, i, j, false);
                            }
                        }
                    }
                }
                if check_solved(board_cpy) {
                    return number;
                }
            }
        }
        '.'
    }

    while !check_solved(board) {
        for y in 0..9 {
            for x in 0..9 {
                if board[y][x] == '.' {
                    board[y][x] = solve_number(board, y, x, true);
                }
            }
        }
    }
}

pub fn print_board(board: &mut Vec<Vec<char>>) {
    println!();
    for i in 0..9 {
        if i != 0 && i % 3 == 0 {
            println!();
        }
        for j in 0..9 {
            if j % 3 == 0 {
                print!("\t");
            }
            print!("{} ", board[i][j]);
        }
        println!();
    }
    println!();
}
