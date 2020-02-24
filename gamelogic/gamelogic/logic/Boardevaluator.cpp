#include "Boardevaluator.h"
#include <assert.h>
#include <algorithm>
#include <vector>
#include "ranking.h"


Boardevaluator::Boardevaluator(){}
Boardevaluator::~Boardevaluator(){}

// should give every cell a value how indicating how interesting it is for a search
inline int Boardevaluator::calcCellLineSignificance(int* line) {
  int subLineValue = 0, lineValue = 0;
  int sum, s, o; // endpos, startPos, offset
  int ownHits = 0, oppHits = 0;
  bool subLineAaccessible;
  int lineCell;
  int canUseOwn = 0, canUseOpp = 0;

  const int l0 = line[0], l1 = line[1], l2 = line[2], l3 = line[3], l4 = line[4], l5 = line[5], l6 = line[6], l7 = line[7], l8 = line[8];
  
  const bool a0 = l0 == 1, b0 = l0 == 2;
  const bool a1 = l1 == 1, b1 = l1 == 2;
  const bool a2 = l2 == 1, b2 = l2 == 2;
  const bool a3 = l3 == 1, b3 = l3 == 2;
  const bool a5 = l5 == 1, b5 = l5 == 2;
  const bool a6 = l6 == 1, b6 = l6 == 2;
  const bool a7 = l7 == 1, b7 = l7 == 2;
  const bool a8 = l8 == 1, b8 = l8 == 2;

  const bool z0 = l0 == 0;
  const bool z1 = l1 == 0;
  const bool z2 = l2 == 0;
  const bool z3 = l3 == 0;
  const bool z5 = l5 == 0;
  const bool z6 = l6 == 0;
  const bool z7 = l7 == 0;
  const bool z8 = l8 == 0;

  if (a0 && a1 && a2 && a3) return fiveInRow;
  if (a1 && a2 && a3 && a5) return fiveInRow;
  if (a2 && a3 && a5 && a6) return fiveInRow;
  if (a3 && a5 && a6 && a7) return fiveInRow;
  if (a5 && a6 && a7 && a8) return fiveInRow;

  if (b0 && b1 && b2 && b3) return blockFiveInRow;
  if (b1 && b2 && b3 && b5) return blockFiveInRow;
  if (b2 && b3 && a5 && b6) return blockFiveInRow;
  if (b3 && b5 && b6 && b7) return blockFiveInRow;
  if (b5 && b6 && b7 && b8) return blockFiveInRow;

  if (z0 && a1 && a2 && a3 && z5) return open4row;
  if (z1 && a2 && a3 && a5 && z6) return open4row;
  if (z2 && a3 && a5 && a6 && z7) return open4row;
  if (z3 && a5 && a6 && a7 && z8) return open4row;

  if (z0 && b1 && b2 && b3 && z5) return blockOpen4row;
  if (z1 && b2 && b3 && b5 && z6) return blockOpen4row;
  if (z2 && b3 && b5 && b6 && z7) return blockOpen4row;
  if (z3 && b5 && b6 && b7 && z8) return blockOpen4row;

  for (s = 0; s < 5; s++) {
    subLineAaccessible = true;
    subLineValue = ownHits = oppHits = 0;
    canUseOwn = canUseOpp = 1;
    for (o = 0; o < 5; o++) {
      sum = s + o;
      lineCell = line[sum];
      if (lineCell == 0) subLineValue += 1;
      else if (lineCell == inaccessible) {
        // this 5 cells was useless
        subLineAaccessible = false;
        o = 5;
        continue;
      }
      else if (lineCell == own_cell) {
        ownHits += 1;
        canUseOpp = 0;
      } else if (lineCell == oppo_cell) {
        oppHits += 1;
        canUseOwn = 0;
      }
      if (canUseOwn == 0 && canUseOpp == 0) {
        subLineAaccessible = false;
        o = 5;
        continue;
      }
    }
    if (subLineAaccessible) { // TODO look into these values later. They will affect passive/aggressive 
      subLineValue += (9 * canUseOwn * (ownHits * ownHits) + 10 * canUseOpp *  (oppHits * oppHits));
      lineValue += subLineValue;
    }
  }
  return lineValue;
}

const int max_steps = 6;
int debugit;

int line9[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, };

