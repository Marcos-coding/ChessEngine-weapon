#pragma once

#include <cstdint>
namespace chess {
    enum class PieceType : std::uint8_t {
        NONE = 0,
        PAWN = 1,
        KNIGHT = 2,
        BISHOP = 3,
        ROOK = 4,
        QUEEN = 5,
        KING = 6
    };

    enum class PieceColor : std::uint8_t {
        BLACK = 0,
        WHITE = 1
    };

    struct Piece {
        PieceType type;
        PieceColor color;
    };

}