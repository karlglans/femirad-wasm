#include <assert.h>
#include "Search.h"
#include "IndexGenerator.h"
#include "ranking.h"

#include <iostream>
using namespace std;





Search::Search(int depth)
  :_depth(depth)
{
}

Search::~Search()
{
}


int Search::minmax(GamestateNode* const node, int depth, bool shouldMax, char plyPlayer, int alpha, int beta) {
  node->applyMoveToBoard(plyPlayer);
  
  if (node->isWin) {
    // will reward shortest path to win by adding depth
    node->value = plyPlayer == actingPlayer ? fiveInRow + depth : -fiveInRow;
    return node->value;
  }

  if (depth == 0) {
    // should we evaluate the whole table?
 #ifdef DEBUG_SEARCH
    cout << "depth: " << depth << ", cmove: " << node->_move << ", value: " << node->value << "     " << std::endl;
 #endif
    return node->value;
  }

  char nextPlyPlayer = plyPlayer == 1 ? 2 : 1;
  GamestateNode* children = node->generateChildren(depth, decideNbOfChildren(depth), nextPlyPlayer);
  int value = -5; // -5 just indicating error
  for (int i = 0; i < node->_nChildren; i++) {
#ifdef DEBUG_SEARCH
    cout << "depth: " << depth << ", cmove: " << children->_move << ", value: " << node->value << ", parent: " << node->_move << "     " << std::endl;
#endif
    value = minmax(&children[i], depth - 1, !shouldMax, nextPlyPlayer, alpha, beta);
    if (shouldMax) alpha = (value > alpha) ? value : alpha; // max(value, alpha)
    else beta = (value < beta) ? value : beta; // min(value, beta)
    if (beta <= alpha) break;
  }

  // reached top node
  if (node->_move == -1) {
    GamestateNode* selection = GamestateNode::getBestChild(children, node->_nChildren, shouldMax);
    if (selection != 0) {
      searchResult.move = selection->_move;
      searchResult.value = selection->value;
      searchResult.goalValue = value;
    }
    //delete[] children;
    return searchResult.value;
  }

  const int bestValue = GamestateNode::getValueFromBestChild(children, node->_nChildren, shouldMax);
  delete[] children;
  node->value = bestValue;
  return bestValue;
}

void Search::doSearch(SearchResult & sr, char actingPlayerStart, Board * board)
{
  // maybe move since it can be reused
  const int row = board->getRow();

  // node0 is the current gameboard. It contains the move from previus player
  GamestateNode* node0 = new GamestateNode();
  node0->getBoard()->copyBoard(board);
  node0->setMove(-1); // the move is already written into the board

  searchResult.move = -22; // indicating no move found. should not happen
  actingPlayer = actingPlayerStart;
  oppPlayer = actingPlayer == 1 ? 2 : 1;
  minmax(node0, _depth, true, oppPlayer, -max_value, max_value);
  sr.move = searchResult.move;
  sr.value = searchResult.value;

  delete node0;
}

int Search::decideNbOfChildren(int curDepth) {
  //if (curDepth == _depth) return curDepth; // top level
  return curDepth + 1;
}