// the main purpose of this method is to put a value on each board position, that inicates how interesting this position is for
// a deeper search.
int Boardevaluator::evaluateBoard(const char* const board, int* const cellValues, const int row, const char team) {
  int i, j, ci, cj, dir;
  int step, pos, cpos = 0; // cpos: centrum position for ci-cj-loop.
  int cellvalue;
  int sum = 0;
  int rowX4 = row * 4;
  for (cj = 0; cj < row; cj++) {
    for (ci = 0; ci < row; ci++, cpos++) {
      cellvalue = 0;
      if (board[cpos] != free_cell) {
        cellValues[cpos] = 0;
        continue;
      }
      if (cpos == 136) {
        debugit += 1;
      }
      for (dir = 0; dir < 4; dir++) {
        // start positions:
        if (dir == 0) { pos = cpos - 4; i = ci - 4; j = cj; }
        else if (dir == 1) { pos = cpos + rowX4 - 4; i = ci - 4; j = cj + 4; }
        else if (dir == 2) { pos = cpos + rowX4; i = ci; j = cj + 4; }
        else if (dir == 3) { pos = cpos + rowX4 + 4; i = ci + 4; j = cj + 4; }
        for (step = 0; step < 9; step++) {
          if (i < 0 || i >= row || j < 0 || j >= row) {
            line9[step] = inaccessible;
          } else if (board[pos] == 0) line9[step] = 0;
          else line9[step] = board[pos] == team ? 1: 2;

          // next step
          if (dir == 0) { pos += 1; i += 1; }
          else if (dir == 1) { pos += 1; pos -= row; i += 1; j -= 1; }
          else if (dir == 2) { pos -= row; j -= 1; }
          else if (dir == 3) { pos -= 1; pos -= row; i -= 1; j -= 1; }
        }
        cellvalue += calcCellLineSignificance(line9);
      }
      cellValues[cpos] = cellvalue;
      sum += cellvalue;
    }
  }
  return sum;
}

// should value how good a move is for team
int Boardevaluator::evaluateCell(const short cpos, const char* board, const int row, const char team) {
  int i, j, ci, cj, dir;
  int step, pos;
  int cellvalue = 0;
  int rowX4 = row * 4;
  ci = cpos % row;
  cj = cpos / row;
  for (dir = 0; dir < 4; dir++) {
    // start positions:
    if (dir == 0) { pos = cpos - 4; i = ci - 4; j = cj; }
    else if (dir == 1) { pos = cpos + rowX4 - 4; i = ci - 4; j = cj + 4; }
    else if (dir == 2) { pos = cpos + rowX4; i = ci; j = cj + 4; }
    else if (dir == 3) { pos = cpos + rowX4 + 4; i = ci + 4; j = cj + 4; }
    for (step = 0; step < 9; step++) {
      if (i < 0 || i >= row || j < 0 || j >= row) {
        line9[step] = inaccessible;
      }
      else if (board[pos] == free_cell) line9[step] = free_cell;
      else line9[step] = board[pos] == team ? own_cell : oppo_cell;

      // next step
      if (dir == 0) { pos += 1; i += 1; }
      else if (dir == 1) { pos += 1; pos -= row; i += 1; j -= 1; }
      else if (dir == 2) { pos -= row; j -= 1; }
      else if (dir == 3) { pos -= 1; pos -= row; i -= 1; j -= 1; }
    }
    cellvalue += calcCellLineSignificance(line9);
  }
  return cellvalue;
}

struct IndexAndValue {
  int pos;
  int value;
};

struct less_than_dist {
  inline bool operator() (const IndexAndValue& struct1, const IndexAndValue& struct2) {
    return (struct1.value > struct2.value);
  }
};

int Boardevaluator::sortCellValues(int* cellValues, short* cellIdx, const int size, const int maxValues) {
  int nGoodValues = 0;
  const int threshold = 0;
  std::vector<IndexAndValue> idxAndDist(size);
  for (int i = 0; i < size; i++) {
    idxAndDist[i].value = cellValues[i];
    idxAndDist[i].pos = i;
    if (cellValues[i] > threshold) {
      nGoodValues += 1;
    }
  }
  int nPotetialValues = 0;
  std::sort(idxAndDist.begin(), idxAndDist.end(), less_than_dist());
  for (int i = 0; i < size; i++) {
    if (i >= maxValues) {
      //i = size;
      break;
    }
    nPotetialValues++;
    cellIdx[i] = idxAndDist[i].pos;
  }
  return nPotetialValues;
}

