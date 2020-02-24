#include "GamestateNode.h"
#include "ranking.h"
#include "Boardevaluator.h"

Boardevaluator boardeval;
const int highValue = fiveInRow * 100;

Board * GamestateNode::getBoard()
{
  return &board;
}

void GamestateNode::setMove(Move move)
{
  _move = move;
}

void GamestateNode::copyBoard(Board * srcBoard)
{
  board.copyBoard(srcBoard);
}

void GamestateNode::applyMoveToBoard(char actingPlayer)
{
  if (_move != -1) {
    board.setCell(_move, actingPlayer);
  }
}

bool  GamestateNode::checkWin(int team) {
  return board.evaluateWin(team) >= fiveInRow;
}

GamestateNode * GamestateNode::getBestChild(GamestateNode* nodes, int nNodes, bool isMaximizing) {
  // NOTE: maybe return ptr to node instead of just best value
  int bestVal = isMaximizing ? -max_value : max_value;
  int childIdx = -1;
  if (nNodes == 0) {
    return 0;
  }
  else if (isMaximizing) {
    for (int n = 0; n < nNodes; n++) {
      if (nodes[n].value > bestVal) {
        childIdx = n;
        bestVal = nodes[n].value;
      }
    }
  }
  else {
    for (int n = 0; n < nNodes; n++) {
      if (nodes[n].value < bestVal) {
        childIdx = n;
        bestVal = nodes[n].value;
      }
    }
  }
  if (childIdx == -1) return 0;
  return &nodes[childIdx];
}

short indexBuffer[BOARD_ROW * BOARD_ROW]; // internal buffer 
int cellSignificanceBuffer[BOARD_ROW * BOARD_ROW];

int GamestateNode::generateChildMoves(int maxNbChildren, int team) {
  const char* boardCells = board.getBoard();
  const int max_board_cells = board.getSize();
  int nChildren = 0;

  boardeval.evaluateBoard(boardCells, cellSignificanceBuffer, board.getRow(), team);
  nChildren = boardeval.sortCellValues(cellSignificanceBuffer, indexBuffer, max_board_cells, maxNbChildren); // 3

  return nChildren;
}

GamestateNode* GamestateNode::generateChildren(int depth, int maxNbChildren, int team)
{
  int nChildren = generateChildMoves(maxNbChildren, team);
  GamestateNode* children = new GamestateNode[nChildren];
  for (int c = 0; c < nChildren; c++) {
    GamestateNode* child = &children[c];
    child->copyBoard((Board*)&board);
    child->setMove(indexBuffer[c]);
    int nodeSigniValue = cellSignificanceBuffer[indexBuffer[c]];
    child->preliminaryValue = nodeSigniValue;
    child->isWin = nodeSigniValue >= fiveInRow;
    child->value = depth == 1? nodeSigniValue: depth%2 == 0 ? max_value: -max_value;
  }
  _nChildren = nChildren;
  return children;
}

GamestateNode::GamestateNode()
{
  _nChildren = -1;
  value = -max_value; // check: sinse node 0 is maximizing
  isWin = false;
  _move = -4; // nagative value indicating uninitialized
  preliminaryValue = -1;
}

GamestateNode::~GamestateNode() {}

void GamestateNode::addToSelectionHistory(int depth) {
  SelectTrace st;
  st.move = _move;
  st.value = preliminaryValue;
  st.depth = depth;
  selectTrace.push_back(st);
}

void GamestateNode::addToSelectionHistory(GamestateNode* child) {
  std::list<SelectTrace> childHist = child->selectTrace;
  for (auto it = childHist.begin(); it != childHist.end(); ++it) selectTrace.push_back(*it);
}
