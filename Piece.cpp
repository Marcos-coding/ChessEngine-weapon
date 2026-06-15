#include "Piece.h";

Piece::Piece() {
	type = EMPTY;
	color = NONE;
	pos = 0;
}

Piece::Piece(PieceType t, Color c, int p) {
	type = t;
	color = c;
	pos = p;
}

void Piece::move(int newPos)
{
	pos = newPos;
}

void Piece::destroy()
{
	type = EMPTY;
	color = NONE;
}
