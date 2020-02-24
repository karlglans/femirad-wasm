#ifndef GAMESTATENODE
#define GAMESTATENODE

#include "common_types.h"
#include "Board.h"
#include "IndexGenerator.h"

class GamestateNode
{
  //static IndexGenerator* indexGen;
public:
  //static void setIndexGenerator(IndexGenerator* indexGen);
  //GamestateNode* parent;
  Move _move;
  Board board;
  int value;
  bool isWin;
  short _nChildren;
  Board* getBoard();
  void setMove(Move);
  void copyBoard(Board* board);
  void applyMoveToBoard(char actingPlayer);
  static int getValueFromBestChild(GamestateNode* nodes, int nNodes, bool max);
  static GamestateNode* getBestChild(GamestateNode* nodes, int nNodes, bool max);
  static short* premadeIndecis;
  GamestateNode* generateChildren(int depth, int maxNbChildren, int team);
  bool checkWin(int team);
  int generateChildMoves(int depth, int team);

  GamestateNode();
  ~GamestateNode();
};
#endif
