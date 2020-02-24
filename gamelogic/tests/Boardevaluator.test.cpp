#include "gtest/gtest.h"
#include "../logic/Board.h"
#include "../logic/ranking.h"
#include "../logic/Boardevaluator.h"

const int valueOfEmpyCell = 1;
const int valueOfEmpyOwnCell = 3 * 3;
const int valueOfEmpyOppCell = valueOfEmpyOwnCell + 1;
const int valueOfEmptyLine = 5 * valueOfEmpyCell;

TEST(Boardevaluator_evaluate, corners_should_have_same_value) {
  const int row = 8;
  Board board(row);
  char brd[] = {
    0, 0, 0, 0, 0, 0, 0, 0, // 0 7
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0 }; // 56 63
  board.setBoard(brd);
  const int boardSize = row * row;
  int* boardValues = new int[boardSize];
  Boardevaluator boardEval;
  int eval = boardEval.evaluateBoard(brd, boardValues, row, 1);
  // 3 fives in 3 diff directions should be possible in each corner
  const int first_corner = boardValues[0];
  EXPECT_EQ(boardValues[7], first_corner);
  EXPECT_EQ(boardValues[56], first_corner);
  EXPECT_EQ(boardValues[63], first_corner);
  delete[] boardValues;
}

TEST(Boardevaluator_evaluate, symmetry_next_to_corners) {
  const int row = 8;
  Board board(row);
  char brd[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, // 9 14
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, // 47 54
    0, 0, 0, 0, 0, 0, 0, 0 }; // 56 63
  board.setBoard(brd);
  const int boardSize = row * row;
  int* boardValues = new int[boardSize];
  Boardevaluator boardEval;
  int eval = boardEval.evaluateBoard(brd, boardValues, row, 1);
  // 3 diff directions should have 2 compleate sequenses 
  const int value_one_step_from_first_corner = boardValues[9];
  EXPECT_EQ(boardValues[14], value_one_step_from_first_corner);
  EXPECT_EQ(boardValues[49], value_one_step_from_first_corner);
  EXPECT_EQ(boardValues[54], value_one_step_from_first_corner);
  delete[] boardValues;
}

TEST(Boardevaluator_calcCellLineSignificance, emptySpaceLine) {
  Boardevaluator boardEval;
  int line[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, };
  int value = boardEval.calcCellLineSignificance(line);
  EXPECT_EQ(value, 5*5);
}

TEST(Boardevaluator_calcCellLineSignificance, 2_empyt_lines_possible) {
  Boardevaluator boardEval;
  int line1[] = { 3, 3, 3, 0, 0, 0, 0, 0, 0, };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line1), 2 * 5);
  int line2[] = { 0, 0, 0, 0, 0, 0, 3, 3, 3, };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line2), 2 * 5);
}

TEST(Boardevaluator_calcCellLineSignificance, sss) {
  Boardevaluator boardEval;
  int line1[] = { 1, 1, 1, 2, 0, 0, 0, 0, 0, };
  int valueOfLastSubLine4 = valueOfEmpyOppCell + 4 * valueOfEmpyCell;
  int valueOfLastSubLine5 = valueOfEmptyLine;
  EXPECT_EQ(boardEval.calcCellLineSignificance(line1), valueOfLastSubLine4 + valueOfLastSubLine5); // 19
  //int line2[] = { 0, 0, 0, 0, 0, 0, 3, 3, 3, };
  //EXPECT_EQ(boardEval.calcCellLineSignificance(line2), 2 * 5);
}

TEST(Boardevaluator_calcCellLineSignificance, _5_empty_last_spots) {
  Boardevaluator boardEval;
  int line[] = { 3, 3, 3, 3, 0, 0, 0, 0, 0 };
  int value = boardEval.calcCellLineSignificance(line);
  EXPECT_EQ(value, 5);
}

TEST(Boardevaluator_calcCellLineSignificance, _5_empty__first_spots) {
  Boardevaluator boardEval;
  int line[] = { 0, 0, 0, 0, 0, 3, 3, 3, 3 };
  int value = boardEval.calcCellLineSignificance(line);
  EXPECT_EQ(value, 5);
}

TEST(Boardevaluator_calcCellLineSignificance, own_4open) {
  Boardevaluator boardEval;
  int line1[] = { 0, 1, 1, 1, 0, 0, 2, 0, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line1), open4row);
  int line2[] = { 0, 0, 1, 1, 0, 1, 0, 2, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line2), open4row);
  int line3[] = { 2, 0, 0, 1, 0, 1, 1, 0, 2 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line3), open4row);
  int line4[] = { 2, 2, 2, 0, 0, 1, 1, 1, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line3), open4row);
}

TEST(Boardevaluator_calcCellLineSignificance, opponent_4open) {
  Boardevaluator boardEval;
  int line1[] = { 0, 2, 2, 2, 0, 0, 0, 0, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line1), blockOpen4row);
  int line2[] = { 0, 0, 2, 2, 0, 2, 0, 0, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line2), blockOpen4row);
  int line3[] = { 0, 0, 0, 2, 0, 2, 2, 0, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line3), blockOpen4row);
  int line4[] = { 0, 0, 0, 0, 0, 2, 2, 2, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line3), blockOpen4row);
}

TEST(Boardevaluator_calcCellLineSignificance, can_find_win) {
  Boardevaluator boardEval;
  int line1[] = { 1, 1, 1, 1, 0, 0, 0, 0, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line1), fiveInRow);
  int line2[] = { 0, 1, 1, 1, 0, 1, 0, 0, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line2), fiveInRow);
  int line3[] = { 0, 0, 1, 1, 0, 1, 1, 0, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line3), fiveInRow);
  int line4[] = { 0, 0, 0, 1, 0, 1, 1, 1, 0 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line4), fiveInRow);
  int line5[] = { 0, 0, 0, 0, 0, 1, 1, 1, 1 };
  EXPECT_EQ(boardEval.calcCellLineSignificance(line5), fiveInRow);
}

//TEST(Boardevaluator, calcCellLineSignificance_10000) {
//  Boardevaluator boardEval;
//  int line[] = { 0, 1, 2, 2, 1, 2, 2, 0, 0 };
//  int value = boardEval.calcCellLineSignificance(line);
//  EXPECT_EQ(value, 6);
//}

//TEST(Boardevaluator, calcCellLineSignificance_10000_00000) {
//  Boardevaluator boardEval;
//  int line[] = { 1, 0, 0, 0, 0, 0, 2, 2, 2 };
//  int value = boardEval.calcCellLineSignificance(line);
//  EXPECT_EQ(value, 6 + 5);
//}

TEST(Boardevaluator, best_cell_should_be_136) {
  const int row = 16;
  Board board(row);
  char brd[] = {
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,1, 0,0,0,0,0,0,0,0, // cell 119

    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, // cell 136
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0, };
  EXPECT_EQ(brd[119], 1);
  int cellValues[16 * 16];
  short cellIdx[16 * 16];
  board.setBoard(brd);
  const int boardSize = row * row;
  Boardevaluator boardEval;
  int eval = boardEval.evaluateBoard(brd, cellValues, row, 2);
  boardEval.sortCellValues(cellValues, cellIdx, boardSize, 10);
  const int bestCellId = 135;
  int value = boardEval.evaluateCell(bestCellId, brd, row, 2);
  EXPECT_EQ(cellIdx[0], bestCellId);
  EXPECT_EQ(cellValues[bestCellId], value);
}