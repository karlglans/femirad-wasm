#ifndef GAMESTATENODE
#define GAMESTATENODE

#include <list>
#include "common_types.h"
#include "Board.h"
#include "IndexGenerator.h"

class GamestateNode
{
public:
  Move _move;
  Board board;
  int value;
  int preliminaryValue;
  bool isWin;
  short _nChildren;
  std::list<SelectTrace> selectTrace; // mainly for debugging
  Board* getBoard();
  void setMove(Move);
  void copyBoard(Board* board);
  void applyMoveToBoard(char actingPlayer);
  static GamestateNode* getBestChild(GamestateNode* nodes, int nNodes, bool max);
  static short* premadeIndecis;
  GamestateNode* generateChildren(int depth, int maxNbChildren, int team);
  bool checkWin(int team);
  int generateChildMoves(int depth, int team);
  void addToSelectionHistory(int depth);
  void addToSelectionHistory(GamestateNode* child);

  GamestateNode();
  ~GamestateNode();
};
#endif
