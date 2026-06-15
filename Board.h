#include "Piece.h"
class Board {
public:
	int pieceBoard[64];
	Piece* piecesList;
	bool whiteToMove;
	bool whiteCastleL, whiteCastleR, blackCastleL, blackCastleR;
	int nMovesNoCapture;

	Board();

	void init();
	void makeMove(int from, int to);
	void undoMove();
	void print();

	bool isLegalMove(int from, int to);
	bool isSquareAttacked(int square);
};

