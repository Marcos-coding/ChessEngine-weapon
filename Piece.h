#pragma once
enum PieceType {
	EMPTY,
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING
};

enum Color {
	NONE,
	BLACK,
	WHITE
};

struct Piece {
	PieceType type;
	Color color;
	int pos;

	Piece();
	Piece(PieceType t, Color c, int pos);

	void move(int pos);
	void destroy();
};