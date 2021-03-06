#include "config.h"
//#ifdef USE_EMSCRIPTEN
//#include <emscripten.h>
//#endif
#include <iostream>
using namespace std;

#include "BoardRow.h"
#include "Board.h"
#include "Game.h"
#include "IndexGenerator.h"
#include "Search.h"

Game* game = nullptr;
Board* board = nullptr;
IndexGenerator* idxGen = nullptr;

int main() {
  idxGen = new IndexGenerator(BOARD_ROW);
  game = new Game();
  board = new Board();
  board->clean();

  int situation = 0;
  if (situation == 1) {
    // depth 7:10 bordeval  5.91s
    // depth 7:10 moveeval  0.87s
    // should give 108 or 106
    board->setCell(120, 1);
    board->setCell(135, 2);
    board->setCell(119, 1);
    board->setCell(136, 2);
    board->setCell(103, 1);
    board->setCell(137, 2);
    board->setCell(138, 1);
    board->setCell(151, 2);
    board->setCell(121, 1);
    board->setCell(118, 2);
    game->setPly(10);
  } else if (situation == 2) {
    board->setCell(119, 1);
    board->setCell(120, 2);
    board->setCell(118, 1);
    board->setCell(103, 2);
    board->setCell(134, 1);
    board->setCell(137, 2);
    board->setCell(133, 1);
    board->setCell(86, 2);
    game->setPly(8);
  } else if (situation == 3) {
    board->setCell(119, 1);
    board->setCell(104, 2);
    board->setCell(102, 1);
    board->setCell(121, 2);
    board->setCell(85, 1);
    game->setPly(5);
  }

  cout << "main() done v41 ev, row: " << BOARD_ROW << endl;
	return 0;
}

extern "C" void releaseApp() {
  delete game;
  delete board;
}

extern "C" int doNextMove(int depth, char* board_ptr) {
  cout << "doNextMove() ply: " << game->getPly() << endl;
  if (game->isOver()) {
    cout << "doNextMove() gameover" << endl;
    return -1; // 1
  }
  if (game->getPly() < 4) depth = 5;
  Search search(depth);
  SearchResult sr;
  const int actingPlayer = game->getActingTeam();
  if (game->getPly() == 0) {
    sr.move = board->getCellPly0();
    sr.value = 1;
  }
  else {
    search.doSearch(sr, actingPlayer, board);
  }
  if (!board->isValidMove(sr.move)) return -1;
  board->setCell(sr.move, actingPlayer); 
  //board_ptr[sr.move] = game->getActingTeam();
  board->copyBoard(board_ptr); // later on just use row above
  game->incPly();
  cout << "doNextMove() acting: " << (int)actingPlayer << ", move: " << sr.move << ", value: " 
     << sr.value << ", depth: " << depth << ", nNodes: " << sr.nNodes << endl;
  sr.print(game->getPly(), depth);
  return game->checkForWin(board) ? -1 : sr.move;
}

extern "C" int startNewGame() {
	game = new Game();
	return 0;
}

extern "C" int markEmptyCell(char* board_ptr, int cellIdx) {
  if (game->isOver()) return 0;
  if (board->isValidMove(cellIdx)) {
    board_ptr[cellIdx] = game->getActingTeam();
    const int actingPlayer = game->getActingTeam();
    board->setCell(cellIdx, actingPlayer);
    game->incPly();
    return game->checkForWin(board) ? 0 : 1;
  }
	return -1;
}