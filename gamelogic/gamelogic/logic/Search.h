#ifndef SEARCH
#define SEARCH

#include "Board.h"
#include "GamestateNode.h"
#include "Boardevaluator.h"

struct SearchResult{
  short move;
  int value; // maybe should be called significance
  int goalValue;
};

class Search
{
private:
  const int _depth;
  SearchResult searchResult;
  char actingPlayer;
  char oppPlayer;
  Boardevaluator bordeval;
  int minmax(GamestateNode* const node, int depth, bool shouldMax, char plyPlayer, int alpha, int beta);
  int decideNbOfChildren(int curDepth);
public:
  Search(int depth);
  void doSearch(SearchResult& sr, char actingPlayer, Board* board);
  ~Search();
};
#endif
