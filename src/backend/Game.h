#ifndef GAME_H
#define GAME_H

#include <vector>
#include <array>

extern unsigned int NUM_COL;
extern unsigned int NUM_ROW;
extern unsigned int PLAYER;
extern unsigned int COMPUTER;
extern unsigned int MAX_DEPTH;

void printBoard(std::vector<std::vector<int>>& b);
void makeMove(std::vector<std::vector<int>>& b, int c, unsigned int p);
int userMove();
int aiMove();
std::array<int, 2> miniMax(std::vector<std::vector<int>>& b, unsigned int d, int alf, int bet, unsigned int p);
int tabScore(const std::vector<std::vector<int>>& b, unsigned int p);
bool winningMove(const std::vector<std::vector<int>>& b, unsigned int p);
std::vector<std::vector<int>> copyBoard(const std::vector<std::vector<int>>& b);
void errorMessage(int t);
void initBoard();
bool isBoardFull(const std::vector<std::vector<int>>& b);
void playGame();
int evaluateLines(const std::vector<std::vector<int>>& b, unsigned int p);
int evaluateLine(const std::vector<std::vector<int>>& b, int row, int col, int deltaRow, int deltaCol, unsigned int p);

#endif
