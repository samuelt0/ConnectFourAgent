    #include "Game.h"
    #include <iostream>
    #include <vector>
    #include <limits.h>
    #include <array>
    #include "Heuristic.h"

    unsigned int NUM_COL = 7;
    unsigned int NUM_ROW = 6;
    unsigned int PLAYER = 1;
    unsigned int COMPUTER = 2;
    unsigned int MAX_DEPTH = 9; //9 too slow?
    bool gameOver = false;
    unsigned int turns = 0;
    unsigned int currentPlayer = PLAYER;
    std::vector<std::vector<int>> board(NUM_ROW, std::vector<int>(NUM_COL));

    void printBoard(std::vector<std::vector<int>>& b) {
        for (unsigned int i = 0; i < NUM_COL; i++) {
            std::cout << " " << i;
        }
        std::cout << std::endl << "---------------" << std::endl;
        for (unsigned int r = 0; r < NUM_ROW; r++) {
            for (unsigned int c = 0; c < NUM_COL; c++) {
                std::cout << "|";
                switch (b[NUM_ROW - r - 1][c]) {
                    case 0: std::cout << " "; break;
                    case 1: std::cout << "O"; break;
                    case 2: std::cout << "X"; break;
                }
                if (c + 1 == NUM_COL) { std::cout << "|"; }
            }
            std::cout << std::endl;
        }
        std::cout << "---------------" << std::endl;
        std::cout << std::endl;
    }

    void makeMove(std::vector<std::vector<int>>& b, int c, unsigned int p) {
        for (unsigned int r = 0; r < NUM_ROW; r++) {
            if (b[r][c] == 0) {
                b[r][c] = p;
                break;
            }
        }
    }

    int userMove() {
        int move = -1;
        while (true) {
            std::cout << "Enter a column: ";
            std::cin >> move;
            if (!std::cin) {
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                errorMessage(1);
            } else if (!(static_cast<unsigned int>(move) < NUM_COL && move >= 0)) {
                errorMessage(2);
            } else if (board[NUM_ROW - 1][move] != 0) {
                errorMessage(3);
            } else {
                break;
            }
            std::cout << std::endl << std::endl;
        }
        return move;
    }

    int aiMove() {
        std::cout << "AI is thinking about a move..." << std::endl;
        return miniMax(board, MAX_DEPTH, INT_MIN, INT_MAX, COMPUTER)[1];
    }

    std::array<int, 2> miniMax(std::vector<std::vector<int>>& b, unsigned int d, int alf, int bet, unsigned int p) {
        if (d == 0 || winningMove(b, COMPUTER) || winningMove(b, PLAYER) || isBoardFull(b)) {
            return {tabScore(b, COMPUTER), -1};
        }

        if (p == COMPUTER) {
            std::array<int, 2> bestMove = {INT_MIN, -1};
            for (unsigned int c = 0; c < NUM_COL; c++) {
                if (b[NUM_ROW - 1][c] == 0) {
                    auto newBoard = copyBoard(b);
                    makeMove(newBoard, c, p);
                    int score = miniMax(newBoard, d - 1, alf, bet, PLAYER)[0];
                    if (score > bestMove[0]) {
                        bestMove = {score, static_cast<int>(c)};
                    }
                    alf = std::max(alf, bestMove[0]);
                    if (alf >= bet) {
                        break;
                    }
                }
            }
            return bestMove;
        } else {
            std::array<int, 2> bestMove = {INT_MAX, -1};
            for (unsigned int c = 0; c < NUM_COL; c++) {
                if (b[NUM_ROW - 1][c] == 0) {
                    auto newBoard = copyBoard(b);
                    makeMove(newBoard, c, p);
                    int score = miniMax(newBoard, d - 1, alf, bet, COMPUTER)[0];
                    if (score < bestMove[0]) {
                        bestMove = {score, static_cast<int>(c)};
                    }
                    bet = std::min(bet, bestMove[0]);
                    if (alf >= bet) {
                        break;
                    }
                }
            }
            return bestMove;
        }
    }

    int tabScore(const std::vector<std::vector<int>>& b, unsigned int p) {
        int score = 0;
        std::vector<unsigned int> rs(NUM_COL);
        std::vector<unsigned int> cs(NUM_ROW);
        std::vector<unsigned int> set(4);

        for (unsigned int r = 0; r < NUM_ROW; r++) {
            for (unsigned int c = 0; c < NUM_COL; c++) {
                rs[c] = b[r][c];
            }
            for (unsigned int c = 0; c < NUM_COL - 3; c++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = rs[c + i];
                }
                score += scoreSet(set, p);
            }
        }

        for (unsigned int c = 0; c < NUM_COL; c++) {
            for (unsigned int r = 0; r < NUM_ROW; r++) {
                cs[r] = b[r][c];
            }
            for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = cs[r + i];
                }
                score += scoreSet(set, p);
            }
        }

        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (unsigned int c = 0; c < NUM_COL; c++) {
                rs[c] = b[r][c];
            }
            for (unsigned int c = 0; c < NUM_COL - 3; c++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = b[r + i][c + i];
                }
                score += scoreSet(set, p);
            }
        }

        for (unsigned int r = 0; r < NUM_ROW - 3; r++) {
            for (unsigned int c = 0; c < NUM_COL - 3; c++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = b[r + 3 - i][c + i];
                }
                score += scoreSet(set, p);
            }
        }

        return score;
    }

    bool winningMove(const std::vector<std::vector<int>>& b, unsigned int p) {
        for (unsigned int r = 0; r < NUM_ROW; ++r) {
            for (unsigned int c = 0; c <= NUM_COL - 4; ++c) {
                if (b[r][c] == p && b[r][c + 1] == p && b[r][c + 2] == p && b[r][c + 3] == p) {
                    return true;
                }
            }
        }

        for (unsigned int c = 0; c < NUM_COL; ++c) {
            for (unsigned int r = 0; r <= NUM_ROW - 4; ++r) {
                if (b[r][c] == p && b[r + 1][c] == p && b[r + 2][c] == p && b[r + 3][c] == p) {
                    return true;
                }
            }
        }

        for (unsigned int r = 0; r <= NUM_ROW - 4; ++r) {
            for (unsigned int c = 0; c <= NUM_COL - 4; ++c) {
                if (b[r][c] == p && b[r + 1][c + 1] == p && b[r + 2][c + 2] == p && b[r + 3][c + 3] == p) {
                    return true;
                }
            }
        }

        for (unsigned int r = 3; r < NUM_ROW; ++r) {
            for (unsigned int c = 0; c <= NUM_COL - 4; ++c) {
                if (b[r][c] == p && b[r - 1][c + 1] == p && b[r - 2][c + 2] == p && b[r - 3][c + 3] == p) {
                    return true;
                }
            }
        }

        return false;
    }

    std::vector<std::vector<int>> copyBoard(const std::vector<std::vector<int>>& b) {
        return b;
    }

    void errorMessage(int t) {
        switch (t) {
            case 1: std::cout << "Invalid input. Please enter a number." << std::endl; break;
            case 2: std::cout << "Invalid column. Please enter a number between 0 and " << NUM_COL - 1 << "." << std::endl; break;
            case 3: std::cout << "This column is full. Please choose another column." << std::endl; break;
        }
    }

    bool isBoardFull(const std::vector<std::vector<int>>& b) {
        for (unsigned int c = 0; c < NUM_COL; ++c) {
            if (b[NUM_ROW - 1][c] == 0) {
                return false;
            }
        }
        return true;
    }

    void playGame() {
        printBoard(board); 
        while (!gameOver) { 
            if (currentPlayer == COMPUTER) { 
                makeMove(board, aiMove(), COMPUTER);
            } else if (currentPlayer == PLAYER) { 
                makeMove(board, userMove(), PLAYER);
            }

            if (winningMove(board, PLAYER) || winningMove(board, COMPUTER)) {
                gameOver = true;
            } else if (isBoardFull(board)) {
                gameOver = true;
            }

            currentPlayer = (currentPlayer == PLAYER) ? COMPUTER : PLAYER;
            turns++; 
            std::cout << std::endl;
            printBoard(board);
        }   

        if (isBoardFull(board)) {
            std::cout << "Draw!" << std::endl;
        } else {
            std::cout << ((currentPlayer == PLAYER) ? "AI Wins!" : "Player Wins!") << std::endl;
        }
    }