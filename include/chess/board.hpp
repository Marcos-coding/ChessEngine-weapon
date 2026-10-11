#pragma once

#include "types.hpp"
#include <array>
#include <cstdint>

namespace chess {

    class Board {
    public:
        // Constructor: inicializa el tablero.
        Board();

        // Consulta qué pieza hay en una casilla.
        Piece pieceAt(Square square) const;

        // Consulta a quién le toca jugar.
        Color sideToMove() const;

    private:
        // Las 64 casillas del tablero.
        std::array<Piece, 64> squares_{};

        // Estado de la partida.
        Color sideToMove_ = Color::White;
        std::uint8_t castlingRights_ = 0;
        Square enPassantSquare_ = 64;
    };
}