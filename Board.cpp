#include "Piece.h"
#include "Board.h"
Board::Board() {
	init();
}

void Board::init()
{
	whiteToMove = true;
	whiteCastleL = whiteCastleR = blackCastleL = blackCastleR = true;
	nMovesNoCapture = 0;


}

void Board::makeMove(int from, int to)
{
	int piece = pieceBoard[from];
	pieceBoard[from] = 0;
	pieceBoard[to] = piece;
}

bool Board::isLegalMove(int from, int to)
{
	return false;
}
