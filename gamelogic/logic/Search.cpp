#include <assert.h>
#include "Search.h"
#include "IndexGenerator.h"
#include "ranking.h"

#include <iostream>
using namespace std;

SearchResult::SearchResult() {
  move = -22;
  value = -2;
  nNodes = 0;
}
void SearchResult::print(int ply, int searchDepth) {
  for (std::list<SelectTrace>::iterator it = selectTrace.begin(); it != selectTrace.end(); ++it) {
    cout << "ply: " << ply + (searchDepth - it->depth - 1)  << " dp: " << it->depth << " mv: " << it->move << " val: " << it->value << endl;
  }
}

Search::Search(int depth)
  :_depth(depth)
{
  actingPlayer = -1;
  oppPlayer = -1;
  nNodesSearched = 0;
}

Search::~Search()
{
}


int Search::minmax(GamestateNode* const node, int depth, bool shouldMax, char plyPlayer, int alpha, int beta) {
  node->applyMoveToBoard(plyPlayer);
  nNodesSearched += 1;
  
  if (node->isWin) {
    // will reward shortest path to win by adding depth
    node->value = plyPlayer == actingPlayer ? fiveInRow + depth : -fiveInRow;
    return node->value;
  }

  if (depth == 0) {
    // should we evaluate the whole board?
 #ifdef DEBUG_SEARCH
    cout << "depth: " << depth << ", cmove: " << node->_move << ", value: " << node->value << "     " << std::endl;
 #endif
    return node->value;
  }

  char nextPlyPlayer = plyPlayer == 1 ? 2 : 1;
  GamestateNode* children = node->generateChildren(depth, decideNbOfChildren(depth), nextPlyPlayer);
  int value = -5; // -5 just indicating error
  for (int i = 0; i < node->_nChildren; i++) {
    GamestateNode* child = &children[i];
#ifdef DEBUG_SEARCH
    cout << "depth: " << depth << ", cmove: " << children->_move << ", value: " << node->value << ", parent: " << node->_move << "     " << std::endl;
#endif
    value = minmax(child, depth - 1, !shouldMax, nextPlyPlayer, alpha, beta);
    
    if (shouldMax) alpha = (value > alpha) ? value : alpha; // max(value, alpha)
    else beta = (value < beta) ? value : beta; // min(value, beta)
    if (beta <= alpha) break;
    if (child->isWin) {
      node->_nChildren = i + 1;
      break;
    }
  }

  // reached top node
  if (node->_move == -1) {
    GamestateNode* selection = GamestateNode::getBestChild(children, node->_nChildren, shouldMax);
    if (selection != 0) {
      selection->addToSelectionHistory(depth - 1);
      searchResult.move = selection->_move;
      searchResult.value = selection->value;
      searchResult.selectTrace = selection->selectTrace;
      delete[] children;
    }
    return searchResult.value;
  }
  GamestateNode* selection = GamestateNode::getBestChild(children, node->_nChildren, shouldMax);
  const int bestValue = selection->value;
  selection->addToSelectionHistory(depth - 1);
  node->addToSelectionHistory(selection);
  
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
  sr.selectTrace = searchResult.selectTrace;
  sr.nNodes = nNodesSearched;
  delete node0;
}

int Search::decideNbOfChildren(int curDepth) {
  //if (curDepth == _depth) return curDepth; // top level
  //return 2;
  return curDepth + 1;
}
