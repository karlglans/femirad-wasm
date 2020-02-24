#include "GamestateNode.h"
#include "ranking.h"
#include "Boardevaluator.h"

//short* GamestateNode::premadeIndecis = 0; // maybe guard better
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

int GamestateNode::getValueFromBestChild(GamestateNode* nodes, int nNodes, bool isMaximizing)
{
  // NOTE: maybe return ptr to node instead of just best value
  int bestVal = isMaximizing ? -highValue : highValue;
  if (nNodes == 0) {
    return -2;
  }
  else if(isMaximizing) {
    for (int n = 0; n < nNodes; n++) {
      if (nodes[n].value > bestVal) bestVal = nodes[n].value;
    }
  } else {
    for (int n = 0; n < nNodes; n++) {
      if (nodes[n].value < bestVal) bestVal = nodes[n].value;
    }
  }
  return bestVal;
}

GamestateNode * GamestateNode::getBestChild(GamestateNode * nodes, int nNodes, bool isMaximizing)
{
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
  //bool isTopLayer = _move == -1;

  boardeval.evaluateBoard(boardCells, cellSignificanceBuffer, board.getRow(), team);
  nChildren = boardeval.sortCellValues(cellSignificanceBuffer, indexBuffer, max_board_cells, maxNbChildren); // 3

  return nChildren;
}

GamestateNode* GamestateNode::generateChildren(int depth, int maxNbChildren, int team)
{
  int nChildren = generateChildMoves(depth, team);
  int nodeSigniValue = -1;
  GamestateNode* children = new GamestateNode[nChildren];
  for (int c = 0; c < nChildren; c++) {
    children[c].copyBoard(&board);
    children[c].setMove(indexBuffer[c]);
    nodeSigniValue = cellSignificanceBuffer[indexBuffer[c]];
    children[c].isWin = nodeSigniValue >= fiveInRow;
    // kan value ocks� s�ttas h�r? 
    children[c].value = depth == 1? nodeSigniValue: depth%2 == 0 ? max_value: -max_value;
  }
  _nChildren = nChildren;
  return children;
}

GamestateNode::GamestateNode()
{
  _nChildren = -1;
  value = -max_value; // check: sinse node 0 is maximizing
  isWin = false;
}


GamestateNode::~GamestateNode()
{
}