#ifndef SEARCH
#define SEARCH

#include "Board.h"
#include "GamestateNode.h"
#include "Boardevaluator.h"
#include "common_types.h"

struct SearchResult {
  short move;
  int value;
  int nNodes;
  std::list<SelectTrace> selectTrace;
  void print(int ply, int searchDepth);
  SearchResult();
};

class Search
{
private:
  const int _depth;
  int nPrune;
  SearchResult searchResult;
  char actingPlayer;
  char oppPlayer;
  Boardevaluator bordeval;
  int minmax(GamestateNode* const node, int depth, bool shouldMax, char plyPlayer, int alpha, int beta);
  int decideNbOfChildren(int curDepth);
public:
  int nNodesSearched; // mainly for debugging
  Search(int depth);
  void doSearch(SearchResult& sr, char actingPlayer, Board* board);
  ~Search();
};
#endif
